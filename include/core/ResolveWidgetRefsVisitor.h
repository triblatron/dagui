//
// Created by Tony Horrobin on 18/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"

namespace dagui
{
    class ResolveWidgetRefsVisitor : public dagbase::DynamicVisitor<Widget>
    {
    public:
        ResolveWidgetRefsVisitor();
    private:
        void visitChildren(Widget& widget);
    };
}