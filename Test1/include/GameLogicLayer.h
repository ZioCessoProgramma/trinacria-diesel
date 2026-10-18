#pragma once

#include <Trinacria/Layer.h>
#include "DangerZoneEvent.h"
#include <cassert>
#include <glm/glm.hpp>
#include <Trinacria/inputEvents.h>

class GameLogicLayer : public Trinacria::DSL::Layer
{
public:
	virtual void OnUpdate(float deltaTime) override;

	virtual void OnAttach() override;
	virtual void OnDetach() override {	}

	float ZOffset = 10.f;

	Trinacria::DSL::EventDispatcher<DangerZoneEnteredEvent> DangerZoneEnteredDispatcher;
	Trinacria::DSL::EventDispatcher<DangerZoneExitedEvent> DangerZoneExitedDispatcher;

private:
	glm::vec3 _cameraPos{ 0.f };
	glm::vec2 _playerPos{ -1.5f };

	bool _wasInDangerZone = false;

	void keyBoardInput(const Trinacria::DSL::KeyboardEvent&);
};

