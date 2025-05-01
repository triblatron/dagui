//
// Created by Tony Horrobin on 01/05/2025.
//

#include "config/config.h"

#include "core/WidgetSnippet.h"
#include "core/Widget.h"

namespace dagui
{
    std::string WidgetSnippet::resolve(const std::string &name)
    {
        if (_widget)
            return _widget->find(name).toString();
        else
            return {};
    }

    WidgetSnippet::WidgetSnippet()
    {
        setOpen(dagbase::Atom::intern("{{"));
        setClose(dagbase::Atom::intern("}}"));
    }
}
