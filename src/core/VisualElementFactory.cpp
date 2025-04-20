//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/VisualElementFactory.h"
#include "core/Group.h"
#include "core/Border.h"

namespace dagui
{

    VisualElement *VisualElementFactory::createElement(dagbase::Atom className, Widget *widget)
    {
        if (className == "Group")
        {
            return new Group(widget);
        }
        else if (className == "Border")
        {
            return new Border(widget);
        }
        return nullptr;
    }
}