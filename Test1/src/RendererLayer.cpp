#include "RendererLayer.h"
#include "Application.h"
#include "GameLogicLayer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <Trinacria/ShapesData.h>

#include "Trinacria/HUD.h"
#include "Trinacria/LightSystem.h"
#include "Trinacria/Materials.h"

#define ANIMATION_FPS 12

static glm::vec3 color{ 1.f };

void RendererLayer::OnUpdate(float deltaTime)
{
    Trinacria::DSL::Renderer::ClearColorBuffer();

    //_buttonTransform.Pivot = glm::vec2(glm::sin(glfwGetTime()), 0.5f);

    Trinacria::DSL::HUDQuadData progressBar (Trinacria::DSL::Transform(glm::vec2(0.f, -0.9f),
		glm::vec2(0.8f, 0.1f), 0, glm::vec2(0.5f)), &_healthBar, glm::vec4(1.f, 1.f, 1.f, 1.f));

    Trinacria::DSL::HUD::CreateProgressBar(progressBar, &_healthBarFillMap, (glm::sin(glfwGetTime()) + 1) / 2,
	    glm::vec4(0.2f, 0.7f, 0.2f, 0.7f));

    Trinacria::DSL::HUDQuadData quad(_buttonTransform, glm::vec4(0.7f, 0.2f, 0.2f, 1.f));

    Trinacria::DSL::HUD::CreateButton(quad, quad.Color - glm::vec4(0.1f, 0.1f, 0.1f, 0.f), quad.Color - glm::vec4(0.2f, 0.2f, 0.2f, 0.f), Application::Get()->GetWindow(), Application::Get()->GetWindowDimensions());

    Trinacria::DSL::HUDQuadData quad_(_buttonTransform1, glm::vec4(0.2f, 0.7f, 0.2f, 1.f));

    Trinacria::DSL::HUD::CreateButton(quad_, quad_.Color - glm::vec4(0.1f, 0.1f, 0.1f, 0.f), quad_.Color - glm::vec4(0.2f, 0.2f, 0.2f, 0.f), Application::Get()->GetWindow(), Application::Get()->GetWindowDimensions());

    Trinacria::DSL::HUDQuadData progressBar1 (Trinacria::DSL::Transform(glm::vec2(0.f, -0.8f),
		glm::vec2(0.6f, 0.1f), 0, glm::vec2(0.5f)), &_healthBar, glm::vec4(1.f, 1.f, 1.f, 1.f));

    Trinacria::DSL::HUD::CreateProgressBar(progressBar1, &_healthBarFillMap, (glm::cos(glfwGetTime()) + 1) / 2,
	    glm::vec4(0.2f, 0.2f, 0.6f, 1.f));



    // Ground

    Trinacria::DSL::Transform ground(glm::vec2(0.f), glm::vec2(100.f));
    Trinacria::DSL::QuadData groundData(ground, glm::vec4(1.f), 2);

    Trinacria::DSL::Renderer::CreateQuad(groundData);

    // Grid

    const float size = 1.f / 2;
    int i = 0;

    for (int x = 0; x < 11; x++)
    {
	for (int y = 0; y < 13; y++)
	{
	    Trinacria::DSL::Texture* tex = i % 2 == 0 ? &_grey : &_purple;

	    Trinacria::DSL::Transform transform(glm::vec2(x * size, y * size));
	    Trinacria::DSL::QuadData data(transform, tex);

	    Trinacria::DSL::Renderer::CreateQuad(data);
	    i++;
	}
    }


    // Grass

    Trinacria::DSL::Transform transform1(glm::vec2(0.5f), glm::vec2(4.f),
	    glfwGetTime() * 60);

    Trinacria::DSL::QuadData grassData(transform1, glm::vec4(0.1f, 0.6f, 0.1f, 1.f));

    Trinacria::DSL::Renderer::CreateQuad(grassData);

    // Tree

    {
	    Trinacria::DSL::Transform transform(glm::vec2(-1.f));
	    Trinacria::DSL::QuadData treeData(transform, &_tree);

	    Trinacria::DSL::Renderer::CreateQuad(treeData);
    }

    // Danger Zone

    {
	    Trinacria::DSL::Transform transform(glm::vec2(3.f), glm::vec2(6.f), 0.f, glm::vec2(0.f));
	    Trinacria::DSL::QuadData data(transform, glm::vec4(0.7f, 0.2f, 0.2f, 1.f), 1);

	    Trinacria::DSL::Renderer::CreateQuad(data);
    }

    // Player

    _playerAnimation.Play(deltaTime, _playerPos, glm::vec4(color, 1.f));

    // Triangle

    {
	    Trinacria::DSL::Transform transform(glm::vec2(-4.f), glm::vec2(4.f), sin(glfwGetTime()) * 20);

	    Trinacria::DSL::TriangleData data(transform, Trinacria::DSL::Texture::NO_TEXTURE,
		    0, Trinacria::DSL::TriangleOrientation::Orientation_RIGHT, glm::vec4(0.2f, 0.2f, 0.6f, 1.f));

	    Trinacria::DSL::Renderer::CreateTriangle(data);
    }

    // Point light

    Trinacria::DSL::Transform pointLightTransform(glm::vec2(-4.f, 3.f));
    Trinacria::DSL::QuadData pointData(pointLightTransform);


    Trinacria::DSL::Renderer::CreateQuad(pointData);

    Trinacria::DSL::Transform pointLightTransform1(glm::vec2(4.f, 4.f));

    // Spotlight

    Trinacria::DSL::Transform spotLightTransform(glm::vec2(-7.f, -3.f), glm::vec2(0.5f), 0,
	    glm::vec2(1.f, 0.f));

    Trinacria::DSL::Transform spotLightTransform1(glm::vec2(7.f, -3.f), glm::vec2(0.5f), 0,
	    glm::vec2(1.f, 0.f));

    Trinacria::DSL::QuadData data(spotLightTransform);
    Trinacria::DSL::QuadData data1(spotLightTransform1);

    Trinacria::DSL::Renderer::CreateQuad(data);
    Trinacria::DSL::Renderer::CreateQuad(data1);

    {
	    Trinacria::DSL::Transform transform(glm::vec2(3.15f, 3.15f));
	    Trinacria::DSL::TriangleData data_(transform, 0, Trinacria::DSL::TriangleOrientation::Orientation_RIGHT,
		    glm::vec4(0.0f, 1.f, 0.0f, 0.6f));

	    Trinacria::DSL::Renderer::CreateTriangle(data_);
    }


    Trinacria::DSL::Renderer::EndScene();

    Trinacria::DSL::HUD::CreateText("Health", {{0.f, -0.9f}, {0.05f, 0.05f}},
                                    glm::vec4(1.f, 1.f, 1.f, 1.f),
                                    false, "assets/fonts/Orbitron/Orbitron-Medium.ttf",
                                    {1.f, 1.f}, msdf_atlas::Charset::ASCII);

    Trinacria::DSL::HUD::EndHUD();

     Trinacria::DSL::LightSystem::InitFrame(Trinacria::DSL::LightSystem::DEFAULT_USE_OF_LIGHT);

    Trinacria::DSL::PointLightData pointLight( glm::vec3(1.f, 1.f, 1.f),
	    pointLightTransform.Position, 1.f);

    Trinacria::DSL::PointLightData pointLight1( glm::vec3(1.f, 1.f, 1.f),
	    pointLightTransform1.Position, 1.f);

    Trinacria::DSL::SpotLightData spotLight(glm::vec3(1.f), spotLightTransform.Position,
	    glm::vec2(-1.f, 1.f), 1.f,
	    12.5f, 17.5f);

    Trinacria::DSL::SpotLightData spotLight1(glm::vec3(1.f), spotLightTransform1.Position,
	    glm::vec2(-1.f, 1.f), 1.f,
	    12.5f, 17.5f);

    Trinacria::DSL::DirectionalLightData dirLight(glm::vec3(0.1f), glm::vec2(0.1f, 0.4f));

    Trinacria::DSL::LightSystem::SetViewPos(_cameraData.CameraPos);

    Trinacria::DSL::LightSystem::SetupLight(pointLight1);
    Trinacria::DSL::LightSystem::SetupLight(pointLight);
    Trinacria::DSL::LightSystem::SetupLight(spotLight1);
    Trinacria::DSL::LightSystem::SetupLight(spotLight);
    Trinacria::DSL::LightSystem::SetupLight(dirLight);

    Trinacria::DSL::Renderer::AddMaterial(Trinacria::DSL::SOLID_OPAQUE);
    Trinacria::DSL::Renderer::AddMaterial(Trinacria::DSL::METAL);

    Trinacria::DSL::Material groundMaterial(glm::vec3(1.f), glm::vec3(1.f), glm::vec3(0.4f), 8.f);

    Trinacria::DSL::Renderer::AddMaterial(groundMaterial);

    Trinacria::DSL::LightSystem::Done();

    Trinacria::DSL::Renderer::Draw(_screenShader, Application::Get()->GetWindowDimensions(), _cameraData.CameraPos, _cameraData.Zoom);
    Trinacria::DSL::Renderer::FlushBuffers();
    Trinacria::DSL::HUD::FlushBuffers();
}

void RendererLayer::OnAttach()
{
    _grey.LoadTexture("assets/textures/grey.jpg");
    _purple.LoadTexture("assets/textures/purple.jpg");
    _triangleTex.LoadTexture("assets/textures/425.jpg");
    _healthBar.LoadTexture("assets/textures/bar.png");
    _healthBarFillMap.LoadTexture("assets/textures/bar-fill.png", GL_NEAREST);

    _playerAnimationSpriteSheet.LoadTexture("assets/textures/player.png", GL_NEAREST);

    _playerAnimationSpriteSheet.CellWidth = (float) _playerAnimationSpriteSheet.GetWidth() / 3.f;
    _playerAnimationSpriteSheet.CellHeight= (float) _playerAnimationSpriteSheet.GetHeight() / 3.f;

    _tinyVillage.LoadTexture("assets/textures/tilemap.png", GL_NEAREST);

    _tinyVillage.CellWidth = (float) _tinyVillage.GetWidth() / 12.f;
    _tinyVillage.CellHeight = (float) _tinyVillage.GetHeight() / 11.f;

    _playerAnimation.Init(_animation, 3, 12);

    GameLogicLayer* layer = Application::Get()->GetLayer<GameLogicLayer>();

    if (layer)
    {
	layer->DangerZoneEnteredDispatcher.RegistCallback(OnEnteredDangerZone);

	layer->DangerZoneExitedDispatcher.RegistCallback([](const DangerZoneExitedEvent& event) {
		color = event.GetColorToRestore();
		});
    }

    Trinacria::DSL::Renderer::Init(Application::Get()->GetWindowDimensions(), Application::Get()->GetWindow());

    Trinacria::DSL::Renderer::ShaderProgram.LoadCoreShader("assets/shaders/Quad.vert",
	    "assets/shaders/Quad.frag");


    _screenShader.LoadShader("assets/shaders/Screen.vert", "assets/shaders/Screen.frag");

    Trinacria::DSL::LightSystem::Init();

    Trinacria::DSL::HUD::Init({"assets/shaders/HUD.vert",
        "assets/shaders/HUD.frag", "assets/shaders/Text.vert",
        "assets/shaders/Text.frag"},
        Application::Get()->GetWindowDimensions());

    Trinacria::DSL::HUD::AddOnClickFunction([] {TRCN_LOG("Hello Guys!");}, _buttonTransform);
    Trinacria::DSL::HUD::AddOnClickFunction([] {TRCN_LOG("Hello Girls!");}, _buttonTransform1);
}

void RendererLayer::OnDetach()
{
    Trinacria::DSL::Renderer::Cleanup();
    Trinacria::DSL::HUD::Cleanup();
}

void RendererLayer::SetCameraData(const CameraData& cameraData)
{
    _cameraData = cameraData;
}

void RendererLayer::SetPlayerPos(glm::vec2 playerPos)
{
    _playerPos = playerPos;
}

void RendererLayer::OnEnteredDangerZone(const DangerZoneEnteredEvent& event)
{
    color = glm::vec3(event.GetDamageToApply(), 0.2f, 0.2f);
}

// Note: for matrix multiplication see this:
//	https://stackoverflow.com/questions/34303483/rotating-scaling-and-translating-2d-points-in-glm
