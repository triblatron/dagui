//
// Created by Tony Horrobin on 25/01/2025.
//

#include "config/config.h"

#include "gfx/Mesh2D.h"
#include "util/enums.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    void Mesh2D::configure(dagbase::ConfigurationElement& config)
    {
        if (auto element = config.findElement("primitiveType"); element)
        {
            _primitiveType = parsePrimitiveType(element->asString().c_str());
        }

        if (auto element = config.findElement("vertices"); element)
        {
            _vertices.reserve(element->numChildren());
            for (unsigned i = 0; i < element->numChildren(); i++)
            {
                Vec2f vertex;

                vertex.x = static_cast<float>(element->child(i)->asDouble());
                vertex.y = static_cast<float>(element->child(i)->asDouble());

                _vertices.push_back(vertex);
            }
        }
    }

    const char* Mesh2D::primitiveTypeToString(PrimitiveType value)
    {
        switch (value)
        {
            ENUM_NAME(PRIMITIVE_UNKNOWN)
            ENUM_NAME(PRIMITIVE_POINT)
            ENUM_NAME(PRIMITIVE_LINE)
            ENUM_NAME(PRIMITIVE_LINE_STRIP)
            ENUM_NAME(PRIMITIVE_LINE_LOOP)
            ENUM_NAME(PRIMITIVE_TRIANGLE)
            ENUM_NAME(PRIMITIVE_TRIANGLE_LIST)
            ENUM_NAME(PRIMITIVE_TRIANGLE_STRIP)
            ENUM_NAME(PRIMITIVE_TRIANGLE_FAN)
        }

        return "<error>";
    }

    Mesh2D::PrimitiveType Mesh2D::parsePrimitiveType(const char* str)
    {
        TEST_ENUM(PRIMITIVE_POINT, str);
        TEST_ENUM(PRIMITIVE_LINE, str);
        TEST_ENUM(PRIMITIVE_LINE_STRIP, str);
        TEST_ENUM(PRIMITIVE_LINE_LOOP, str);
        TEST_ENUM(PRIMITIVE_TRIANGLE, str);
        TEST_ENUM(PRIMITIVE_TRIANGLE_LIST, str);
        TEST_ENUM(PRIMITIVE_TRIANGLE_STRIP, str);
        TEST_ENUM(PRIMITIVE_TRIANGLE_FAN, str);

        return PRIMITIVE_UNKNOWN;
    }

}