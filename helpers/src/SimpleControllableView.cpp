#include <SimpleControllableView.h>

SimpleControllableView::SimpleControllableView(sf::VideoMode videoMode, sf::String title, SpeedParameters speedParameters)
	: m_Window(videoMode, title), m_SpeedParameters(speedParameters)
{
	m_Window.setFramerateLimit(60);
}

void SimpleControllableView::UpdateView()
{
	sf::View view(m_ViewCenter, sf::Vector2f(1.f, 1.f));
	view.zoom(m_ZoomLevel);

	m_Window.setView(view);
}

void SimpleControllableView::ClearWindow()
{
	m_Window.clear(m_BackgroundColor);
}

void SimpleControllableView::ProcessEvents()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_Window.close();
		}
	}
}

void SimpleControllableView::UpdateInputs()
{
	sf::Vector2f direction;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		direction = { -1.f, 0.f };
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		direction = { 1.f, 0.f };
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		direction = { 0.f, -1.f };
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		direction = { 0.f, 1.f };
	}

	m_ViewCenter += direction * m_SpeedParameters.m_CameraSpeed * m_ZoomLevel;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_ZoomLevel *= m_SpeedParameters.m_ZoomSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		m_ZoomLevel /= m_SpeedParameters.m_ZoomSpeed;
	}

	m_ZoomLevel = std::max(1.f, m_ZoomLevel);
}