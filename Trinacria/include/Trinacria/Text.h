#pragma once

#include <memory>
#include "Macros.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"

namespace TRCN_CORE_NAMESPACE
{
    class Atlas
    {
    public:
        int Width, Height;

        /**
         *
         * @param fontPath the path of the font
         * @param charSet the charset for example Charset::ASCII
         */
        void LoadAtlas(const char* fontPath, msdf_atlas::Charset charSet);

    private:
        std::unique_ptr<msdf_atlas::FontGeometry> _fontGeometry;
    };
}