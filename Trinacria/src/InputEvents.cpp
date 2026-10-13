#include "Trinacria/InputEvents.h"
#include "Trinacria/HUD.h"

#include <GLFW/glfw3.h>

void TRCN_CORE_NAMESPACE::InputPollerLayer::OnUpdate(float deltaTime)
{
	for (int i = 0; i < KEY_NUM; i++)
	{
		if (isPressed(_keys[i]))
		{
			KeyBoardDispatcher.Submit(this, KeyboardEvent((KeyValue) _keys[i], InputEventType::Pressed));
			
			if(!_wasPressed[i])
				KeyBoardDispatcher.Submit(this, KeyboardEvent((KeyValue) _keys[i], InputEventType::Clicked));

			_wasPressed[i] = true;
		}

		// For GLFW when it's not pressed it is automatically released
		//	for me it has to be pressed first
		if (isReleased(_keys[i]) && _wasPressed[i])
		{
			KeyBoardDispatcher.Submit(this, KeyboardEvent((KeyValue)_keys[i], InputEventType::Released));
			_wasPressed[i] = false;
		}
	}

	submitMouseEvent(GLFW_MOUSE_BUTTON_LEFT, Mouse_LEFT);
	submitMouseEvent(GLFW_MOUSE_BUTTON_RIGHT, Mouse_RIGHT);
	submitMouseEvent(GLFW_MOUSE_BUTTON_MIDDLE, Mouse_SCROLLWHEEL);
	submitMouseEvent(GLFW_MOUSE_BUTTON_4, Mouse_4);
	submitMouseEvent(GLFW_MOUSE_BUTTON_5, Mouse_5);
	
	HUD::UpdateEvents(_window, _windowDimensions);
}

void TRCN_CORE_NAMESPACE::InputPollerLayer::OnAttach()
{
	KeyBoardDispatcher.SetOwner(this);
	MouseDispatcher.SetOwner(this);

	uint32_t numberToPut = 0;

	for (int i = 0; i < KEY_NUM; i++)
	{
		switch (i)
		{
		case 0:
			_keys[i] = KeyValue::Key_SPACE;
			break;
		case 1:
			_keys[i] = KeyValue::Key_APOSTROPHE;
			numberToPut = KeyValue::Key_COMMA;
			break;

		case 16:
			_keys[i] = KeyValue::Key_EQUAL;
			numberToPut = KeyValue::Key_A;
			break;
		
		case 46:
			_keys[i] = KeyValue::Key_ESCAPE;
			numberToPut = KeyValue::Key_ENTER;
			break;

		case 65:
			_keys[i] = KeyValue::Key_F1;
			numberToPut = KeyValue::Key_F2;
			break;

		case 60:
			_keys[i] = KeyValue::Key_CAPS_LOCK;
			numberToPut = KeyValue::Key_SCROLL_LOCK;
			break;

		case 77:
			_keys[i] = KeyValue::Key_NUMPAD_0;
			numberToPut = KeyValue::Key_NUMPAD_1;
			break;

		default:
			_keys[i] = numberToPut;
			numberToPut++;
			break;
		}
	}
}

bool TRCN_CORE_NAMESPACE::InputPollerLayer::isPressed(int key) const
{
	return glfwGetKey(_window, key) == GLFW_PRESS;
}

bool TRCN_CORE_NAMESPACE::InputPollerLayer::isReleased(int key) const 
{
	return glfwGetKey(_window, key) == GLFW_RELEASE;
}

bool TRCN_CORE_NAMESPACE::InputPollerLayer::isMousePressed(int key) const
{
	return glfwGetMouseButton(_window, key) == GLFW_PRESS;
}

bool TRCN_CORE_NAMESPACE::InputPollerLayer::isMouseReleased(int key) const
{
	return glfwGetMouseButton(_window, key) == GLFW_RELEASE;
}

void TRCN_CORE_NAMESPACE::InputPollerLayer::submitMouseEvent(int glfwKey, MouseValue mouseKey)
{
	if (isMousePressed(glfwKey))
	{
		MouseDispatcher.Submit(this, MouseKeyEvent(mouseKey, InputEventType::Pressed));

		if (!_wasMousePressed[mouseKey]) 
			MouseDispatcher.Submit(this, MouseKeyEvent(mouseKey, InputEventType::Clicked));

		_wasMousePressed[mouseKey] = true;
	}

	if (isMouseReleased(glfwKey) && _wasMousePressed[mouseKey])
	{
		MouseDispatcher.Submit(this, MouseKeyEvent(mouseKey, InputEventType::Released));

		_wasMousePressed[mouseKey] = false;
	}
}
