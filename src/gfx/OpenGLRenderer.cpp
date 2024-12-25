//
// Created by Tony Horrobin on 25/12/2024.
//

#include "config/config.h"

#include "gfx/OpenGLRenderer.h"
#include "core/Vec2f.h"
#include "gfx/BinImageDef.h"
#include "gfx/ImageDef.h"

namespace dagui
{
    void OpenGLRenderer::generateTextureCoordinates(ImageDef& imageDef, BinImageDef& binImageDef)
    {
        imageDef.p0.x = float(imageDef.x)/float(binImageDef.width);
        imageDef.p0.y = float(imageDef.y)/float(binImageDef.height);
        imageDef.p1.x = float(imageDef.x+imageDef.width)/float(binImageDef.width);
        imageDef.p1.y = float(imageDef.y)/float(binImageDef.height);
        imageDef.p2.x = float(imageDef.x+imageDef.width)/float(binImageDef.width);
        imageDef.p2.y = float(imageDef.y+imageDef.height)/float(binImageDef.height);
        imageDef.p3.x = float(imageDef.x)/float(binImageDef.width);
        imageDef.p3.y = float(imageDef.y+imageDef.height)/float(binImageDef.height);
    }
}
