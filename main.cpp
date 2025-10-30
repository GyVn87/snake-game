#include <SFML/Graphics.hpp>

int main() {
    unsigned int maxXCells = 40, maxYCells = 20;

    sf::RenderWindow window(sf::VideoMode({maxXCells * 40, maxYCells * 40}), "Snake Game");
    window.setFramerateLimit(10);

    float speed = 40.f;
    float distance;
    int direction = 0;

    sf::CircleShape circle(20.f);
    circle.setFillColor(sf::Color::Blue);
    circle.setPosition({20.f, 20.f});
    circle.setOrigin({20.f, 20.f});

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

        distance = speed;
        
        switch (direction) {
            case 1:
                circle.move({0.f, -distance}); // Lên
                break;
            case 2:
                circle.move({-distance, 0.f}); // Trái
                break;
            case 3:
                circle.move({0.f, distance}); // Xuống
                break;
            case 4:
                circle.move({distance, 0.f}); // Phải
                break;
        }

        window.clear();

        for (int y = 0; y < maxYCells; y++) {
            for (int x = 0; x < maxXCells; x++) {
                sf::RectangleShape cell({40.f, 40.f});
                cell.setPosition({40.f * x, 40.f * y});
                if ((x + y) % 2 == 0) 
                    cell.setFillColor({102, 204, 0}); // Màu xanh lá đậm
                else
                    cell.setFillColor({178, 255, 102}); // Màu xanh lá nhạt
                window.draw(cell);
            }
        }

        window.draw(circle);
        window.display();
    }

    return 0;
}