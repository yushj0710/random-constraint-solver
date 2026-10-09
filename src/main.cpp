#include <iostream>
#include <string>
#include "solver.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_systemverilog_file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    ConstraintSolver solver;

    if (!solver.parseConstraints(inputFile)) {
        std::cerr << "Error parsing constraints from file: " << inputFile << std::endl;
        return 1;
    }

    auto solution = solver.generateRandomSolution();
    if (!solver.validateSolution(solution)) {
        std::cerr << "Generated solution is invalid." << std::endl;
        return 1;
    }

    std::cout << "Generated solution: " << solution << std::endl;
    return 0;
}