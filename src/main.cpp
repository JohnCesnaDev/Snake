#include <Nazara/Graphics.hpp>
#include <Nazara/Core.hpp>
#include <Nazara/Platform.hpp>
#include <Nazara/Widgets.hpp>

#include "main.h"
#include "States/GameState.hpp"
#include "States/MenuState.hpp"


int main()
{
	Nz::Application < Nz::Graphics, Nz::Widgets > app;
		

	auto& windowComponent = app.AddComponent<Nz::WindowingAppComponent>();

	std::string title = "Snake by JC";
	Nz::WindowStyleFlags flagWindow = Nz::WindowStyle::Closable | Nz::WindowStyle::Titlebar;
	Nz::Window& mainWindow = windowComponent.CreateWindow(Nz::VideoMode(802, 482), title, flagWindow);
	
	auto& ecs = app.AddComponent<Nz::EntitySystemAppComponent>();
	auto& world = ecs.AddWorld<Nz::EnttWorld>();

	auto& renderSystem = world.AddSystem<Nz::RenderSystem>();
	auto& windowSwapchain = renderSystem.CreateSwapchain(mainWindow);


	entt::handle cameraEntity = world.CreateEntity();
	{
		auto& cameraNode = cameraEntity.emplace<Nz::NodeComponent>();
		auto& cameraComponent = cameraEntity.emplace<Nz::CameraComponent>(std::make_shared<Nz::RenderWindow>(windowSwapchain), Nz::ProjectionType::Orthographic);
		cameraComponent.UpdateClearColor(Nz::Color(0.46f, 0.48f, 0.84f, 1.f));
	}

	Nz::StateMachine fsm{ std::make_shared<MenuState>(world, mainWindow) };
	
	app.AddUpdaterFunc([&](Nz::Time elapsedTime) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 240));

		// We update the state machine which will ensure that our states are updated
		if (!fsm.Update(elapsedTime)) {
			NazaraError("Failed to update state machine");
			app.Quit();
		}
		});
	

	return app.Run();
}

