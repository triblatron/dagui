//
// Created by Tony Horrobin on 29/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/ConfigurationElement.h"
#include "core/SceneTemplate.h"

#include <string_view>

namespace dagui
{
    class SceneNode;
    class Widget;

    class DAGUI_API WidgetPattern
    {
    public:
        WidgetPattern() = default;

        void configure(dagbase::ConfigurationElement& config);

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const;

        SceneNode* match(Widget& widget);
    private:
        std::string _matchClass;
        SceneTemplate _sceneTemplate;
    };
}
