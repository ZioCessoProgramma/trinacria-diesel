#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GameLogicLayer.h"


Application* Application::Get()
{
	static Application s_instance;
	return &s_instance;
}

void Application::PushLayer(Trinacria::DSL::Layer* lay)
{
	_layerStack.emplace(_layerStack.begin(), lay); // or push_back
	lay->OnAttach();
}

void Application::PushOverlayLayer(Trinacria::DSL::Layer* lay)
{
	_layerStack.emplace_back(lay); // or push_back
	lay->OnAttach();
}


void Application::Run()
{
	if (_layerStack.empty()) return;

	float lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(_window))
	{
		const float currentTime = glfwGetTime();
		const float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		_deltaTime = deltaTime;

		for (Trinacria::DSL::Layer* l : _layerStack)
		{
			l->OnUpdate(deltaTime);
		}

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Application::CleanUp()
{
	if (_layerStack.empty()) return;

	for (Trinacria::DSL::Layer* l : _layerStack)
	{
		l->OnDetach();
		delete l;
	}

	glfwTerminate();
}

GLFWwindow* Application::GetWindow()
{
	return _window;
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	GameLogicLayer* layer = Application::Get()->GetLayer<GameLogicLayer>();
	float& zOffset = layer->ZOffset;

	constexpr float zoomFactor = 1.1f;

	if(yOffset > 0)
		 zOffset *= zoomFactor;

	if (yOffset < 0)
	{
		zOffset /= zoomFactor;
	}

	zOffset = glm::clamp(zOffset, 0.01f, 1000000.f);
}

glm::vec2 Application::GetWindowDimensions()
{
	return glm::vec2(_windowWidth, _windowHeight);
}

void Application::Init()
{
	if (!glfwInit())
	{
		std::cerr << "ERROR: GLFW was not initialized correctly" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	_window = glfwCreateWindow(_windowWidth, _windowHeight, "Test1 window", NULL, NULL);

	if (_window == NULL)
	{
		std::cerr << "ERROR: creating window did not go as expected" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "ERROR: GLAD init did not go as expected" << std::endl;
		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) 
		{
			Application::Get()->_windowWidth = width;
			Application::Get()->_windowHeight = height;

			Trinacria::DSL::Renderer::OnResize(glm::vec2(width, height));
		});

	glfwSetScrollCallback(_window, scrollCallback);

	glViewport(0, 0, _windowWidth, _windowHeight);
}
