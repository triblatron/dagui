//
// Created by Tony Horrobin on 02/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Group.h"
#include "core/Vec2.h"

namespace dagui
{
    class Shape;

    class DAGUI_API ClipGroup : public Group
    {
    public:
        explicit ClipGroup(Widget* widget);

        void setBounds(const dagbase::Vec2& bounds)
        {
            _bounds = bounds;
        }

        dagbase::Variant find(std::string_view path) const override;
    private:
        Shape* _shape{nullptr};
        dagbase::Vec2 _bounds;
    };
}
