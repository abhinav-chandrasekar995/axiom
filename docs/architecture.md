# Engine Architecture

This document describes how Axiom's subsystems are organized and how data flows from the simulation core to the rendered frame.

## Design Goals

- **No external physics or math dependencies.** Every subsystem — vector math, integration, collision detection, collision response — is implemented from first principles.
- **Clear separation of concerns.** The C++ engine has no knowledge of rendering, and the JavaScript/Three.js layer has no knowledge of physics — it only reads state.
- **Minimal, well-defined boundary crossing.** Data crosses from C++ to JavaScript exactly once per frame, in a single flat array, rather than through many small calls.

## Layered Structure

Vector3 — math primitive, no dependencies
↑ (composition — Particle contains Vector3 fields)
Particle — position, velocity, acceleration, mass; semi-implicit Euler integration
↑ (inheritance — RigidBody extends Particle)
RigidBody — adds shape (radius) and material property (restitution);
owns collision detection and impulse resolution between two bodies
↑ (composition — PhysicsWorld owns many RigidBody instances)
PhysicsWorld — owns all bodies; drives the per-frame simulation step;
performs the broad-phase pairwise collision check and
boundary collision handling


`RigidBody` inherits from `Particle` rather than composing it, since a rigid body genuinely *is* a particle with additional shape and material properties — every method `Particle` exposes (`integrate`, `applyGravity`) remains valid and unmodified on a `RigidBody`. `Particle`, in turn, *has* `Vector3` fields rather than inheriting from `Vector3`, since a particle is not itself a vector — this composition-vs-inheritance distinction is applied deliberately throughout the codebase rather than defaulting to inheritance everywhere.

## Module Responsibilities

| Module | Responsibility |
|---|---|
| `math/Vector3` | 3D vector arithmetic — no knowledge of physics or simulation state |
| `dynamics/Particle` | Point-mass motion under acceleration; no shape, no collision |
| `dynamics/RigidBody` | Spherical shape, collision detection and resolution between two bodies |
| `world/PhysicsWorld` | Owns the full body list; steps simulation; broad-phase collision; boundary handling; flattens state for the JS boundary |
| `bindings/bindings.cpp` | Embind declarations exposing `Vector3`, `RigidBody`, and `PhysicsWorld` to JavaScript |

## Crossing the WebAssembly Boundary

Embind is used to expose C++ classes directly to JavaScript, so that `new Module.PhysicsWorld()`, `world.addBody(...)`, and `world.step(dt)` behave, from JavaScript's perspective, like ordinary method calls on a JavaScript object — the fact that the underlying implementation is compiled C++ is entirely opaque to the calling code.

Two design choices minimize the cost and complexity of this boundary:

**1. Base-class binding for inheritance.** Since `RigidBody` inherits from `Particle` in C++, Embind is told about this relationship explicitly via `class_<RigidBody, base<Particle>>`, so that fields inherited from `Particle` (`position`, `velocity`, `mass`) remain accessible from JavaScript on a `RigidBody` instance without being redeclared.

**2. Flat-array state transfer.** Rather than exposing individual `RigidBody` objects to JavaScript for per-frame reading (which would require many small, relatively expensive boundary crossings), `PhysicsWorld::getPositions()` flattens every body's position into a single `vector<float>`, laid out as `[x₀, y₀, z₀, x₁, y₁, z₁, ...]`. This is registered with Embind via `register_vector<float>`, and results in exactly one boundary crossing per frame regardless of body count — significantly cheaper than crossing the boundary once per body.

## Rendering Integration

Three.js has no knowledge of the physics engine's internals. Each frame, the render loop:

1. Calls `world.step(dt)`, advancing the simulation by one fixed timestep
2. Calls `world.getPositions()`, retrieving the flattened position array
3. Unpacks the array back into per-body `(x, y, z)` triples
4. Assigns each triple to the corresponding Three.js mesh's `position`

This keeps the rendering layer a pure function of simulation state — Three.js never modifies physics state, and the physics engine never references any rendering object. The same `PhysicsWorld` could be driven by a completely different renderer, or by no renderer at all (as in the engine's standalone C++ test executables), without any change to the physics code itself.

## Testing Strategy

Each layer was verified independently before integration:

- `Particle` — verified via a standalone executable simulating free-fall under gravity, cross-checked against the analytical free-fall equation
- `RigidBody` — verified via a standalone executable simulating two spheres on a collision course, checking that momentum is conserved and that velocities correctly reflect off the collision normal
- `PhysicsWorld` — verified via a standalone executable managing multiple bodies simultaneously, confirming gravity, pairwise collision, and Embind bindings all agree with the equivalent pure-C++ results

This bottom-up testing approach meant that once the WebAssembly bridge and Three.js integration were added, any unexpected behavior could be isolated to those specific layers, rather than the underlying physics math.