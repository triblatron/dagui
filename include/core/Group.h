//
// Created by Tony Horrobin on 18/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/VisualElement.h"
#include "core/VisualElementVisitor.h"
#include "util/SearchableArray.h"

#include <vector>

namespace dagui
{
    class DAGUI_API Group : public VisualElement
    {
    public:
        Group();

        void accept(VisualElementVisitor& visitor) override
        {
            visitor.visit(*this);
            for (auto child : _children.a)
            {
                child->accept(visitor);
            }
        }

        void addChild(VisualElement* child)
        {
            if (child)
            {
                _children.a.push_back(child);
            }
        }

        dagbase::ConfigurationElement::ValueType find(std::string_view path) const override;
    private:
        using ChildArray = dagbase::SearchableArray<std::vector<VisualElement*>>;
        ChildArray _children;
    };
}
