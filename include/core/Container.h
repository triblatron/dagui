//
// Created by tony on 30/07/24.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"

#include <vector>
#include <functional>

namespace dagui
{
    class DAGUI_API Container : public Widget
    {
    public:
        explicit Container(Widget* parent=nullptr);

        ~Container() override;

        void addChild(Widget* child)
        {
            if (child != nullptr)
            {
                _children.emplace_back(child);
            }
        }

        void eachChild(std::function<void (Widget*)> f)
        {
            for (auto it=_children.begin(); it!=_children.end(); ++it)
            {
                std::invoke(f, *it);
            }
        }
    private:
        using Children = std::vector<Widget*>;
        Children _children;
    };
}
