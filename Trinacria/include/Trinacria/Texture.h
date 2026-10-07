#pragma once

#include <stdint.h>
#include <string>

#include <glad/glad.h>
#include "Trinacria/Macros.h"

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief class representing a texture
	 */

	class Texture
	{
	public:
		/**
		 * @brief constructor for client initialization
		 * @note use this only if you know what you're doing
		 * @param id the openGL id for the texture
		 * @param width the width of the texture
		 * @param height the height of the texture
		 * @param channels the color channels of a texture
		 */

		Texture(uint32_t id, int width, int height, int channels);

		Texture() = default;

		~Texture() = default;

		/**
		 * @brief returns the openGL id of the texture
		 * @return returns the openGL id of the texture
		 */

		virtual int GetId() { return _id; }

		/**
		 * @brief returns the width of the texture
		 * @return returns the width of the texture
		 */

		virtual int GetWidth() { return _width; }

		/**
		 * @brief returns the height of the texture
		 * @return returns the height of the texture
		 */

		virtual int GetHeight() { return _height; }

		/**
		 * @brief returns the channels of the texture
		 * @return returns the channels of the texture
		 */

		virtual int GetChannels() { return _channels; }

		/**
		 * @brief it loads the texture from a specified path
		 * @note it generates the openGL texture no need to do it manually
		 * @param path the path to load texture from
		 * @param filter the filter to use. See khronos openGL documentation to know more of the filters
		 */

		virtual void LoadTexture(const std::string& path, uint32_t filter = GL_LINEAR);

		/**
		 * @brief it generates the openGL texture
		 * @note use it only once before doing any operation that isn't LoadTexture()
		 */

		virtual void GenerateTexture();

		/**
		 * @brief static function that allocates the data of the texture
		 * @param internalFormat the format used to store the data
		 * @param format the format viewed
		 * @param width the width of the texture
		 * @param height the height of the texture
		 * @param type the type of data to store in the texture
		 * @param data the data to store in the texture
		 * @param filter the filter to use. See khronos openGL documentation to know more of the filters
		 */

		void TexImage(uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data, uint32_t filter);


		/**
		 * @brief static function that allocates the data of the texture
		 * @note it binds the texture
		 * @param internalFormat the format used to store the data
		 * @param format the format viewed
		 * @param width the width of the texture
		 * @param height the height of the texture
		 * @param type the type of data to store in the texture
		 * @param data the data to store in the texture
		 * @param filter the filter to use. See khronos openGL documentation to know more of the filters
		 */

		void BoundTexImage(uint32_t
		                   internalFormat, uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data, uint32_t filter);

		/**
		 * @brief static function that changes the data of the texture
		 * @param format the format viewed
		 * @param width the width of the texture
		 * @param height the height of the texture
		 * @param type the type of data to store in the texture
		 * @param data the data to store in the texture
		 */

		static void SubImage(uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data);

		/**
		 * @brief static function that changes the data of the texture
		 * @note it binds the texture
		 * @param format the format viewed
		 * @param width the width of the texture
		 * @param height the height of the texture
		 * @param type the type of data to store in the texture
		 * @param data the data to store in the texture
		 */

		void BoundSubImage(uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data);

		/**
		 * @brief binds the texture
		 * @param textureSlot the texture slot to bind the texture
		 */

		virtual void Bind(uint32_t textureSlot);

		/**
		 * @brief binds the texture to first available texture slot
		 */

		virtual void Bind();

		/**
		 * @brief clean up the texture
		 */

		virtual void Cleanup();

		/**
		 * @brief unbinds the texture
		 * @note this function doesn't need to be done before binding another texture
		 */

		virtual void Unbind();

		/**
		 * @brief static function that refresh the texture slots
		 */

		static void ClearTextureSlots();

		/**
		 * @brief the chosen slot by the texture is the slot it will bind to when the texture slot is not specified
		 * @return the chosen slot for the texture
		 */

		uint32_t GetTextureChosenSlot() const {return _textureSlotChosenIndex;}

		/**
		 * @brief a pointer representing no texture
		 */
		static constexpr Texture* NO_TEXTURE = nullptr;
	private:
		uint32_t _id;
		int _width, _height;
		int _channels;
		uint32_t _textureSlotChosenIndex = -1; // value for not choosing any

		static bool _isTextureSlotOccupied[32];
	};
}