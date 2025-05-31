//
// Created by Tony Horrobin on 31/05/2025.
//

#include "config/config.h"

#include "core/ConstraintSolverVisitor.h"
#include "core/Widget.h"
#include "core/Atom.h"

namespace dagui
{

    ConstraintSolverVisitor::ConstraintSolverVisitor()
    {
        registerHandler(dagbase::Atom::intern("Window"), [this](Widget& widget) {
            widget.eachConstraint([this](Constraint& constraint) {
                constraint.makeItSo();
                return true;
            });
        });
    }
}