#pragma once

#include "Trinacria/Macros.h"
#include "Log.h"
#include "Layer.h"
#include <functional>
#include <vector>

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief base class for every event, polymorphic and templated on its category type
	 * @tparam Category the enum type used to identify the event kind
	 */

	template<typename Category>
	TRCN_ABSTRACT class Event
	{
	public:
		/**
		 * @brief constructor of the event
		 * @param type the category of the event
		 */

		Event(Category type) : _type(type) { }

		/**
		 * @brief get the category of the event
		 * @return the category of the event
		 */

		Category GetType() const { return _type; }

	protected:
		Category _type;
	};

	/**
	 * @brief dispatches events of a given type to all registered callbacks
	 * @tparam EventType the type of event this dispatcher handles
	 */

	template<typename EventType>
	class EventDispatcher
	{
		using CallbackType = std::function<void(const EventType& event)>;

	public:
		/**
		 * @brief register a callback to be called when an event is submitted
		 * @param callBack the callback to register
		 */

		void RegistCallback(CallbackType callBack)
		{
			_callBacks.push_back(callBack);
		}

		/**
		 * @brief sets the owner layer of the dispatcher, can only be set once
		 * @param owner the layer that owns this dispatcher
		 */

		void SetOwner(Layer* owner)
		{
			if (!_hasOwnerBeenSet)
			{
				_owner = owner;
				_hasOwnerBeenSet = true;
			}
			else
			{
				TRCN_LOG("Owner of this event has been set already");
			}
		}

		/**
		 * @brief submits an event to the owner layer, invoking all registered callbacks
		 * @param context the layer submitting the event
		 * @param event the event to dispatch
		 */

		void Submit(Layer* context, const EventType& event)
		{
			if (context != _owner) return;

			for (CallbackType& callBack : _callBacks)
			{
				callBack(event);
			}
		}

	private:
		std::vector<CallbackType> _callBacks;

		Layer* _owner = nullptr;
		bool _hasOwnerBeenSet = false;
	};
}