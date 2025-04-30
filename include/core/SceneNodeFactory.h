//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/Export.h"

#include "core/Atom.h"
#include "Widget.h"

namespace dagui
{
    class SceneNode;

    class DAGUI_API SceneNodeFactory
    {
    public:
        SceneNode* createNode(dagbase::Atom className, Widget* widget);
    };
}
