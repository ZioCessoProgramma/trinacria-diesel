#pragma once

#include "Sprite.h"
#include "Assert.h"
#include "Renderer.h"
#include "ShapesData.h"

#include "Trinacria/Macros.h"
#include <cstring>

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief plays an animation made of a fixed number of sprites
	 * @tparam SpriteNum the number of frames of the animation
	 */

	template<size_t SpriteNum>
	class Animation
	{
	public:
		/**
		 * @brief advances the animation and draws the current frame
		 * @param deltaTime time elapsed since the last call
		 * @param position where to draw the animation
		 * @param color tint of the animation, defaults to white
		 * @param materialIndex the material's index to use, defaults to 0
		 */

		void Play(float deltaTime, const glm::vec2& position, const glm::vec4& color = glm::vec4(1.f), int materialIndex = 0)
		{
			_accumulatedDeltaTime += deltaTime;

			if (_accumulatedDeltaTime >= 1.f / Fps)
			{
				_currentSpriteIndex++;

				if (_currentSpriteIndex > SpriteNum - 1)
				{
					_currentSpriteIndex = 0;
				}

				_accumulatedDeltaTime = 0.f;
			}

			Transform transform(position, glm::vec2(0.5f));
			QuadData data(transform, &_sprites[_currentSpriteIndex], materialIndex, color);

			Renderer::CreateQuad(data);
			
		}

		/**
		 * @brief initializes the animation, can only be called once
		 * @param array the array of sprites (frames) of the animation
		 * @param count number of sprites in the array
		 * @param fps frames per second of the animation
		 */

		void Init(Sprite* array, size_t count, uint32_t fps)
		{
			if (!_initialized)
			{
				memcpy(_sprites, array, count * sizeof(Sprite));

				Fps = fps;
				_initialized = true;
			}
			else
			{
				TRCN_ASSERTEXP_MSG(false, "You cannot initialize an asset more than one time");
			}

		}

		/**
		 * @brief frames per second of the animation
		 */

		uint32_t Fps = 0;

	private:
		Sprite _sprites[SpriteNum];

		bool _initialized = false;

		float _accumulatedDeltaTime = 0.f;
		uint32_t _currentSpriteIndex = 0;
	};
}


