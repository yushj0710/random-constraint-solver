#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <string>
#include <vector>
#include <map>

class ConstraintSolver {
public:
    ConstraintSolver();
    void parseConstraints(const std::string& svFile);
    std::map<std::string, int> generateRandomSolution();
    bool validateSolution(const std::map<std::string, int>& solution);

private:
    struct Constraint {
        std::string name;
        std::string expression;
    };

    std::vector<Constraint> constraints;

    void loadConstraintsFromFile(const std::string& svFile);
    void evaluateConstraint(const Constraint& constraint, std::map<std::string, int>& solution);
};

#endif // SOLVER_HPP