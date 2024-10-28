#include <Nazara/Core.hpp>
#include <Nazara/Platform/Window.hpp>
#include <Nazara/Widgets.hpp>



#include <iostream>
#include <vector>

#include "GameState.hpp"
#include "MenuState.hpp"


    GameState::GameState(Nz::EnttWorld& world, Nz::Window& window) : 
		m_world(world),
		m_window(window),
		m_windowSizeX(window.GetSize().x),
		m_windowSizeY(window.GetSize().y),
		m_materialInstance(Nz::MaterialInstance::Instantiate(Nz::MaterialType::Basic))
	{
		
		CreateGrid();
		AddKeyboard(window);

		m_snake.push_back(AddSnake({ 12 , 7 }));
		m_snake.push_back(AddSnake({ 11 , 7 }));
		m_snake.push_back(AddSnake({ 10 , 7 }));
		m_snake.push_back(AddSnake({ 9 , 7 }));
		m_snake.push_back(AddSnake({ 8 , 7 }));
		
	
	};

    void GameState::Enter(Nz::StateMachine& fsm){

       		    
        std::cout << "gamestate enter\n";

    };
    void GameState::Leave(Nz::StateMachine& fsm){

	
    
        std::cout << "gamestate sortie\n";

    };
    bool GameState::Update(Nz::StateMachine& fsm, Nz::Time elapsedTime){
		
		if (m_speedSnake.RestartIfOver(Nz::Time::Seconds(0.5f))) {
			//std::cout << "Vector2f(" << m_direction.x << ", " << m_direction.y << ")" << std::endl;
			MoveSnake();
		}
								
    
		if (m_gameOver) {
			m_world.GetRegistry().clear<Nz::GraphicsComponent>();			
			fsm.ChangeState(std::make_shared<MenuState>(m_world, m_window));			
		}
			

        return true;
    };


	// Called from constructor
	void GameState::AddKeyboard(Nz::Window& window) {
		Nz::WindowEventHandler& eventHandler = window.GetEventHandler();

		//When the player press one key
		eventHandler.OnKeyPressed.Connect([&](const Nz::WindowEventHandler*, const Nz::WindowEvent::KeyEvent& key) {

			//Player Z go up
			if (m_direction.y != -m_squareSize && m_allowMove && (key.scancode == Nz::Keyboard::Scancode::W || key.scancode == Nz::Keyboard::Scancode::Up))
			{
				m_direction = {0.f , m_squareSize };
				m_allowMove = false;
			}
			//Player Q go left
			if (m_direction.x != m_squareSize && m_allowMove && (key.scancode == Nz::Keyboard::Scancode::A || key.scancode == Nz::Keyboard::Scancode::Left))
			{
				m_direction = { -m_squareSize , 0.f };
				m_allowMove = false;
			}
			//Player S go down
			if (m_direction.y != m_squareSize && m_allowMove && (key.scancode == Nz::Keyboard::Scancode::S || key.scancode == Nz::Keyboard::Scancode::Down))
			{
				m_direction = { 0.f , -m_squareSize };
				m_allowMove = false;
			}
			//Player D go right
			if (m_direction.x != -m_squareSize && m_allowMove && (key.scancode == Nz::Keyboard::Scancode::D || key.scancode == Nz::Keyboard::Scancode::Right))
			{
				m_direction = { m_squareSize, 0.f };
				m_allowMove = false;
			}

		});


	};

	void GameState::CreateGrid() {
		
		std::shared_ptr<Nz::Sprite> spriteLineVertical = std::make_shared<Nz::Sprite>(m_materialInstance);
		spriteLineVertical->SetColor(Nz::Color::Gray());
		spriteLineVertical->SetSize({ 2.f, static_cast<float>(m_windowSizeY) });

		std::shared_ptr<Nz::Sprite> spriteLineHorizontal = std::make_shared<Nz::Sprite>(m_materialInstance);
		spriteLineHorizontal->SetColor(Nz::Color::Gray());
		spriteLineHorizontal->SetSize({ static_cast<float>(m_window.GetSize().x),2.f});

		for (int i = 0; i < m_window.GetSize().x/m_squareSize+1; i++)
		{
			entt::handle entityGrid = m_world.CreateEntity();
			{
				auto& nodeGrid = entityGrid.emplace<Nz::NodeComponent>();
				nodeGrid.SetPosition({ 0.f + i * m_squareSize ,0.f,0.f });
				auto& graphicsGrid = entityGrid.emplace<Nz::GraphicsComponent>();
				graphicsGrid.AttachRenderable(spriteLineVertical);
			}
		}

		for (int i = 0; i < m_window.GetSize().y / m_squareSize + 1; i++)
		{
			entt::handle entityGrid = m_world.CreateEntity();
			{
				auto& nodeGrid = entityGrid.emplace<Nz::NodeComponent>();
				nodeGrid.SetPosition({ 0.f,0.f + i * m_squareSize,0.f });
				auto& graphicsGrid = entityGrid.emplace<Nz::GraphicsComponent>();
				graphicsGrid.AttachRenderable(spriteLineHorizontal);
			}
		}

	};

	entt::handle GameState::AddSnake(Nz::Vector2f position) {

		std::shared_ptr<Nz::Sprite> spriteSnake = std::make_shared<Nz::Sprite>(m_materialInstance);
		spriteSnake->SetColor(Nz::Color::Blue());
		spriteSnake->SetSize({ m_squareSize-4,m_squareSize - 4 });

		entt::handle entitySnake = m_world.CreateEntity();
		{
			auto& nodeGrid = entitySnake.emplace<Nz::NodeComponent>();
			nodeGrid.SetPosition({ position.x * m_squareSize + 3,position.y * m_squareSize + 3 });
			auto& graphicsGrid = entitySnake.emplace<Nz::GraphicsComponent>();
			graphicsGrid.AttachRenderable(spriteSnake);
		}
	
		return entitySnake;
	};

	void GameState::MoveSnake() {

		//std::cout << "Vector2f(" << m_direction.x << ", " << m_direction.y << ")" << std::endl;

		

		for (auto it = m_snake.rbegin(); it != m_snake.rend() - 1; it++) {

			auto nextIt = it + 1;
			auto& nodeSnake = it->get<Nz::NodeComponent>();
			auto& nodeSnakeNext = nextIt->get<Nz::NodeComponent>();

			nodeSnake.SetPosition(nodeSnakeNext.GetPosition());
		}

		auto& nodeSnake = m_snake[0].get<Nz::NodeComponent>();
		CollisionSnake({ nodeSnake.GetPosition().x + m_direction.x,nodeSnake.GetPosition().y + m_direction.y });
		nodeSnake.SetPosition(nodeSnake.GetPosition() + m_direction);
		m_allowMove = true;

	};

	void GameState::CollisionSnake(Nz::Vector2f position) {
	
		if (position.x < 0 || position.y < 0)
			m_gameOver = true;

		if (position.x > m_windowSizeX || position.y > m_windowSizeY)
			m_gameOver = true;

		for (auto it = m_snake.begin(); it != m_snake.end(); it++) {

			auto& nodeSnake = it->get<Nz::NodeComponent>();

			if (position.x == nodeSnake.GetPosition().x && position.y == nodeSnake.GetPosition().y) {
				m_gameOver = true;
				break;
			}
		}


	};



	