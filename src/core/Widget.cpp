//
// Created by tony on 30/07/24.
//

#include "config/config.h"
#include "core/Widget.h"


namespace nfe
{
    Widget::Widget(Widget *parent)
    :
    _parent(parent)
    {
        // Do nothing.
    }
}