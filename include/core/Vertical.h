//
// Created by Tony Horrobin on 17/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"
#include "core/LayoutProperties.h"

namespace dagui
{
    class DAGUI_API Vertical : public Widget
    {
    public:
        explicit Vertical(Widget * parent);

        ~Vertical() override = default;

        LayoutProperties& props()
        {
            return _props;
        }

        const LayoutProperties& props() const
        {
            return _props;
        }

        void computeConstraints();

        void configure(dagbase::ConfigurationElement& config, WidgetFactory& factory) override;

        dagbase::Variant find(std::string_view path) const override;
    private:
        LayoutProperties _props;
        struct ChildProperties
        {
            Widget* widget{nullptr};
            float growthFactor{0.0f};
        };
        using GrowthFactorArray = std::vector<ChildProperties>;
        ChildProperties _childProps;
        float _minWidth{0.0};
    };
}
