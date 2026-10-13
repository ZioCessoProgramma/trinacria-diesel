#pragma once

#include <Trinacria/Layer.h>
#include <Trinacria/Renderer.h>
#include <Trinacria/Sprite.h>
#include <Trinacria/Animation.h>

#include "DangerZoneEvent.h"

#define QUAD_COUNT 145
#define VERTICES_COUNT QUAD_COUNT * 4
#define INDICES_COUNT QUAD_COUNT * 6

struct CameraData
{
	glm::vec3 CameraPos;
	float Zoom;
};

class RendererLayer : public Trinacria::DSL::Layer
{
public:
	virtual void OnUpdate(float deltaTime);

	virtual void OnAttach();
	virtual void OnDetach();

	void SetCameraData(const CameraData& cameraData);
	void SetPlayerPos(glm::vec2 playerPos);

	static void OnEnteredDangerZone(const DangerZoneEnteredEvent& event);

private:
	glm::vec2 _playerPos{ 0.f };

	Trinacria::DSL::Texture _grey;
	Trinacria::DSL::Texture _purple;
	Trinacria::DSL::Texture _wood;
	Trinacria::DSL::Texture _triangleTex;
	Trinacria::DSL::Texture _healthBar;
	Trinacria::DSL::Texture _healthBarFillMap;

	Trinacria::DSL::QuadTexCoords _lastCoords{ glm::vec2(0.f, 2.f / 3), glm::vec2(1.f / 3, 2.f / 3), glm::vec2(1.f / 3, 1.f), glm::vec2(0.f, 1.f) };

	Trinacria::DSL::SpriteSheet _playerAnimationSpriteSheet{ 0.f, 0.f };

	Trinacria::DSL::SpriteSheet _tinyVillage{ 0.f, 0.f };

	Trinacria::DSL::Sprite _animation[3] = { Trinacria::DSL::Sprite(&_playerAnimationSpriteSheet, 0, 2), Trinacria::DSL::Sprite(&_playerAnimationSpriteSheet, 1, 2),
		Trinacria::DSL::Sprite(&_playerAnimationSpriteSheet, 2, 2) };

	Trinacria::DSL::Animation<3> _playerAnimation;

	Trinacria::DSL::Sprite _tree{ &_tinyVillage, 3, 9, 1, 2 };

	uint32_t _currentSpriteIndex = 0;

	CameraData _cameraData;

	Trinacria::DSL::Shader _screenShader;

    Trinacria::DSL::Transform _buttonTransform = Trinacria::DSL::Transform(glm::vec2(-0.9f, 0.f),
        glm::vec2(0.1f, 0.1f),0, glm::vec2(0.5f, 0.5f));

    Trinacria::DSL::Transform _buttonTransform1 = Trinacria::DSL::Transform(glm::vec2(0.9f, 0.f),
    glm::vec2(0.1f, 0.1f), 0, glm::vec2(1.f, 0.f));
};

