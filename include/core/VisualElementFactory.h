//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/Export.h"

#include "core/Atom.h"

namespace dagui
{
    class VisualElement;

    class DAGUI_API VisualElementFactory
    {
    public:
        VisualElement* createElement(dagbase::Atom className);
    };
}