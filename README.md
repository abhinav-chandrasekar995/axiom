# Axiom

> A browser-native rigid body physics engine built entirely from scratch in modern C++, compiled to WebAssembly, and rendered in real time using Three.js.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![WebAssembly](https://img.shields.io/badge/WebAssembly-WASM-654FF0)
![Emscripten](https://img.shields.io/badge/Emscripten-Compiled-black)
![Three.js](https://img.shields.io/badge/Three.js-Rendering-black)
![License](https://img.shields.io/badge/License-MIT-green)

---

## Overview

Axiom is a browser-native rigid body physics engine implemented entirely in modern C++ and executed through WebAssembly. The project explores the architecture and implementation of real-time physics engines by constructing their core mathematical and physical systems from first principles rather than integrating existing engine libraries.

Unlike projects that rely on third-party mathematics or physics frameworks, every core simulation component in Axiom is implemented from scratch. This includes vector mathematics, physics primitives, numerical integration, collision detection, collision response, and simulation management.

Three.js is used exclusively for visualization, while Emscripten compiles the native C++ engine into WebAssembly, enabling high-performance physics simulation directly inside the browser.

---

## Objectives

The primary goals of Axiom are to:

- Design a modular rigid body physics engine
- Implement fundamental mathematical and physical systems from first principles
- Execute native C++ simulation in the browser using WebAssembly
- Maintain a clear separation between the simulation engine and the rendering layer
- Explore modern engine architecture through original implementations rather than external physics libraries

---

## Features

### Mathematics

- Custom Vector3 implementation
- Vector arithmetic using operator overloading
- Dot product
- Cross product
- Magnitude and normalization
- Distance calculations

### Physics Engine

- Particle simulation
- Rigid body simulation
- Physics world management
- Fixed timestep simulation
- Euler integration
- Gravity simulation
- Velocity integration

### Collision System

- Sphere–sphere collision detection
- Contact generation
- Impulse-based collision resolution
- Position correction
- Floor collision handling
- World boundary constraints

### Browser Integration

- Native C++ compiled to WebAssembly
- JavaScript bindings using Embind
- Real-time synchronization between the simulation and rendering pipelines
- Interactive visualization using Three.js

---

## Architecture

```text
                        Browser

                   Three.js Renderer
                           │
                    JavaScript Layer
                           │
                     Embind Bindings
                           │
                   WebAssembly Module
                           │
        ┌─────────────────────────────────┐
        │             Axiom               │
        │                                 │
        │  Vector Mathematics             │
        │  Physics World                  │
        │  Particle System                │
        │  Rigid Body System              │
        │  Numerical Integration          │
        │  Collision Detection            │
        │  Collision Resolution           │
        └─────────────────────────────────┘
                           │
                        Modern C++
```

---

## Project Structure

```text
Axiom/
│
├── engine/
│   ├── include/
│   ├── src/
│   └── ...
│
├── web/
│   ├── index.html
│   ├── main.js
│   └── ...
│
├── CMakeLists.txt
├── package.json
├── README.md
└── LICENSE
```

---

## Technology Stack

| Category | Technology |
|----------|------------|
| Language | C++17 |
| Rendering | Three.js |
| Browser Runtime | WebAssembly |
| Toolchain | Emscripten |
| Build System | CMake |
| JavaScript Bindings | Embind |

---

## Design Philosophy

Axiom follows a first-principles approach to engine development.

Every subsystem responsible for physics simulation was implemented specifically for this project instead of relying on existing mathematics or physics libraries.

Custom implementations include:

- Vector mathematics
- Physics primitives
- Simulation pipeline
- Numerical integration
- Collision detection
- Collision response
- Physics world management

External dependencies are intentionally limited to browser interoperability and visualization.

| Component | Implementation |
|----------|----------------|
| Mathematics | Custom |
| Physics Engine | Custom |
| Collision Detection | Custom |
| Collision Resolution | Custom |
| Rendering | Three.js |
| Browser Compilation | Emscripten |

---

## Building

### Clone the repository

```bash
git clone https://github.com/<your-username>/Axiom.git
cd Axiom
```

### Configure

```bash
mkdir build
cd build

emcmake cmake ..
```

### Build

```bash
emmake make
```

### Run

```bash
cd ../web
npm install
npm run dev
```

Open the local development server in your browser.

---

## Future Development

The current implementation establishes the foundation for a complete real-time physics engine.

Future work includes:

- Broad-phase collision detection
- Spatial partitioning
- Additional collider primitives
- Rotational dynamics
- Quaternion mathematics
- Constraint solver
- Joint system
- Friction and restitution improvements
- Continuous collision detection
- Performance optimization
- Multithreaded simulation

---

## License

This project is licensed under the MIT License.

See the [LICENSE](LICENSE) file for details.
