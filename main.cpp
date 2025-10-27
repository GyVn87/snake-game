#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Snake Game");
    window.setFramerateLimit(60);

    float speed = 100.f;
    float distance;
    int direction = 0;

    sf::Clock frameClock;
    float dt;

    sf::CircleShape circle(20.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition({400.f, 300.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                    direction = 1;
                else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                    direction = 2;
                else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                    direction = 3;
                else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                    direction = 4;
            }
        }

        dt = frameClock.restart().asSeconds();
        distance = speed * dt;
        
        switch (direction) {
            case 1:
                circle.move({0.f, -distance});
                break;
            case 2:
                circle.move({-distance, 0.f});
                break;
            case 3:
                circle.move({0.f, distance});
                break;
            case 4:
                circle.move({distance, 0.f});
                break;
        }

        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}