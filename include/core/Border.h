//
// Created by Tony Horrobin on 14/04/2025.
//

#pragma once

#include "config/Export.h"

#include "core/VisualElement.h"

namespace dagui
{
    class DAGUI_API Border : public VisualElement
    {
    public:
        Border();

        void accept(VisualElementVisitor& visitor) override;
    };

}
