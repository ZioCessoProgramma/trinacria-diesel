#pragma once

#include "Macros.h"

#include "Event.h"
#include "glm/vec2.hpp"

#define KEY_NUM 94

struct GLFWwindow;

namespace TRCN_CORE_NAMESPACE
{
	/**
	 * @brief the possible states of an input event
	 */

	enum class InputEventType
	{
		Pressed,
		Clicked,
		Released
	};

	/**
	 * @brief the value of every supported key, based on GLFW key codes
	 */

	enum KeyValue : uint32_t
    {
        Key_SPACE = 32, Key_APOSTROPHE = 39, Key_COMMA = 44, Key_MINUS = 45, Key_PERIOD = 46, Key_SLASH = 47,
        Key_0 = 48, Key_1 = 49, Key_2 = 50, Key_3 = 51, Key_4 = 52, Key_5 = 53, Key_6 = 54, Key_7 = 55, Key_8 = 56, Key_9 = 57,
        Key_EQUAL = 61,
        Key_A = 65, Key_B = 66, Key_C = 67, Key_D = 68, Key_E = 69, Key_F = 70, Key_G = 71, Key_H = 72, Key_I = 73, Key_J = 74,
        Key_K = 75, Key_L = 76, Key_M = 77, Key_N = 78, Key_O = 79, Key_P = 80, Key_Q = 81, Key_R = 82, Key_S = 83, Key_T = 84,
        Key_U = 85, Key_V = 86, Key_W = 87, Key_X = 88, Key_Y = 89, Key_Z = 90,
        Key_LEFT_BRACKET = 91, Key_BACKSLASH = 92, Key_RIGHT_BRACKET = 93,

        Key_ESCAPE = 256, Key_ENTER = 257, Key_TAB = 258, Key_BACKSPACE = 259,
        Key_INSERT = 260, Key_DELETE = 261, Key_RIGHT = 262, Key_LEFT = 263, Key_DOWN = 264, Key_UP = 265,
        Key_PAGE_UP = 266, Key_PAGE_DOWN = 267, Key_HOME = 268, Key_END = 269,
        Key_CAPS_LOCK = 280, Key_SCROLL_LOCK = 281, Key_NUM_LOCK = 282, Key_PRINT_SCREEN = 283, Key_PAUSE = 284,

        Key_F1 = 290, Key_F2 = 291, Key_F3 = 292, Key_F4 = 293, Key_F5 = 294, Key_F6 = 295,
        Key_F7 = 296, Key_F8 = 297, Key_F9 = 298, Key_F10 = 299, Key_F11 = 300, Key_F12 = 301,

        Key_NUMPAD_0 = 320, Key_NUMPAD_1 = 321, Key_NUMPAD_2 = 322, Key_NUMPAD_3 = 323,
        Key_NUMPAD_4 = 324, Key_NUMPAD_5 = 325, Key_NUMPAD_6 = 326, Key_NUMPAD_7 = 327,
        Key_NUMPAD_8 = 328, Key_NUMPAD_9 = 329, Key_NUMPAD_DECIMAL = 330,
        Key_NUMPAD_DIVIDE = 331, Key_NUMPAD_MULTIPLY = 332,
        Key_NUMPAD_SUBTRACT = 333, Key_NUMPAD_ADD = 334,
        Key_NUMPAD_ENTER = 335, Key_NUMPAD_EQUAL = 336
    };

	/**
	 * @brief event raised when a keyboard key is pressed, clicked or released
	 */

	class KeyboardEvent : public Event<InputEventType>
	{
	public:
		/**
		 * @brief constructor of the event
		 * @param key the key involved in the event
		 * @param type the state of the key (Pressed, Clicked, Released)
		 */

		KeyboardEvent(KeyValue key, InputEventType type) : Event(type), _key(key) {}

		/**
		 * @brief get the key involved in the event
		 * @return the key involved in the event
		 */

		KeyValue GetKey() const { return _key; }

	private:
		KeyValue _key;
	};

	/**
	 * @brief the value of every supported mouse button
	 */

	enum MouseValue
	{
		Mouse_LEFT = 0, Mouse_RIGHT = 1,
		Mouse_SCROLLWHEEL = 2,
		Mouse_4 = 3, Mouse_5 = 4
	};

	/**
	 * @brief event raised when a mouse button is pressed, clicked or released
	 */

	class MouseKeyEvent : public Event<InputEventType>
	{
	public:
		/**
		 * @brief constructor of the event
		 * @param mouseKey the mouse button involved in the event
		 * @param type the state of the button (Pressed, Clicked, Released)
		 */

		MouseKeyEvent(MouseValue mouseKey, InputEventType type) : Event(type), _mouseKey(mouseKey) {}

		/**
		 * @brief get the mouse button involved in the event
		 * @return the mouse button involved in the event
		 */

		MouseValue GetKey() const { return _mouseKey; }

	private:
		MouseValue _mouseKey;
	};

    /*enum class MouseMovedEventType { Moved };
    enum class MouseScrolledEventType { Moved };*/

    // Commented because glfw doesn't offer a way without his callbacks
    //  client will use directly glfw for mouse movement and scroll

    /*class MouseMoveEvent : Event<MouseMovedEventType>
    {
    public:
        MouseMoveEvent(uint32_t xPos, uint32_t yPos) : Event(MouseMovedEventType::Moved), _xPos(xPos), _yPos(yPos) { }

        uint32_t GetXPos() const { return _xPos; }
        uint32_t GetYPos() const { return _yPos; }

        glm::vec2 GetPosition() const { return glm::vec2(_xPos, _yPos); }
        
    private:
        uint32_t _xPos, _yPos;
    };

    class MouseScrollEvent : Event<MouseScrolledEventType>
    {
    public:
        MouseScrollEvent(int yOffset) : Event(MouseScrolledEventType::Moved), _yOffset(yOffset) { }

    private:
        int _yOffset;
    };*/

	/**
	 * @brief layer that polls keyboard and mouse state and dispatches the resulting events
	 */

	class InputPollerLayer : public Layer
	{
	public:
		/**
		 * @brief polls input state each frame and submits keyboard/mouse events
		 * @param deltaTime time elapsed since the last frame
		 */

		virtual void OnUpdate(float deltaTime) override;

		/**
		 * @brief called when the layer is attached to the application
		 */

		virtual void OnAttach() override;

		/**
		 * @brief called when the layer is detached from the application
		 */

		virtual void OnDetach() override { }

		/**
		 * @brief sets the window used to poll input state
		 * @param window the GLFW window
		 * @param windowDimensions
		 */

		void SetWindow(GLFWwindow* window, const glm::vec2& windowDimensions)
		{
		    _window = window;
		    _windowDimensions  = windowDimensions;
		}

		/**
		 * @brief dispatcher of keyboard events
		 */

		EventDispatcher<KeyboardEvent> KeyBoardDispatcher;

		/**
		 * @brief dispatcher of mouse button events
		 */

		EventDispatcher<MouseKeyEvent> MouseDispatcher;
        /*EventDispatcher<MouseScrollEvent> MouseScrollDispatcher;
        EventDispatcher<MouseMoveEvent> MouseMoveDispatcher;*/

    private:
        bool isPressed(int key) const;
        bool isReleased(int key) const;

        bool isMousePressed(int key) const;
        bool isMouseReleased(int key) const;

        void submitMouseEvent(int glfwKey, MouseValue mouseKey);

        GLFWwindow* _window = nullptr;
	    glm::vec2 _windowDimensions;

        uint32_t _keys[KEY_NUM] = {};
        bool _wasPressed[KEY_NUM] = {};

        bool _wasMousePressed[5] = {};
    };
}
