//
// Created by tony on 30/09/24.
//

#pragma once

#include "config//Export.h"

namespace nfe
{
    class ImageSource;
}

namespace nfe
{
    class ImageDef;

    class NFE_API PackingStrategy
    {
    public:
        enum Error
        {
            //! The packing operation completed successfully.
            ERR_OK,
            //! The packing operation failed to pack an input image
            ERR_FAILED_TO_PACK,
            //! The packing operation is not complete.
            ERR_UNKNOWN
        };
    public:
        virtual ~PackingStrategy() = default;

        virtual void setBinRectangle(ImageDef* bin) = 0;
        virtual void setInputSource(ImageSource* source) = 0;
        virtual void makeItSo() = 0;
        virtual Error error() const = 0;

        static const char* errorToString(Error err);

        static Error parseError(const char* str);
    };
}
