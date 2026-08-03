# Diffusion Garden

Diffusion Garden is a modern C++ implementation of the Gray–Scott reaction-diffusion model using SFML for visualization.

The project explores how simple local chemical interactions can produce complex emergent patterns.

## How It Works

The simulation models two chemicals, **A** and **B**, distributed across a two-dimensional grid.

Each cell stores its own concentration of both chemicals. During every timestep, the simulation calculates how those concentrations change through four processes:

- **Diffusion:** A and B spread between neighboring cells.
- **Reaction:** A is converted into B where both chemicals are present.
- **Feed:** Chemical A is gradually replenished.
- **Kill:** Chemical B is gradually removed.

The reaction can be thought of as:

**A + 2B → 3B**

This means that wherever A and B coexist, B catalyzes the conversion of A into more B. The strength of this reaction is proportional to:

**A × B²**

As the concentration of B increases, the reaction accelerates, creating a positive feedback loop.

Diffusion tends to smooth concentration differences, while reaction, feed, and kill continually create and sustain new differences. The competition between these processes produces emergent spots, branches, ridges, and other patterns.

Even small changes to the diffusion, feed, and kill rates can produce dramatically different forms and behaviors. Most parameter combinations produce little or no visible pattern, requiring patience and a spirit of experimentation. This makes the discovery of a complex structure all the more rewarding.

## Goals

- Explore emergent behavior through reaction-diffusion
- Discover how parameter changes influence pattern formation
- Catalog interesting reaction-diffusion patterns
- Build a foundation for future artificial life and ecosystem simulations


## Current Features

- Gray–Scott reaction-diffusion simulation
- Real-time visualization with SFML
- Texture-based renderer for efficient drawing
- Adjustable model parameters (diffusion, feed, and kill rates)
- Reset simulation
- Pause simulation
- Experiment log documenting discovered patterns

## Experiments

Interested in the patterns discovered so far?

See the [Experiment Log](experiment-log.md) for screenshots, parameter sets, observations, and notes from exploring the Gray–Scott model.
    

## Technologies

- C++17
- SFML 3.1
- CMake

## Controls

- `R` — Reset the simulation
- `Space` — Pause or resume the simulation

## Build

```bash
cmake -B build
cmake --build build
```

Run the executable:

```bash
./build/bin/diffusion-garden
```
