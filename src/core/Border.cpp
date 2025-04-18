//
// Created by Tony Horrobin on 14/04/2025.
//

#include "config/config.h"

#include "core/Border.h"
#include "core/VisualElementVisitor.h"

namespace dagui
{
    Border::Border()
        :
    VisualElement("Border")
    {
    }

    void Border::accept(VisualElementVisitor& visitor)
    {
        visitor.visit(*this);
    }
}