//
// Created by Tony Horrobin on 18/05/2025.
//

#include "config/config.h"

#include "core/ResolveWidgetRefsVisitor.h"

namespace dagui
{

    ResolveWidgetRefsVisitor::ResolveWidgetRefsVisitor()
    {
        auto h = [this](Widget &widget) {
            widget.eachConstraint([this, &widget](Constraint &constraint) {
                constraint.resolveRefs(widget);
                return true;
            });
        };
        registerHandler(dagbase::Atom::intern("Label"), h);
        registerHandler(dagbase::Atom::intern("Window"), h);
    }

    void ResolveWidgetRefsVisitor::visitChildren(Widget &widget)
    {
        widget.eachChild([this](Widget& child) {
            child.accept(*this);
            return true;
        });
    }
}