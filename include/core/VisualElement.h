//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"
#include "core/VisualElementVisitor.h"
#include "core/Atom.h"
#include "core/ConfigurationElement.h"

#include <string>

namespace dagui
{
    class DAGUI_API VisualElement
    {
    public:
        VisualElement(std::string className);

        virtual ~VisualElement() = default;

        dagbase::Atom typeName() const
        {
            return _className;
        }

        virtual dagbase::ConfigurationElement::ValueType find(std::string_view path) const;

        virtual void accept(VisualElementVisitor& visitor) = 0;
    protected:

    private:
        dagbase::Atom _className;
    };
}
