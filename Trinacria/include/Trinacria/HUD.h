#pragma once

#include "Macros.h"
#include "Renderer.h"
#include "Shader.h"
#include "Text.h"
#include "Texture.h"
#include "Transform.h"
#include "Trinacria/InputEvents.h"
#include "glm/glm.hpp"

#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace TRCN_CORE_NAMESPACE
{
    class Sprite;
    /**
     * @brief struct containing all data a hud quad needs
     */
    struct HUDQuadData
    {
        /**
         * @brief constructor for quad with textures
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param texture the texture of the quad
         * @param color the color of the quad, it'll be multiplied with the texture color e.j. if the color is set to red the texture will appear red-ish
         * @note use this only when dealing with textured quads. If it is plain color omit the texture parameter, it has a special constructor
         */

        HUDQuadData(const Transform& transform, Texture* texture, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), texture(texture), Color(color) { }

        /**
         * @brief constructor for quad with sprites
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param sprite the sprite of the quad
         * @param color the color of the quad, it'll be multiplied with the sprite color e.j. if the color is set to red the sprite will appear red-ish
         * @note use this only when dealing with sprite quads. If it is plain color omit the sprite parameter, it has a special constructor
         */

        HUDQuadData(const Transform& transform, Sprite* sprite, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), sprite(sprite), Color(color) { }

        /**
         * @brief the constructor for plain color quads
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param color the color of the quad
         */

        HUDQuadData(const Transform& transform, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), Color(color) { }

        /**
         * @brief the transform of the quad, it has position, scale, rotation etc...
         */

        Transform transform;

        /**
         * @brief the texture of the quad
         * @note if set to nullptr it'll use plain color or sprite
         */

        Texture* texture = nullptr;

        /**
         * @brief the sprite of the quad
         * @note if set to nullptr it'll use plain color or texture
         */

        Sprite* sprite = nullptr;

        /**
         * @brief the color of the quad, it'll be multiplied with the sprite/texture color e.j. if the color is set to red the sprite/texture will appear red-ish
         */

        glm::vec4 Color;

        /**
         * @brief texture coordinates for quad
         * @note change it only if you know what you are doing
         */

        QuadTexCoords TexCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
                                                glm::vec2(1.f), glm::vec2(0.f, 1.f));
    };

    /**
     * @brief ProgressBarVertex is the vertex that is used by the HUD to make progress bars
     * @note don't worry about it
     */

    struct HUDVertex
    {
        /**
         * @brief position in screen in range [0, 1] [0, 1], pivot is at bottom left
         */

        glm::vec2 Position;

        /**
         * @brief the color the vertex should have
         */

        glm::vec4 Color;

        /**
         * @brief texture index in texture array
         * @note 0 for none
         * @note the texture array for the HUD is different from the one of the Renderer
         */

        uint32_t TextureIndex;

        /**
         * @brief the tex coordinate (range [0; 1] [0; 1]) associated with this vertex
         */

        glm::vec2 TexCoord;

        /**
         * @brief the fill texture index in the texture array
         * @note it shares the same array with the other textures
         * @details it's used to know where to fill the progress bar. It replaces magenta with the color to fill
         */

        uint32_t FillTextureIndex;

        /**
         * @brief the progress of the progress bar
         * @note -1 when no progress
         */

        float Progress;

        /**
         * @brief the color to fill the progress bar with
         * @note don't worry about it if are drawing just a quad
         */

        glm::vec4 FillColor;
    };

    /**
     * @brief the vertex used by the text
     * @note don't worry about it
     */

    struct TextVertex
    {
        /**
         * @brief the position on the screen of the vertex
         */

        glm::vec2 Position;

        /**
         * @brief the color of the vertex
         */

        glm::vec4 Color;

        /**
         * @brief the texture coordinate in the atlas of the vertex
         */

        glm::vec2 TexCoord;

        /**
         * @brief the index of the texture in an internal array
         */

        uint32_t TextureIndex;
    };

    struct HUDShaderSet
    {
        /**
         * the path to the vertex shader that batches the HUD
         */

        std::string HUDQuadVertPath;

        /**
         * the path to the fragment shader that batches the HUD
         */

        std::string HUDQuadFragPath;

        /**
        * the path to the vertex shader that batches the text
        */

        std::string TextVertPath;

        /**
         * the path to the fragment shader that batches the text
         */

        std::string TextFragPath;

        // TODO: Define

        static const HUDShaderSet DEFAULT;
    };

    using OnClickType = std::function<void()>;

    class HUD
    {
    public:

        /**
         * @brief initializes the HUD
         * @param shaderSet the set of the shader to generate
         * @param windowDimensions
         * @note to call once at the start of the application
         */

        static void Init(const HUDShaderSet& shaderSet, const glm::vec2& windowDimensions);

        /**
        * @brief cleans up the HUD
        * @note to call once at the end of the application
        */

        static void Cleanup();

        /**
         * @brief deals with openGL buffers
         * @note call when finished the HUD
         */

        static void EndHUD();

        /**
         * @brief creates a quad that will be later drawn
         * @param HUDQuad the quad to create
         */

        static void CreateHUDQuad(const HUDQuadData& HUDQuad);

        /**
         * @brief creates a button that will be later drawn
         * @param HUDQuad the quad to create
         * @param  hoveredColor the color to show when the cursor is on it
         * @param pressedColor the color to show when the button is being pressed
         */

        static void CreateButton(const HUDQuadData& HUDQuad, const glm::vec4& hoveredColor, const glm::vec4& pressedColor, GLFWwindow* window, const glm::vec2& windowDimensions);

        /**
         * @brief queues some text to draw
         * @param text the string to write in the text
         * @param transform the position, scale ..etc to draw the text
         * @param color the color of the text
         * @param inWorld should the text scale, translates, etc.. based on the world?
         * @param fontPath the path of the font
         * @param charset the charset to use for example msdf_atlas::Charset:ASCII
         * @param windowDimensions
         */

        static void CreateText(const std::string& text, const Transform& transform, const glm::vec4& color, bool inWorld, const std::string &fontPath, const
                               msdf_atlas::Charset &charset);

        /**
         * @brief it adds the specified call to a private array
         * @note to be called once per button
         * @param onClick the onClick function
         * @param transformOfTheQuad
         */

        static void AddOnClickFunction(const OnClickType& onClick, const Transform& transformOfTheQuad);

        /**
         * @brief creates a quad that can be filled like a progress bar
         * @param HUDQuad the quad to render
         * @param fillTexture the texture that contains in magenta the pixels to fill
         * @param progress the progress in range [0, 1]
         * @param fillColor the color to use when filling the progress bar
         */

        static void CreateProgressBar(const HUDQuadData& HUDQuad, Texture* fillTexture,
            float progress, const glm::vec4& fillColor);

        /**
         * @brief flushes all the buffers
         * @note call it at the end of the frame after Renderer::Draw
         */

        // TODO: add overload that makes you flush only a part of the buffer
        static void FlushBuffers();

        /**
         * @brief it check for button clicking and other events
         * @note there's no need to call this functions if InputLayer.Update() is call every frame
         * @param window the GLFW window
         * @param windowDimensions the dimensions
         */

        static void UpdateEvents(GLFWwindow* window, const glm::vec2& windowDimensions);


        static constexpr size_t MaxHUDQuads    =            100;
        static constexpr size_t MaxHUDVertices = MaxHUDQuads * 4;
        static constexpr size_t MaxHUDIndices  = MaxHUDQuads * 6;

        static constexpr size_t MaxGlyphs       =          2000;
        static constexpr size_t MaxTextVertices = MaxGlyphs * 4;
        static constexpr size_t MaxTextIndices  = MaxGlyphs * 6;

    private:
        friend class Renderer;

        inline static Shader _shader;
        inline static Shader _textShader;

        inline static uint32_t _vao;
        inline static uint32_t _vbo;
        inline static uint32_t _ebo;

        inline static uint32_t _textVao;
        inline static uint32_t _textVbo;
        inline static uint32_t _textEbo;

        inline static std::vector<HUDVertex> _vertices;
        inline static std::vector<uint32_t> _indices;

        inline static std::vector<TextVertex> _textVertices;
        inline static std::vector<uint32_t> _textIndices;

        inline static std::vector<std::pair<Texture*, uint32_t>> _textures;

	    inline static std::vector<std::pair<std::function<void()>, Transform>> _onClickAndTransform;

        inline static int _lastStateOfLeftMouseButton = GLFW_RELEASE;

        inline static float _aspectRatio;

        inline static std::vector<TextAtlas> _textAtlases;

        static void draw(const glm::vec2& windowDimensions, const glm::vec2& cameraPos, float zoom);

        static void createHUDQuad(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex,
                                  const glm::vec2& scale, glm::mat4 matrix, const QuadTexCoords& coord,
                                  uint32_t fillTextureIndex, float progress, const glm::vec4& fillColor
            );

        static void createText(const glm::vec2& position, const glm::vec4& color, uint32_t textureIndex,
                               const glm::vec2& scale, glm::mat4 matrix, const QuadTexCoords& coord
            );

        static bool findTextureIndex(uint32_t& outIndex, const Texture* texToFind);

        static uint32_t setupTexture(Texture* texture);

        static bool isInRange(const Transform& transform, GLFWwindow* window, const glm::vec2& windowDimensions);

        static void setupQuads();

        static void setupText();

        static void onResize(const glm::vec2& windowDimensions);

        static uint32_t setupAtlas(const std::string& path, const msdf_atlas::Charset &charset);

        static bool findAtlasIndex(const std::string& path, uint32_t &outIndex);
    };
}
