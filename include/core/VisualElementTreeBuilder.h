//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/Export.h"

#include "core/Atom.h"

#include <stack>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class Group;
    class VisualElement;
    class VisualElementFactory;
    class Widget;

    class DAGUI_API VisualElementTreeBuilder
    {
    public:
        VisualElementTreeBuilder() = default;

        void setFactory(VisualElementFactory* factory)
        {
            _factory = factory;
        }

        void beginElement(dagbase::Atom className, Widget *widget);

        void endElement();

        VisualElement* build()
        {
            return _currentElement;
        }
    private:
        VisualElementFactory* _factory{nullptr};
        using ParentStack = std::stack<VisualElement*>;
        ParentStack _parentStack;
        VisualElement* _currentElement{nullptr};
        VisualElement* createElement(dagbase::Atom className, Widget* widget);
    };
}