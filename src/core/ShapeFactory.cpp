//
// Created by Tony Horrobin on 05/07/2025.
//

#include "config/config.h"

#include "core/ShapeFactory.h"
#include "core/ConfigurationElement.h"
#include "core/Rectangle.h"
#include "core/Text.h"

namespace dagui
{

    Shape *ShapeFactory::createShape(dagbase::ConfigurationElement &config)
    {
        Shape* retval = nullptr;
        std::string className;

        if (auto element = config.findElement("class"); element)
        {
            className = element->asString();
        }

        if (className == "Rectangle")
        {
            retval = new Rectangle();
        }
        else if (className == "Text")
        {
            retval = new Text();
        }

        if (retval)
        {
            retval->configure(config, *this);
        }

        return retval;
    }
}