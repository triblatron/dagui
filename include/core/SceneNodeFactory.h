//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/Export.h"

#include "core/Atom.h"
#include "Widget.h"
#include "core/IdentifierGenerator.h"

namespace dagui
{
    class SceneNode;

    class DAGUI_API SceneNodeFactory
    {
    public:
        SceneNode* createNode(dagbase::Atom className, Widget* widget);

        SceneNode* nodeByID(dagbase::IdentifierGenerator::Identifier id)
        {
            if (id<_nodes.size())
                return _nodes[id];

            return nullptr;
        }

        void deleteNode(dagbase::IdentifierGenerator::Identifier id);
    private:
        dagbase::IdentifierGenerator _idGen;
        using NodeArray = std::vector<SceneNode*>;
        NodeArray _nodes;
    };
}
