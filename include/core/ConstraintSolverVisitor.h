//
// Created by Tony Horrobin on 31/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/DynamicVisitor.h"

namespace dagui
{
    class Widget;

    class ConstraintSolverVisitor : public dagbase::DynamicVisitor<Widget>
    {
    public:
        ConstraintSolverVisitor();
    };
}