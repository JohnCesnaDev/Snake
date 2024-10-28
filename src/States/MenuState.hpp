#pragma once


class MenuState : public Nz::State {

public:

	MenuState(Nz::EnttWorld& world,Nz::Window& window);
	void Enter(Nz::StateMachine& fsm) override;
	void Leave(Nz::StateMachine& fsm) override;
	bool Update(Nz::StateMachine& fsm, Nz::Time elapsedTime) override;

private:
		
	Nz::Window& m_window;
	Nz::EnttWorld& m_world;
	bool m_enterGame = false;
	bool m_exit = false;
	Nz::Canvas m_canvas2D;


};


