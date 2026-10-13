#pragma once

#include <cstdint>

#include "Macros.h"

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief class representing a framebuffer
	 */

	class FrameBuffer
	{
	public:
		/**
		 * @brief deletes the framebuffer
		 * @note to call at the end of the application
		 */

		void Cleanup();

		/**
		 * @brief generates the framebuffer
		 * @note use it only once before doing any operation
		 */

		void GenFrameBuffer();

		/**
		 * @brief binds the framebuffer
		 */

		void Bind() const;

		/**
		 * @brief binds the framebuffer and attaches a texture to it
		 * @param attachment the attachment point to attach the texture to
		 * @param textureType the type of texture to attach
		 * @param textureId the openGL id of the texture to attach
		 */

		void BindAttachTexture(uint32_t attachment, uint32_t textureType, uint32_t textureId) const;

	private:
		uint32_t _frameBufferId = 0;
	};
}