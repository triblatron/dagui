//
// Created by tony on 30/07/24.
//

#include "config/config.h"

#include "core/Container.h"

namespace dagui
{

    Container::Container(Widget *parent) : Widget(parent)
    {
        // Do nothing.
    }

    Container::~Container()
    {
        eachChild([](Widget* child) {
           delete child;
        });
    }
}