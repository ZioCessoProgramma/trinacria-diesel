#pragma once

#include <memory>
#include "Macros.h"

namespace msdf_atlas
{
    class Charset;
    class GlyphGeometry;
    class FontGeometry;
}

namespace TRCN_CORE_NAMESPACE
{
    class TextAtlas
    {
    public:
        int Width, Height;

        TextAtlas() = default;

        /**
        * @param fontPath the path of the font
        * @param charSet the charset for example Charset::ASCII
        */

        TextAtlas(const char* fontPath, const msdf_atlas::Charset& charSet);

        /**
         * @param fontPath the path of the font
         * @param charSet the charset for example Charset::ASCII
         */

        void LoadAtlas(const char* fontPath, const msdf_atlas::Charset& charSet);

        /**
         * @param c the char the of the glyph
         * @return a msdf_atlas::GlyphGeometry representing the glyph
         */

        const msdf_atlas::GlyphGeometry* GetGlyph(char c);

        /**
         * @brief get the underlying openGL texture
         * @return the underlying openGL texture
         */

        uint32_t GetTexture() const { return _atlasTexture; }

        std::string GetName() const { return _name; }

    private:
        std::unique_ptr<msdf_atlas::FontGeometry> _fontGeometry;

        uint32_t _atlasTexture = 0;

        std::string _name;

        bool _alreadyLoaded = false;
    };

    class Glyph
    {
    public:
        /**
         * @param owner the atlas that contains the glyph
         */

        Glyph(TextAtlas* owner) : _owner(owner) { }

        void SetGlyph(char c) ;

        float GetBound0() const { return _bound0; }
        float GetBound1() const { return _bound1; }
        float GetBound2() const { return _bound2; }
        float GetBound3() const { return _bound3; }

    private:
        TextAtlas* _owner;

        float _bound0 = 0, _bound1 = 0, _bound2 = 0, _bound3 = 0;
    };
}