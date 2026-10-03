#include "Trinacria/Text.h"

void TRCN_CORE_NAMESPACE::Atlas::LoadAtlas(const char* fontPath, msdf_atlas::Charset charSet)
{
    if (msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype())
    {
        if (msdfgen::FontHandle *font = msdfgen::loadFont(ft, fontPath))
        {
            std::vector<msdf_atlas::GlyphGeometry> glyphs;

            _fontGeometry = std::make_unique<msdf_atlas::FontGeometry>(&glyphs);
            _fontGeometry->loadCharset(font, 1.0, charSet);

            constexpr float maxCornerAngle = 3.f;

            for (msdf_atlas::GlyphGeometry &glyph : glyphs)
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
                3,
                msdf_atlas::msdfGenerator,
                msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>
            >  generator(width, height);

            msdf_atlas::GeneratorAttributes attributes;

            generator.setAttributes(attributes);
            generator.setThreadCount(4);
            generator.generate(glyphs.data(), glyphs.size());

            Width = width;
            Height = height;

            msdfgen::destroyFont(font);
        }

        msdfgen::deinitializeFreetype(ft);

    }
}