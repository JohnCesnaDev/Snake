#include <Nazara/Core.hpp>

#include <Nazara/Platform/Window.hpp>
#include <Nazara/Widgets.hpp>

#include <iostream>

#include "MenuState.hpp"
#include "GameState.hpp"


MenuState::MenuState(Nz::EnttWorld& world, Nz::Window& window) :
	m_world(world),
	m_window(window) ,
	m_canvas2D(world, window.GetEventHandler(), window.GetCursorController().CreateHandle(), 0xFFFFFFFF) 
{

	m_canvas2D.Resize(Nz::Vector2f(window.GetSize()));

	Nz::ButtonWidget* buttonJouer = m_canvas2D.Add<Nz::ButtonWidget>();
	buttonJouer->Center();
	buttonJouer->SetPosition({ buttonJouer->GetPosition().x - buttonJouer->GetSize().x / 2, buttonJouer->GetPosition().y + 100});
	buttonJouer->UpdateText(Nz::SimpleTextDrawer::Draw("Jouer", 42));

	Nz::ButtonWidget* buttonOption = m_canvas2D.Add<Nz::ButtonWidget>();
	buttonOption->Center();
	buttonOption->SetPosition({ buttonOption->GetPosition().x - buttonOption->GetSize().x / 2, buttonOption->GetPosition().y});
	buttonOption->UpdateText(Nz::SimpleTextDrawer::Draw("Option", 42));

	Nz::ButtonWidget* buttonQuitter = m_canvas2D.Add<Nz::ButtonWidget>();
	buttonQuitter->Center();
	buttonQuitter->SetPosition({ buttonQuitter->GetPosition().x- buttonQuitter->GetSize().x / 2, buttonQuitter->GetPosition().y - 100});
	buttonQuitter->UpdateText(Nz::SimpleTextDrawer::Draw("Quitter", 42));

	buttonJouer->OnButtonTrigger.Connect([&](const Nz::ButtonWidget*)
		{
			m_enterGame = true;
		});

	buttonQuitter->OnButtonTrigger.Connect([&](const Nz::ButtonWidget*)
		{
			m_exit = true;
		});


};

void MenuState::Enter(Nz::StateMachine& fsm){

	std::cout << "menustate enter\n";

};

void MenuState::Leave(Nz::StateMachine& fsm){

	std::cout << "menustate sortie\n";

};

bool MenuState::Update(Nz::StateMachine& fsm, Nz::Time elapsedTime){

	if (m_enterGame)
		fsm.ChangeState(std::make_shared<GameState>(m_world, m_window));

	if (m_exit)
		return false;
	
	return true;
};
