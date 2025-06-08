//
// Created by Tony Horrobin on 18/05/2025.
//

#pragma once

#include "core/Reference.h"
#include "core/Atom.h"

namespace dagui
{
    class Widget;
    using WidgetRef = typename dagbase::Reference<dagbase::Atom, Widget, Widget>;
}