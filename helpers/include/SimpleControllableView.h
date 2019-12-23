#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

class SimpleControllableView
{
public:
	struct SpeedParameters
	{
		float m_CameraSpeed;
		float m_ZoomSpeed;
	};

	SimpleControllableView(sf::VideoMode videoMode, sf::String title, SpeedParameters params = { 1.f, 1.f });

	inline void SetBackgroundColor(sf::Color color) { m_BackgroundColor = color; }
	inline void SetSpeedParameters(SpeedParameters params) { m_SpeedParameters = params; }

	template<typename DrawCallback>
	void RunUntilClosed(DrawCallback drawCallback)
	{
		while (m_Window.isOpen())
		{
			ClearWindow();
			ProcessEvents();
			UpdateInputs();
			UpdateView();

			drawCallback(m_Window);
			m_Window.display();
		}
	}

private:
	void ClearWindow();
	void ProcessEvents();
	void UpdateInputs();
	void UpdateView();

	sf::RenderWindow m_Window;
	sf::Color m_BackgroundColor = sf::Color::White;

	sf::Vector2f m_ViewCenter { 0.f, 0.f };
	float m_ZoomLevel = 1.f;
	
	SpeedParameters m_SpeedParameters;
};