//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"
#include "core/VisualElementVisitor.h"
#include "core/Atom.h"

#include <string>

namespace dagui
{
    class DAGUI_API VisualElement
    {
    public:
        VisualElement(std::string className);

        dagbase::Atom className() const
        {
            return _className;
        }

        virtual ~VisualElement() = default;

        dagbase::Atom typeName() const
        {
            return _className;
        }

        virtual void accept(VisualElementVisitor& visitor) = 0;
    protected:

    private:
        dagbase::Atom _className;
    };

    inline VisualElement::VisualElement(std::string className)
        : _className(dagbase::Atom::intern(className))
    {
        // Do nothing.
    }
}
