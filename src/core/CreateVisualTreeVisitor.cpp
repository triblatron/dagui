//
// Created by Tony Horrobin on 18/04/2025.
//

#include "config/config.h"

#include "core/CreateVisualTreeVisitor.h"
#include "core/Atom.h"
#include "core/VisualElement.h"
#include "core/Label.h"
#include "core/Border.h"
#include "core/Group.h"
#include "core/RootWidget.h"

namespace dagui
{
    CreateVisualTreeVisitor::CreateVisualTreeVisitor()
    {
        _builder.setFactory(&_factory);
        registerHandler(dagbase::Atom::intern("RootWidget"), [this](Widget& widget) {
            _builder.beginElement(dagbase::Atom::intern("Group"), &widget);
            RootWidget& root = static_cast<RootWidget&>(widget);

            root.eachChild([this](Widget& child) {
                child.accept(*this);

                return true;
            });

            _builder.endElement();

            _tree = _builder.build();
        });
        auto fparent = [this](Widget& widget) {
            _builder.beginElement(dagbase::Atom::intern("Group"), &widget);

            widget.eachChild([this](Widget& child) {
                child.accept(*this);

                return true;
            });
            _builder.endElement();
        };

        registerHandler(dagbase::Atom::intern("Window"), fparent);
        registerHandler(dagbase::Atom::intern("Vertical"), fparent);

        auto fchild = [this](Widget& widget) {
            Label& label = static_cast<Label&>(widget);

            _builder.beginElement(dagbase::Atom::intern("Group"), &widget);
            _builder.beginElement(dagbase::Atom::intern("Border"), &widget);
            _builder.endElement();
            _builder.endElement();
        };

        registerHandler(dagbase::Atom::intern("Label"), fchild);
        registerHandler(dagbase::Atom::intern("Button"), fchild);
    }
}