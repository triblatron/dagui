//
// Created by Tony Horrobin on 18/04/2025.
//

#pragma once

#include "core/DynamicVisitor.h"
#include "core/Widget.h"

namespace dagui
{
    class VisualElement;

    class CreateVisualTreeVisitor : public dagbase::DynamicVisitor<Widget>
    {
    public:
        CreateVisualTreeVisitor();

        VisualElement* tree()
        {
            return _tree;
        }
    private:
        VisualElement* _tree{nullptr};
    };
}
