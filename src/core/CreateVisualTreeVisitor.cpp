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

namespace dagui
{
    CreateVisualTreeVisitor::CreateVisualTreeVisitor()
    {
        registerHandler(dagbase::Atom::intern("RootWidget"), [this](Widget& widget) {
           Group* parent = new Group();

           _tree = parent;
        });
        registerHandler(dagbase::Atom::intern("Label"), [this](Widget& widget) {
            Label& label = static_cast<Label&>(widget);
            Group* parent = new Group();

            _tree = parent;
        });
    }
}