//
// Created by Tony Horrobin on 10/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/WidgetPattern.h"
#include "util/VectorMap.h"
#include "core/Atom.h"

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API WidgetToSceneNodeConverter
    {
    public:
        void configure(dagbase::ConfigurationElement& config);

        SceneNode *convert(SceneNodeFactory &factory, Widget &widgetTree);
    private:
        using NameToWidgetPatternMap = dagbase::VectorMap<dagbase::Atom, WidgetPattern*>;
        NameToWidgetPatternMap _widgetPatterns;
    };
}