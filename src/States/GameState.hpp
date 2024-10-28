#pragma once


class GameState : public Nz::State {

public:

	GameState(Nz::EnttWorld& world, Nz::Window& window);

	void Enter(Nz::StateMachine& fsm) override;
	void Leave(Nz::StateMachine& fsm) override;
	bool Update(Nz::StateMachine& fsm, Nz::Time elapsedTime) override;

private:

	void AddKeyboard(Nz::Window& window);
	void CreateGrid();
	entt::handle AddSnake(Nz::Vector2f position);
	void CollisionSnake(Nz::Vector2f position);
	void MoveSnake();


	//const
	const int m_windowSizeX;
	const int m_windowSizeY;
	const float m_squareSize = 32;
	const int m_snakeLenght = 256;

	
	Nz::Window& m_window;
	Nz::EnttWorld& m_world;
	std::shared_ptr<Nz::MaterialInstance> m_materialInstance;
	Nz::MillisecondClock m_speedSnake;

	Nz::Vector2f m_direction = { m_squareSize,0.f};
	bool m_allowMove = true;
	bool m_gameOver = false;

	std::vector<entt::handle> m_snake;
	entt::handle m_eat;

	




};
