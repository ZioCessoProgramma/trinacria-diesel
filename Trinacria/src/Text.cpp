#include <Trinacria/Text.h>
#include <Trinacria/Macros.h>

#include "Trinacria/Log.h"
#include "glad/glad.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>

Trinacria::DSL::TextAtlas::TextAtlas(const char* fontPath, const msdf_atlas::Charset& charSet)
{
    LoadAtlas(fontPath, charSet);
}

void TRCN_CORE_NAMESPACE::TextAtlas::LoadAtlas(const char* fontPath, const msdf_atlas::Charset& charSet)
{
    if (_alreadyLoaded)
    {
        TRCN_LOG("Can't load a font atlas more than once");
    }

    msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype();

    if (!ft) TRCN_LOG("Failed to initialize freetype library");

    msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontPath);

    if (!font) TRCN_LOG("Failed to load font atlas");

    _alreadyLoaded = true;


    _fontGeometry = std::make_unique<msdf_atlas::FontGeometry>(&_glyphs);
    _fontGeometry->loadCharset(font, 1.0, charSet);

    constexpr float maxCornerAngle = 3.f;

    for (msdf_atlas::GlyphGeometry& glyph : _glyphs)
        glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

    msdf_atlas::TightAtlasPacker packer;

    packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
    packer.setMinimumScale(24.0);
    packer.setPixelRange(2.0);
    packer.setMiterLimit(1.0);
    packer.pack(_glyphs.data(), _glyphs.size());

    int width = 0, height = 0;
    packer.getDimensions(width, height);
    msdf_atlas::ImmediateAtlasGenerator<
        float,
        4,
        msdf_atlas::mtsdfGenerator,
        msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 4>>
        generator(width, height);

    const msdf_atlas::GeneratorAttributes attributes;

    generator.setAttributes(attributes);
    generator.setThreadCount(4);
    generator.generate(_glyphs.data(), _glyphs.size());

    _atlasTexture.GenerateTexture();
    _atlasTexture.Bind();

    const msdfgen::BitmapConstRef<unsigned char, 4> bitmap = generator.atlasStorage();

    _atlasTexture.TexImage(GL_RGBA, GL_RGBA, width, height,
        GL_UNSIGNED_BYTE, (void*) bitmap.pixels, GL_LINEAR);

    Width = width;
    Height = height;

    _name = fontPath;

    msdfgen::destroyFont(font);

    msdfgen::deinitializeFreetype(ft);
}
const msdf_atlas::GlyphGeometry* Trinacria::DSL::TextAtlas::GetGlyph(char c)
{
    return _fontGeometry->getGlyph(c);
}

void Trinacria::DSL::Glyph::SetGlyph(char c)
{
    const msdf_atlas::GlyphGeometry* glyph = _owner->GetGlyph(c);

    double bound0, bound1, bound2, bound3;
    glyph->getQuadAtlasBounds(bound0, bound1, bound2, bound3);

    _bound0 = (float)bound0 / _owner->GetTexture().GetWidth();
    _bound1 = (float)bound1 / _owner->GetTexture().GetHeight();
    _bound2 = (float)bound2 / _owner->GetTexture().GetWidth();
    _bound3 = (float)bound3 / _owner->GetTexture().GetHeight();
}