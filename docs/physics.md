# Physics Implementation

This document describes the mathematical foundations and algorithms used in Axiom's simulation core.

## 1. Vector Mathematics

All spatial quantities — position, velocity, acceleration — are represented using a custom `Vector3` type implementing standard 3D vector operations:

- **Addition / subtraction** — componentwise, used for combining displacements and velocities
- **Scalar multiplication / division** — used for scaling by timestep (`Δt`) and mass
- **Dot product** (`a · b = aₓbₓ + a_y b_y + a_z b_z`) — used to project one vector onto another; central to collision response, where it measures how much relative velocity lies along the collision normal
- **Cross product** — computed for completeness; not used in the current linear-motion-only scope, but required for future rotational dynamics
- **Length / length squared** — length squared is preferred wherever only a distance *comparison* is needed (e.g. collision detection), avoiding an unnecessary square root

## 2. Numerical Integration

Motion is advanced using **semi-implicit Euler integration**:
velocity += acceleration × Δt
position += velocity × Δt

Velocity is updated *before* position, using the newly-updated velocity to advance position in the same step. This is a deliberate choice over explicit (forward) Euler integration, which updates both quantities using the *previous* frame's values. Semi-implicit Euler is unconditionally stable for the simple gravitational and impulse-driven forces used here, and is the standard choice in real-time physics engines where computational cost must remain low and a fixed, small timestep is used (Δt ≈ 0.016s, corresponding to 60Hz).

## 3. Forces

At present, the only continuous force applied is gravity:
acceleration.y += g (g = -9.8 by default, configurable at runtime)

Acceleration is reset to zero at the start of every simulation step, before gravity is reapplied. This prevents acceleration from accumulating indefinitely across frames — a common bug in naive implementations, since `applyGravity()` adds to the existing acceleration rather than setting it outright.

## 4. Collision Detection

Axiom implements sphere–sphere collision detection using a distance check against the sum of the two bodies' radii:
‖p₂ − p₁‖ ≤ r₁ + r₂

To avoid a square root on every pairwise check, the *squared* distance is compared against the *squared* radius sum instead:
‖p₂ − p₁‖² ≤ (r₁ + r₂)²


This is mathematically equivalent and considerably cheaper when checking many body pairs per frame — the primary cost of a naive O(n²) broad phase, which is what `PhysicsWorld::step()` currently performs (every body pair is checked once per frame, with `j` starting at `i+1` to avoid duplicate and self-comparisons).

## 5. Collision Response

Collisions are resolved using an **impulse-based solver**, following the standard formulation used in most real-time physics engines (e.g. Box2D, Bullet).

**Collision normal:**
n = normalize(p₂ − p₁)
**Relative velocity along the normal:**
If `vrel > 0`, the bodies are already separating and no impulse is applied — this early exit prevents redundant or incorrect impulses from being applied to bodies that are moving apart but still numerically overlapping.

**Impulse magnitude:**
j = −(1 + e) × vrel / (1/m₁ + 1/m₂)

where `e = min(e₁, e₂)` is the combined coefficient of restitution of the two bodies. The `(1/m₁ + 1/m₂)` term distributes the impulse according to each body's inverse mass, so heavier bodies experience proportionally smaller velocity changes — consistent with conservation of momentum.

**Applying the impulse:**
v₁ -= (j × n) / m₁
v₂ += (j × n) / m₂

## 6. Positional Correction

Because collisions are only evaluated at discrete timesteps, a fast-moving body can end up with its surface embedded slightly inside another by the time an overlap is detected. Velocity resolution alone does not correct this — it changes *how* the bodies will move going forward, not *where* they currently are.

To fix this, Axiom applies a positional correction immediately after impulse resolution:

penetration = (r₁ + r₂) − ‖p₂ − p₁‖
correction = n × (penetration / 2)

p₁ -= correction
p₂ += correction

Each body is pushed apart by half the penetration depth along the collision normal, symmetrically separating them without introducing any additional velocity change.

## 7. Boundary Collision (Floor and Walls)

Floor and wall collisions are handled as a simplified special case of the general collision response, where one "body" (the boundary) is treated as having infinite mass and zero velocity. This removes the need for a momentum split — only the dynamic body's position and velocity are corrected:

if body's lower/outer edge has crossed the boundary:
snap position back to the boundary surface
velocity_component = -velocity_component × restitution


This mirrors the sphere–sphere resolution's reflection step, but without an impulse calculation, since the boundary cannot move.

## 8. Simulation Loop

Each call to `PhysicsWorld::step(dt)` performs, in order:

1. Apply gravity and integrate motion for every body
2. Reset each body's acceleration to zero
3. Check every unique pair of bodies for collision, resolving any that overlap
4. Check every body against the floor and world boundaries, resolving any that have crossed them

This ordering — integrate first, then resolve collisions — is standard in real-time engines and ensures that collision response always acts on the current frame's updated positions.