# Random Constraint Solver for SystemVerilog

## Overview
The Random Constraint Solver is a C++ application designed to parse and solve SystemVerilog constraints. It generates random solutions based on the specified constraints, making it useful for verification and testing purposes.

## Project Structure
```
random-constraint-solver
├── src
│   ├── main.cpp          # Entry point of the application
│   ├── solver.cpp        # Implementation of the ConstraintSolver class
│   ├── solver.hpp        # Declaration of the ConstraintSolver class
│   └── utils
│       └── utils.hpp     # Utility functions for the solver
├── include
│   └── solver.hpp        # Public interface for the ConstraintSolver class
├── CMakeLists.txt        # CMake configuration file
└── README.md             # Project documentation
```

## Setup Instructions
1. **Clone the repository:**
   ```
   git clone <repository-url>
   cd random-constraint-solver
   ```

2. **Build the project using CMake:**
   ```
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run the executable:**
   ```
   ./random-constraint-solver <input_file.sv>
   ```

## Usage
To use the Random Constraint Solver, provide a SystemVerilog file containing constraints as a command-line argument. The solver will parse the constraints and generate a random solution.

## Supported SystemVerilog Constraints
The solver currently supports a subset of SystemVerilog constraints, including:
- Simple expressions
- Logical operators
- Range constraints

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any enhancements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for details.