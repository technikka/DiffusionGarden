# Diffusion Garden

Diffusion Garden is a modern C++ implementation of the Gray–Scott reaction-diffusion model using SFML for visualization.

The project explores how simple local chemical interactions can produce complex emergent patterns.

## Current Status

Initial project setup is complete.

- CMake build configuration
- SFML 3.1 integration
- Basic graphical window and rendering loop
    

## Goals

- Implement the Gray–Scott reaction-diffusion model 
- Visualize chemical concentrations in real time
- Explore how parameter changes affect the patterns produced by the simulation.
    

## Technologies

- C++17
- SFML 3.1
- CMake
    

## Build

```bash
cmake -B build
cmake --build build
```

Run the executable:

```bash
./build/bin/diffusion-garden
