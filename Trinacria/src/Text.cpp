#include <Trinacria/Text.h>
#include <Trinacria/Macros.h>

#include "Trinacria/Log.h"
#include "glad/glad.h"

#include <msdf-atlas-gen/msdf-atlas-gen.h>

void TRCN_CORE_NAMESPACE::Atlas::LoadAtlas(const char* fontPath, const msdf_atlas::Charset& charSet)
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

    std::vector<msdf_atlas::GlyphGeometry> glyphs;

    _fontGeometry = std::make_unique<msdf_atlas::FontGeometry>(&glyphs);
    _fontGeometry->loadCharset(font, 1.0, charSet);

    constexpr float maxCornerAngle = 3.f;

    for (msdf_atlas::GlyphGeometry& glyph : glyphs)
        glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

    msdf_atlas::TightAtlasPacker packer;

    packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
    packer.setMinimumScale(24.0);
    packer.setPixelRange(2.0);
    packer.setMiterLimit(1.0);
    packer.pack(glyphs.data(), glyphs.size());

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
    generator.generate(glyphs.data(), glyphs.size());

    glGenTextures(1, &_atlasTexture);
    glBindTexture(GL_TEXTURE_2D, _atlasTexture);

    const msdfgen::BitmapConstRef<unsigned char, 4> bitmap;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Width = width;
    Height = height;

    msdfgen::destroyFont(font);

    msdfgen::deinitializeFreetype(ft);
}
const msdf_atlas::GlyphGeometry* Trinacria::DSL::Atlas::GetGlyph(char c)
{
    return _fontGeometry->getGlyph(c);
}
void Trinacria::DSL::Glyph::LoadGlyph(char c)
{
    const msdf_atlas::GlyphGeometry* glyph = _owner->GetGlyph(c);

    double bound0, bound1, bound2, bound3;
    glyph->getQuadAtlasBounds(bound0, bound1, bound2, bound3);

    _bound0 = (float)bound0;
    _bound1 = (float)bound1;
    _bound2 = (float)bound2;
    _bound3 = (float)bound3;
}