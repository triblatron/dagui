//
// Created by Tony Horrobin on 16/03/2025.
//

#include "config/config.h"

#include "util/glmOutput.h"
#include <iostream>
#include <glm/glm.hpp>

std::ostream & operator<<(std::ostream & os, const glm::mat4 & mat)
{
    os << "mat4\n{\n";
    for (int i=0; i<4; i++)
    {
        os << "  {";
        for (int j=0; j<4; j++)
        {
            os << "    " << mat[i][j];
        }
        os << "  }\n";
    }
    os << "}";
    os << std::endl;
    return os;
}