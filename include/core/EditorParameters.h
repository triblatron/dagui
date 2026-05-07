//
// Created by Tony Horrobin on 07/05/2026.
//

#ifndef DAGUI_EDITORPARAMETERS_H
#define DAGUI_EDITORPARAMETERS_H

#include "config/Export.h"

#include "core/Atom.h"

#include <cstdint>

namespace dagui
{
    struct DAGUI_API EditorParameters
    {
        dagbase::Atom typeName;
        dagbase::Atom className;
        std::int64_t minSigned{0};
        std::int64_t maxSigned{0};
        bool isSigned{false};
        std::uint64_t minUnsigned{0};
        std::uint64_t maxUnsigned{0};

        void configure(dagbase::ConfigurationElement& config);
    };
}

#endif //DAGUI_EDITORPARAMETERS_H
