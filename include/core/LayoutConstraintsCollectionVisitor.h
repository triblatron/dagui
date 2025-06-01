//
// Created by Tony Horrobin on 01/06/2025.
//

#pragma once

#include "config/Export.h"

#include "core/DynamicVisitor.h"
#include "core/Variant.h"
#include "util/SearchableArray.h"
#include "core/Constraint.h"
#include <string_view>

namespace dagui
{
    class Widget;

    class DAGUI_API LayoutConstraintsCollectionVisitor : public dagbase::DynamicVisitor<Widget>
    {
    public:
        LayoutConstraintsCollectionVisitor();

        std::size_t numConstraints() const
        {
            return _constraints.a.size();
        }

        dagbase::Variant find(std::string_view path) const;
    private:
        using Constraints = dagbase::SearchableArray<std::vector<Constraint>>;
        Constraints _constraints;
    };
}