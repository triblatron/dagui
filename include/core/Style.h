//
// Created by Tony Horrobin on 08/06/2025.
//

#include "config/Export.h"

#include "core/Variant.h"

#include <string_view>

namespace dagbase
{
    class ConfigurationElement;
}

namespace dagui
{
    class DAGUI_API Style
    {
    public:
        void configure(dagbase::ConfigurationElement& config);

        dagbase::Variant find(std::string_view path) const;
    private:
        float _border{0.0f};
    };
}