//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"

#include <string>

namespace dagui
{
    class VisualElementVisitor;

    class DAGUI_API VisualElement
    {
    public:
        VisualElement(std::string className);

        std::string className() const
        {
            return _className;
        }

        virtual ~VisualElement() = default;

        virtual void accept(VisualElementVisitor& visitor) = 0;
    private:
        std::string _className;
    };

    inline VisualElement::VisualElement(std::string className)
        : _className(className)
    {
        // Do nothing.
    }
}
