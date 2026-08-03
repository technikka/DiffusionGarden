# Diffusion Garden

Diffusion Garden is a modern C++ implementation of the Gray–Scott reaction-diffusion model using SFML for visualization.

The project explores how simple local chemical interactions can produce complex emergent patterns.

## Current Features

- Gray–Scott reaction-diffusion simulation
- Real-time visualization with SFML
- Texture-based renderer for efficient drawing
- Adjustable model parameters (diffusion, feed, and kill rates)
- Reset simulation
- Pause simulation
- Collection of saved parameter presets
    

## Goals

- Explore emergent behavior through reaction-diffusion
- Discover how parameter changes influence pattern formation
- Catalog interesting reaction-diffusion patterns
- Build a foundation for future artificial life and ecosystem simulations


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
```

## Controls

- `R` — Reset the simulation
- `Space` — Pause or resume the simulation

