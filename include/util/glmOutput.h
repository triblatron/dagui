//
// Created by Tony Horrobin on 16/03/2025.
//

#pragma once

#include "config/Export.h"

#include <iosfwd>
#include <glm/fwd.hpp>

extern std::ostream DAGUI_API & operator<<(std::ostream & os, const glm::mat4 & mat);
