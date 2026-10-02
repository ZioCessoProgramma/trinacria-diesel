#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include <vector>

#include "Log.h"
#include "Materials.h"
#include "Shader.h"
#include "Transform.h"
#include "GLFW/glfw3.h"
#include "Trinacria/FrameBuffer.h"

#include <GLFW/glfw3.h>

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief struct representing the tex coordinates of a quad
	 * @note the coordinates should be used by absolute value in 0 to width range for x and 0 to height range for y
	 */

	struct QuadTexCoords
	{
		/**
		 * @brief coordinates for the first vertex
		 */

		glm::vec2 Coord0;

		/**
		 * @brief coordinates for the second vertex
		*/
		glm::vec2 Coord1;

		/**
		 * @brief coordinates for the third vertex
		*/
		glm::vec2 Coord2;

		/**
		 * @brief coordinates for the fourth vertex
		*/
		glm::vec2 Coord3;

		QuadTexCoords(float f) : Coord0(f), Coord1(f), Coord2(f), Coord3(f) { }

		QuadTexCoords(const glm::vec2& coord0, const glm::vec2& coord1, const glm::vec2& coord2, const glm::vec2& coord3) :
			Coord0(coord0), Coord1(coord1), Coord2(coord2), Coord3(coord3) { }

		/**
		 * @brief normalize the coordinates in device coordinates 0 to 1
		 * @param texWidth texture's width
		 * @param texHeight texture's height
		 */

		void Normalize(uint32_t texWidth, uint32_t texHeight);
	};

	/**
	 * @brief struct representing the tex coordinates of a triangle
	 * @note the coordinates should be used by absolute value in 0 to width range for x and 0 to height range for y
	 */
	struct TriangleTexCoords
	{
		/**
		 * @brief coordinates for the first vertex
		 */

		glm::vec2 Coord0;

		/**
		 * @brief coordinates for the second vertex
		*/
		glm::vec2 Coord1;

		/**
		 * @brief coordinates for the third vertex
		 */
		glm::vec2 Coord2;

		TriangleTexCoords(float f) : Coord0(f), Coord1(f), Coord2(f) { }

		TriangleTexCoords(const glm::vec2& coord0, const glm::vec2& coord1, const glm::vec2& coord2) :
			Coord0(coord0), Coord1(coord1), Coord2(coord2) { }

		/**
		 * @brief normalize in device coordinates [0->1] [0->1]
		 * @param texWidth texture's width
		 * @param texHeight texture's height
		 */
		void Normalize(uint32_t texWidth, uint32_t texHeight);
	};

	/**
	 * @brief a struct representing the data per openGL vertex
	 */
	struct Vertex
	{
		glm::vec2 Position;
		int TextureIndex;
		glm::vec2 TexCoords;
		glm::vec4 Color;
		int MaterialIndex;

		Vertex(const glm::vec2& pos, float textureIndex, const glm::vec2& texCoords, const glm::vec4& color, int materialIndex)
		: Position(pos), TextureIndex(textureIndex), TexCoords(texCoords), Color(color), MaterialIndex(materialIndex) { }
	};

	/**
	 * @brief Describes the orientation of a triangle
	 */

	enum class TriangleOrientation
	{
		Orientation_RIGHT, Orientation_LEFT
	};

	/**
	 * @brief a static class deals with all the opengl and calculations
	 */
	class Renderer
	{
	public:
		/**
		 * @brief a function that initializes the renderer
		 * @param windowDimensions window dimensions
		 * @param window glfw window
		 */
		static void Init(const glm::vec2& windowDimensions, GLFWwindow* window);

		/**
		 * @brief clears the color buffer
		 * @note to call at the start of the frame
		 * @param color the color to put in the background
		 */

		static void ClearColorBuffer(const glm::vec3& color = { 0.1f, 0.1f, 0.1f });

		/**
		 * @brief it queues a quad to be drawn
		 * @param quadData data to create a quad
		 */

		static void CreateQuad(const struct QuadData& quadData);

		/**
		 * @brief it queues a triangle to be drawn
		 * @param triangleData data to create a triangle
		 */

		static void CreateTriangle(const struct TriangleData& triangleData);

		/**
		 * @brief deals with openGL buffers
		 * @note to call when you finished the scene
		 */

		static void EndScene();

		/**
		 * @brief the function that draws all the quads and triangles queued
		 * @param screenShader the shader used to process the final texture
                 * @param windowDimensions the dimensions of the window
                 * @param cameraPos the position of the viewer or camera
                 * @param zoom zoom
		 */

		static void Draw(Shader& screenShader, const glm::vec2& windowDimensions, const glm::vec2& cameraPos, float zoom);

		/**
		 * @brief the function that delete all the openGL objects
		 * @note to call at the end of the application
		 */
		static void Cleanup();

		/**
		 * @brief the function that clears all buffers
		 * @note to call at the end of the frame after EndScene()
		 */
		static void FlushBuffers();

		/**
		 * @brief a function that adds a material to be used in the scene
		 * @param material the material to add to the scene
		 */

		static void AddMaterial(const Material& material);

		/**
		 * @brief a function that searches the material
		 * @param material the material to search
		 * @return the index of the material
		 */

		static int SearchMaterial(const Material& material);

		/**
		 * @brief a function that deals with the resizing
		 * @note to call when the window resizes
		 * @param windowDimensions the new window dimensions
		 */

		static void OnResize(const glm::vec2& windowDimensions);

		/**
		 * @brief the shader's <<manager>>
		 */
		
		static Shader ShaderProgram;

		static constexpr size_t MaxQuads = 1000;
		static constexpr size_t MaxQuadVertices = MaxQuads * 4;
		static constexpr size_t MaxQuadIndices = MaxQuads * 6;

		static constexpr size_t MaxTriangles = 1000;
		static constexpr size_t MaxTrianglesVertices = MaxTriangles * 3;

	private:
		static std::vector<Vertex> _quadBuffer;
		static std::vector<uint32_t> _quadIndexBuffer;

		static std::vector<Vertex> _triangleBuffer;

		static uint32_t _vao, _vbo, _ebo;
		static uint32_t _triangleVao, _triangleVbo;

		static std::vector<std::pair<Texture*, uint32_t>> _textures;

		static std::array<Material, 32> _materials;
		static int _materialCount;

		static FrameBuffer _frameBuffer;
		static Texture _colorAttachment;

	    inline static uint32_t _vboScreen;
	    inline static uint32_t _vaoScreen;

		static void attribPointer(uint32_t location, uint32_t nParameters, uint32_t parameterType,
			size_t stride, void* offsetFromOrigin);

		static bool findTextureIndex(uint32_t& out, const Texture* textureToFind);

		static void createQuad(
            const glm::vec2& position,
            Texture* texture = nullptr,
            const glm::vec2& size = glm::vec2(0.5f),
            const glm::vec4& color = glm::vec4(1.f),
            int materialIndex = 0,
            const glm::mat4& transform = glm::mat4(1.0f),
            const QuadTexCoords& texCoords = QuadTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f), glm::vec2(1.f), glm::vec2(0.f, 1.f))
        );

		static void createQuad(
            const glm::vec2& position,
            class Sprite* sprite,
            const glm::vec2& size = glm::vec2(0.5f),
            const glm::vec4& color = glm::vec4(1.f),
            int materialIndex = 0,
            const glm::mat4& transform = glm::mat4(1.f)
        );


		// Triangle

		static void createTriangle(
            const glm::vec2& position,
            Texture* texture = nullptr,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec2& size = glm::vec2(0.5f),
            const glm::vec4& color = glm::vec4(1.f),
            int materialIndex = 0,
            const glm::mat4& transform = glm::mat4(1.f),
            const TriangleTexCoords& texCoords = TriangleTexCoords(glm::vec2(0.f), glm::vec2(1.f, 0.f), glm::vec2(0.f, 1.f))
        );

		static void createTriangle(
            const glm::vec2& position,
            Sprite* sprite,
            TriangleOrientation orientation = TriangleOrientation::Orientation_RIGHT,
            const glm::vec2& size = glm::vec2(0.5f),
            const glm::vec4& color = glm::vec4(1.f),
            int materialIndex = 0,
            const glm::mat4& transform = glm::mat4(1.f)
        );

		static void drawInScreen(Shader& screenShader);
	    static void initScreen();
	};
}
