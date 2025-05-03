//
// Created by Tony Horrobin on 03/05/2025.
//

#pragma once

#include "config/Export.h"

#include "core/PropertyBinding.h"

namespace dagui
{
    class Widget;

    class DAGUI_API WidgetPropertyBinding : public dagbase::PropertyBinding
    {
    public:
        WidgetPropertyBinding() = default;

        void setWidget(Widget* widget)
        {
            _widget = widget;
        }
    private:
        Widget* _widget{nullptr};
    };
}
