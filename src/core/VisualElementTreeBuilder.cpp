//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/VisualElementTreeBuilder.h"
#include "core/VisualElementFactory.h"
#include "core/Group.h"

namespace dagui
{
    VisualElement *VisualElementTreeBuilder::createElement(dagbase::Atom className)
    {
        if (_factory)
        {
            return _factory->createElement(className);
        }
        return nullptr;
    }

    void VisualElementTreeBuilder::beginElement(dagbase::Atom className)
    {
        if (_currentElement)
        {
            _parentStack.push(_currentElement);
        }
        _currentElement = createElement(className);
        if (!_parentStack.empty())
        {
            if (auto group = dynamic_cast<Group*>(_parentStack.top()); group)
                group->addChild(_currentElement);
        }
    }

    void VisualElementTreeBuilder::endElement()
    {
        if (!_parentStack.empty())
        {
            _currentElement = _parentStack.top();
            _parentStack.pop();
        }
    }
}
