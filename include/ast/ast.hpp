/**
 * @file ast.hpp
 * @author Shengjiao Yu (yushj0710@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-02-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>

namespace nimo
{
    class Ast
    {
    protected:
        uint32_t _id;    // Unique identifier for each node
        uint32_t _depth; // Depth of the node in the AST
        uint32_t _hash;  // Hash value of the node
    };
} // namespace nimo
