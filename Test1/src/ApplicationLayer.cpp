#include "ApplicationLayer.h"
#include "Application.h"
#include "RendererLayer.h"
#include "GameLogicLayer.h"
#include <Trinacria/InputEvents.h>

void ApplicationLayer::OnAttach()
{
	// Will be destroyed by application decostructor
	auto* layer = new Trinacria::DSL::InputPollerLayer;
	layer->SetWindow(Application::Get()->GetWindow(), Application::Get()->GetWindowDimensions());

	Application::Get()->PushOverlayLayer(layer);

	Application::Get()->PushLayer<GameLogicLayer>();
	Application::Get()->PushLayer<RendererLayer>();
}
