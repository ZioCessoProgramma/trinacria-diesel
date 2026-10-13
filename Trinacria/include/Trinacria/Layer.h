#pragma once

#include "Trinacria/Macros.h"

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief tiny piece of logic of your game/app
	 */
	TRCN_ABSTRACT class Layer
	{
	public:
		/**
		 * @brief function to call every frame
		 * @note should be called every frame
		 * @param deltaTime the time that passes between frame in seconds
		 */

		virtual void OnUpdate(float deltaTime) = 0;

		/**
		 * @brief should be called when the layer is attached
		 * @note  should be called when the layer is attached
		 */

		virtual void OnAttach() = 0;

		/**
		 * @brief should be called when the layer is attached
		 * @note  should be called when the layer is attached
		 */

		virtual void OnDetach() = 0;

		virtual ~Layer() = default;
	};
}
