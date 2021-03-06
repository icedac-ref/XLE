// Copyright 2015 XLGAMES Inc.
//
// Distributed under the MIT License (See
// accompanying file "LICENSE" or the website
// http://www.opensource.org/licenses/mit-license.php)

#include "ShaderResource.h"
#include "../../RenderUtils.h"
#include <GLES2/gl2.h>

namespace RenderCore { namespace Metal_OpenGLES
{
    ShaderResourceView::ShaderResourceView(OpenGL::Texture* underlyingTexture)
    {
        if (glIsTexture((GLuint)underlyingTexture)) {
            _underlyingTexture = underlyingTexture;
        } else {
            Throw(Exceptions::GenericFailure("Binding non-texture to resource"));
        }
    }

    ShaderResourceView::ShaderResourceView(OpenGL::Resource* underlyingTexture)
    {
        if (glIsTexture((GLuint)underlyingTexture)) {
            _underlyingTexture = underlyingTexture->As<GlObject_Type::Texture>();
        } else {
            Throw(Exceptions::GenericFailure("Binding non-texture to resource"));
        }
    }

}}
