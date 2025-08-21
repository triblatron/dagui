//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/SceneNodeFactory.h"
#include "core/Group.h"
#include "core/Border.h"
#include "core/Text.h"
#include "core/ClipGroup.h"

#include <algorithm>

namespace dagui
{

    SceneNode *SceneNodeFactory::createNode(dagbase::Atom className, Widget *widget)
    {
        dagbase::IdentifierGenerator::Identifier id = _idGen.generate();
        SceneNode* node = nullptr;
        if (className == "Group")
        {
            node = new Group(id, widget);
        }
        else if (className == "Border")
        {
            node = new Border(id, widget);
        }
        else if (className == "ClipGroup")
        {
            node = new ClipGroup(id, widget);
        }

        if (node)
        {
            _nodes.resize(std::max(std::size_t(id+1),_nodes.size()),nullptr);
            if (id<_nodes.size())
            {
                _nodes[id] = node;
            }
        }

        return node;
    }

    void SceneNodeFactory::deleteNode(dagbase::IdentifierGenerator::Identifier id)
    {
        if (id<_nodes.size())
        {
            delete _nodes[id];
            _idGen.release(id);
            _nodes[id] = nullptr;
        }
    }
}