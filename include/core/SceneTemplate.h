//
// Created by Tony Horrobin on 29/04/2025.
//

#include "config/Export.h"

#include "util/SearchableArray.h"
#include "util/SearchableMap.h"
#include "util/VectorMap.h"
#include "core/Variant.h"
#include "core/PropertyBinding.h"

#include <string>
#include <vector>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class SceneNode;
    class SceneNodeFactory;
    class Widget;

    class DAGUI_API SceneTemplate
    {
    public:
        SceneTemplate() = default;

        ~SceneTemplate();

        void configure(dagbase::ConfigurationElement& config);

        void addChild(SceneTemplate* child)
        {
            if (child)
                _children.a.emplace_back(child);
        }

        SceneNode* instantiate(SceneNodeFactory& factory, Widget& widget);
    private:
        std::string _sceneClass;
        using PropertyMap = dagbase::SearchableMap<dagbase::VectorMap<std::string, dagbase::Variant>>;
        PropertyMap _props;
        using ChildArray = dagbase::SearchableArray<std::vector<SceneTemplate*>>;
        ChildArray _children;
        using SnippetMap = dagbase::SearchableMap<dagbase::VectorMap<std::string, dagbase::PropertyBinding*>>;
        SnippetMap _snippets;
    };
}
