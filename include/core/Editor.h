//
// Created by Tony Horrobin on 02/05/2026.
//

#pragma once

#include "config/Export.h"

#include "core/Variant.h"
#include "core/Atom.h"

#include <string_view>

namespace dagbase
{
    struct Property;
}

namespace dagui
{
    class DAGUI_API Editor
    {
    public:
        //! We have virtual functions, so a virtual dtor helps avoid problems
        virtual ~Editor() = default;

        virtual void setObject(void* obj) = 0;

        virtual void setName(dagbase::Atom name) = 0;

        virtual void setProperty(const dagbase::Property& prop) = 0;

        //! Initialise the property from the object
        //! Edit the value with validation
        //! Set the final value on the object
        virtual void makeItSo() = 0;

        //! Clone this object using its copy ctor
        //! \note Supports the Prototype pattern in EditorRegistry
        virtual Editor* clone() = 0;

        //! Find an attribute
        //! \see dagbase::findEndpoint
        //! \see dagbase::findInternal
        virtual dagbase::Variant find(std::string_view path) const = 0;
    };
}
