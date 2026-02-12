# Awareness Spread Simulation

A C++ simulation using **SFML** that models how awareness spreads through a population and evolves into action, based on individual thresholds, influence, and passion. The simulation is visualized on a grid where nodes change states from *ignorant* → *aware* → *active*.

---

## Features

- **Node-based modeling**: Each individual is represented as a node with attributes such as influence, passion, and energy.
- **Multiple threshold patterns**: Nodes can become active based on:
  - Proximity to other active nodes
  - Total number of active nodes
  - Amount of exposure
  - Influence-based thresholds
  - Passion-based thresholds
- **Dynamic interactions**: Nodes interact with neighbors or the environment and evolve their awareness over time.
- **Visual simulation**: Grid-based visualization using SFML:
  - **Cyan** → Ignorant  
  - **Blue** → Aware  
  - **Green** → Active
- **Configurable parameters**: Easily tweak the initial setup, node properties, and simulation ticks for experimentation.

---

## Requirements

- **C++17** or later  
- **SFML 2.5+** (Simple and Fast Multimedia Library)  

Install SFML for your platform:

- **Windows (vcpkg)**:  
  ```bash
  vcpkg install sfml

- **Linux (apt)**:
  ```bash
  sudo apt install libsfml-dev

- **macOS (brew)**:
  ```bash
  brew install sfml

---

## Customization

You can experiment with:
- Node properties (in struct.h):
  - activityIntent, influence, passion, thresholdPattern
- Grid setup (in Map.cpp):
  - Initial active nodes
  - Grid size (ROWS x COLS)
- Simulation parameters:
  - Number of ticks (myMap.initialize(150);)
  - Energy, decay rates, and spread amounts

---

##  How It Works

- Initialization: The map is created with all nodes in the ignorant state except for initial active nodes.
- Tick cycle:
  - Active nodes attempt to influence surrounding nodes based on range and spread probability.
  - Nodes update awareness levels, decay rates, and determine state transitions.
- Visualization: Each node is drawn on the grid with color representing its state.

---

##  Notes

- This simulation is meant for experimentation. Many values are examples and can be tuned for different scenarios.
- The focus is on interaction patterns and awareness evolution, rather than real-world accuracy.

---

## License

MIT License. Feel free to modify and experiment with the code.
