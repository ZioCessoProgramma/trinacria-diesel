#pragma once

#include "Macros.h"
#include "Sprite.h"
#include "Texture.h"
#include "Transform.h"
#include "Renderer.h"

namespace TRCN_CORE_NAMESPACE
{
    /**
     * @brief struct for representing quads
     */

    struct QuadData
    {
        /**
         * @brief constructor for quad with textures
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param texture the texture of the quad
         * @param materialIndex the index of the desired material
         * @param color the color of the quad, it'll be multiplied with the texture color e.j. if the color is set to red the texture will appear red-ish
         * @note use this only when dealing with textured quads. If it is plain color omit the texture parameter, it has a special constructor
         */

        QuadData(const Transform& transform, Texture* texture, int materialIndex = 0, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), texture(texture), Color(color), MaterialIndex(materialIndex){ }

        /**
         * @brief constructor for quad with sprites
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param sprite the sprite of the quad
         * @param materialIndex the index of the desired material
         * @param color the color of the quad, it'll be multiplied with the sprite color e.j. if the color is set to red the sprite will appear red-ish
         * @note use this only when dealing with sprite quads. If it is plain color omit the sprite parameter, it has a special constructor
         */

        QuadData(const Transform& transform, Sprite* sprite, int materialIndex = 0, const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), sprite(sprite), Color(color), MaterialIndex(materialIndex) { }

        /**
         * @brief the constructor for plain color quads
         * @param transform the transform of the quad, it contains position, scale, rotation etc...
         * @param color the color of the quad
         * @param materialIndex the index of the desired material
         */

        QuadData(const Transform& transform, const glm::vec4& color = glm::vec4(1.f), int materialIndex = 0) :
            transform(transform), Color(color), MaterialIndex(materialIndex) { }

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
         * @brief the index of the desired material
         */

        int MaterialIndex;

        /**
         * @brief texture coordinates for quad
         * @note change it only if you know what you are doing
         */

        QuadTexCoords TexCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
                                                glm::vec2(1.f), glm::vec2(0.f, 1.f));
    };

    /**
     * @brief struct for representing triangles
     */
    struct TriangleData
    {
        /**
         * @brief constructor for triangle with textures
         * @param transform the transform of the triangle, it contains position, scale, rotation etc...
         * @param texture the texture of the triangle
         * @param materialIndex the index of the desired material
         * @param orientation the orientation of the triangle
         * @param color the color of the triangle, it'll be multiplied with the texture color e.j. if the color is set to red the texture will appear red-ish
         * @note use this only when dealing with textured triangles. If it is plain color omit the texture parameter, it has a special constructor
         */

        TriangleData(const Transform& transform, Texture* texture,
            int materialIndex = 0,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec4& color = glm::vec4(1.f)) :
                transform(transform), texture(texture), Orientation(orientation), Color(color), MaterialIndex(materialIndex) { }

        /**
         * @brief constructor for triangle with sprite
         * @param transform the transform of the triangle, it contains position, scale, rotation etc...
         * @param sprite the sprite of the triangle
         * @param materialIndex the index of the desired material
         * @param orientation the orientation of the triangle
         * @param color the color of the triangle, it'll be multiplied with the sprite color e.j. if the color is set to red the sprite will appear red-ish
         * @note use this only when dealing with sprite triangles. If it is plain color omit the sprite parameter, it has a special constructor
         */

        TriangleData(const Transform& transform,
            Sprite* sprite, int materialIndex = 0,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec4& color = glm::vec4(1.f)) :
                transform(transform), sprite(sprite), Orientation(orientation), Color(color), MaterialIndex(materialIndex) { }

        /**
         * @brief constructor for triangle with sprite
         * @param transform the transform of the triangle, it contains position, scale, rotation etc...
         * @param materialIndex the index of the desired material
         * @param orientation the orientation of the triangle
         * @param color the color of the triangle
         */

        TriangleData(const Transform& transform,
            int materialIndex = 0,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec4& color = glm::vec4(1.f)) :
            transform(transform), Orientation(orientation), Color(color), MaterialIndex(materialIndex) { }

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
         * @brief the sprite of the triangle
         * @note if et to nullptr it'll use plain color or texture
         */

        Sprite* sprite = nullptr;

        /**
         * @brief color the color of the triangle, it'll be multiplied with the sprite/texture color e.j. if the color is set to red the sprite/texture will appear red-ish
         */

        glm::vec4 Color;

        /**
         * @brief the index of the desired material
         */

        int MaterialIndex = 0;

        /**
         * @brief the orientation of the triangle
         */

        TriangleOrientation Orientation;

        /**
         * @brief the texture coordinates of the triangle
         * @note change this only if you know what you're doing
         */
        
        TriangleTexCoords TexCoords = TriangleTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f),
                                                        glm::vec2(0.f, 1.f));
    };
}