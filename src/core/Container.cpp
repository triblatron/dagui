//
// Created by tony on 30/07/24.
//

#include "config/config.h"

#include "core/Container.h"

namespace nfe
{

    Container::Container(Widget *parent) : Widget(parent)
    {

    }

    Container::~Container()
    {
        eachChild([](Widget* child) {
           delete child;
        });
    }
}