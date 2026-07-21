# Axiom

> A browser-native rigid body physics engine built entirely from scratch in modern C++, compiled to WebAssembly, and rendered in real time using Three.js.

---

# Demo

https://github.com/user-attachments/assets/5f8750bb-fcdf-4aa8-b727-ad7ca2cfa523

---

# Overview

Axiom is a browser-native rigid body physics engine written entirely in modern C++. The project explores the architecture of real-time physics engines by implementing every major subsystem from first principles rather than relying on existing mathematics or physics libraries.

The engine is compiled to WebAssembly using Emscripten and executed directly inside the browser. JavaScript serves only as the interface between the simulation and the rendering pipeline, while all numerical computations—including integration, collision detection, and collision response—remain inside native C++.

The objective of Axiom is not only to simulate rigid bodies, but also to understand and recreate the engineering principles that underpin modern real-time physics engines.

---

# Motivation

Modern physics engines such as Bullet, PhysX, and Box2D provide highly optimized implementations of rigid body simulation but abstract away much of the mathematics and architecture behind them.

Axiom was developed to explore these systems directly by implementing the engine from first principles. Every major subsystem—including vector mathematics, rigid body dynamics, numerical integration, collision detection, collision response, and simulation management—was designed specifically for this project.

The project also investigates how WebAssembly enables modern C++ systems programming to execute efficiently inside web browsers while maintaining a clear separation between simulation and rendering.

---

# Features

## Mathematics

- Custom Vector3 implementation
- Operator overloading
- Dot product
- Cross product
- Magnitude and normalization
- Distance calculations

## Physics Engine

- Particle simulation
- Rigid body simulation
- Physics world management
- Fixed timestep simulation
- Semi-Implicit Euler integration
- Gravity simulation
- Velocity integration

## Collision System

- Sphere–sphere collision detection
- Contact generation
- Impulse-based collision resolution
- Positional correction
- Floor collision handling
- World boundary constraints

## Browser Integration

- Native C++ compiled to WebAssembly
- JavaScript bindings using Embind
- Real-time synchronization between simulation and rendering
- Three.js visualization

Unlike conventional browser physics simulations, every numerical computation is executed inside the native C++ engine. JavaScript is responsible only for interacting with the WebAssembly module and rendering object transforms through Three.js.

---

# Architecture

```text
                              Browser
                                  │
                                  ▼
                      Three.js Rendering Layer
                                  │
                                  ▼
                      JavaScript Interface (ES6)
                                  │
                                  ▼
                       Embind JavaScript Bindings
                                  │
                                  ▼
                 WebAssembly Runtime (Compiled C++)
                                  │
                                  ▼
┌──────────────────────────────────────────────────────────────┐
│                          AXIOM ENGINE                        │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Mathematics                                                 │
│   • Vector3                                                  │
│                                                              │
│  Dynamics                                                    │
│   • Particle                                                 │
│   • RigidBody                                                │
│   • PhysicsWorld                                             │
│                                                              │
│  Simulation                                                  │
│   • Semi-Implicit Euler Integration                          │
│   • Gravity                                                  │
│                                                              │
│  Collision                                                   │
│   • Sphere–Sphere Detection                                  │
│   • Impulse Resolution                                       │
│   • Positional Correction                                    │
│                                                              │
└──────────────────────────────────────────────────────────────┘
                                  │
                                  ▼
                             Modern C++17
```

---

# Engineering Challenges

Developing Axiom required solving several problems commonly abstracted away by mature physics engines.

- Designing a reusable vector mathematics library.
- Maintaining numerical stability during real-time simulation.
- Resolving rigid body collisions using impulse-based dynamics.
- Preventing persistent interpenetration through positional correction.
- Exposing native C++ classes to JavaScript through Embind.
- Synchronizing a WebAssembly simulation with a Three.js rendering pipeline.

These challenges shaped the modular architecture of the engine and motivated a clear separation between simulation, browser interoperability, and visualization.

---

# Physics Implementation

## Numerical Integration

Motion is integrated using **Semi-Implicit Euler Integration**, a stable numerical integration technique widely used in real-time physics engines.

```text
velocity += acceleration × Δt

position += velocity × Δt
```

Updating velocity before position improves numerical stability compared to explicit Euler integration while remaining computationally inexpensive.

---

## Collision Detection

Sphere collisions are detected by comparing the distance between object centers with the sum of their radii.

```text
||p₂ − p₁|| < r₁ + r₂
```

The collision normal is computed from the normalized vector between object centers and is used during impulse resolution.

---

## Collision Resolution

Rigid body collisions are resolved using an impulse-based solver.

The impulse magnitude depends on

- Relative velocity
- Collision normal
- Mass
- Coefficient of restitution

The computed impulse is applied to both bodies, conserving linear momentum while producing elastic or inelastic collisions depending on restitution.

---

## Positional Correction

Because collisions are evaluated at discrete timesteps, small overlaps may remain after collision resolution.

Axiom applies positional correction immediately after impulse resolution to separate intersecting bodies and improve long-term simulation stability.

---

# Project Structure

```text
Axiom
│
├── engine
│   ├── math
│   ├── dynamics
│   ├── collision
│   ├── world
│   └── bindings
│
├── web
│   ├── index.html
│   ├── main.js
│   ├── scene.js
│   └── style.css
│
├── docs
│   ├── demo.gif
│   ├── physics.md
│   ├── architecture.md
│   └── images
│
├── CMakeLists.txt
├── package.json
├── README.md
└── LICENSE
```

---

# Technology Stack

| Category | Technology |
|-----------|------------|
| Language | C++17 |
| Browser Runtime | WebAssembly |
| Toolchain | Emscripten |
| JavaScript Bindings | Embind |
| Rendering | Three.js |
| Build System | CMake |
| Development | Vite |

---

# Building

## Clone

```bash
git clone https://github.com/<your-username>/Axiom.git
cd Axiom
```

## Configure

```bash
mkdir build
cd build

emcmake cmake ..
```

## Build

```bash
emmake make
```

## Run

```bash
cd ../web
npm install
npm run dev
```

Open the local development server in your browser.

---

# Documentation

Additional implementation details are available in the `docs` directory.

- **physics.md** — Mathematical foundations and simulation algorithms
- **architecture.md** — Engine architecture and subsystem design

---

# Roadmap

## Version 1.1

- Force generators
- Springs
- Additional collider primitives

## Version 2.0

- Rotational dynamics
- Angular momentum
- Inertia tensors
- Quaternion mathematics

## Version 3.0

- SAT collision detection
- Broad-phase collision detection
- Spatial partitioning
- Constraint solver
- Performance optimization
- Multithreaded simulation

---

# Closing Remarks

Axiom represents an exploration of real-time physics engine architecture through original implementation rather than library integration.

The project demonstrates how modern C++, WebAssembly, and browser technologies can be combined to build high-performance simulation software while maintaining a clean separation between simulation, interoperability, and visualization.

Although the current implementation focuses on rigid body dynamics, the architecture is intentionally modular and designed to support future extensions such as rotational dynamics, advanced collision detection algorithms, constraints, and spatial acceleration structures.

---

# License

This project is licensed under the MIT License.

See the [LICENSE](LICENSE) file for details.
