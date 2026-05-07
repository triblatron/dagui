//
// Created by Tony Horrobin on 07/05/2026.
//

#include "config/config.h"

#include "core/EditorParameters.h"
#include "core/ConfigurationElement.h"

namespace dagui
{
    void EditorParameters::configure(dagbase::ConfigurationElement &config)
    {
        dagbase::ConfigurationElement::readConfig(config, "typeName", &typeName);
        dagbase::ConfigurationElement::readConfig(config, "className", &className);
        dagbase::ConfigurationElement::readConfig(config, "isSigned", &isSigned);
        if (isSigned)
        {
            dagbase::ConfigurationElement::readConfig(config, "minValue", &minSigned);
            dagbase::ConfigurationElement::readConfig(config, "maxValue", &maxSigned);
        }
        else
        {
            dagbase::ConfigurationElement::readConfig(config, "minValue", &minUnsigned);
            dagbase::ConfigurationElement::readConfig(config, "maxValue", &maxUnsigned);
        }
    }
}