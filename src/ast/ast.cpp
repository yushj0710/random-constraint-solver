#include "ast/ast.hpp"
#include <sstream>

namespace nimo {

// Ast class implementations
std::string ast::toString() const {
    return "";
}

bool ast::isVariable() const {
    return false;
}

bool ast::isLiteral() const {
    return false;
}

bool ast::isBinaryExpression() const {
    return false;
}

bool ast::isUnaryExpression() const {
    return false;
}

bool ast::isFunctionCall() const {
    return false;
}

// BinaryExpressionNode class implementations
std::string BinaryExpressionNode::toString() const {
    return "(" + left_->toString() + " op " + right_->toString() + ")";
}

bool BinaryExpressionNode::isBinaryExpression() const {
    return true;
}

// LiteralNode class implementations
std::string LiteralNode::toString() const {
    return std::to_string(value_);
}

bool LiteralNode::isLiteral() const {
    return true;
}

// UnaryExpressionNode class implementations
std::string UnaryExpressionNode::toString() const {
    return "(op " + operand_->toString() + ")";
}

bool UnaryExpressionNode::isUnaryExpression() const {
    return true;
}

// VariableNode class implementations
std::string VariableNode::toString() const {
    return name_;
}

bool VariableNode::isVariable() const {
    return true;
}

// FunctionCallNode class implementations
std::string FunctionCallNode::toString() const {
    std::string result = name_ + "(";
    for (size_t i = 0; i < arguments_.size(); ++i) {
        result += arguments_[i]->toString();
        if (i < arguments_.size() - 1) {
            result += ", ";
        }
    }
    result += ")";
    return result;
}

bool FunctionCallNode::isFunctionCall() const {
    return true;
}

} // namespace nimo
