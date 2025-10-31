#include <SFML/Graphics.hpp>
#include <vector>

class SnakeSegment {
    private:
        sf::RectangleShape shape;
    public:
        SnakeSegment() : shape({40.f, 40.f}) {
            shape.setFillColor(sf::Color::Blue);
            shape.setOrigin(shape.getLocalBounds().getCenter());
            shape.setOutlineThickness(1.f);
            shape.setOutlineColor(sf::Color::Black);
        }

        void setPosition(sf::Vector2f position) {
            shape.setPosition(position);
        }

        void draw(sf::RenderWindow &window) {
            window.draw(shape);
        }

        sf::Vector2f getPosition() {
            return shape.getPosition();
        }
};

int main() {
    unsigned int maxXCells = 40, maxYCells = 20, segmentsNumber = 10;

    sf::RenderWindow window(sf::VideoMode({maxXCells * 40, maxYCells * 40}), "Snake Game");
    window.setFramerateLimit(10);

    float speed = 40.f;
    float distance;
    char direction = 'O';

    sf::CircleShape snakeHead(20.f);
    snakeHead.setFillColor(sf::Color::Red);
    snakeHead.setPosition({20.f, 20.f});
    snakeHead.setOrigin(snakeHead.getLocalBounds().getCenter());
    snakeHead.setOutlineThickness(1.f);
    snakeHead.setOutlineColor(sf::Color::Black);

    std::vector<SnakeSegment> snakeSegmentVector;
    for (int index = 0; index < segmentsNumber; index++) {
        snakeSegmentVector.emplace_back();
    }
    SnakeSegment &firstSegment = snakeSegmentVector.at(0);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                    direction = 'W';
                else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                    direction = 'A';
                else if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                    direction = 'S';
                else if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                    direction = 'D';
            }
        }

        window.clear();

        //  Hình nền
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

        for (int index = segmentsNumber - 1; index > 0; index--) {
            SnakeSegment &segment = snakeSegmentVector.at(index);
            segment.setPosition(snakeSegmentVector.at(index - 1).getPosition());
            segment.draw(window);
        }

        firstSegment.setPosition(snakeHead.getPosition());
        firstSegment.draw(window);

        distance = speed;
        
        switch (direction) {
            case 'W':
                snakeHead.move({0.f, -distance}); // Lên
                break;
            case 'A':
                snakeHead.move({-distance, 0.f}); // Trái
                break;
            case 'S':
                snakeHead.move({0.f, distance}); // Xuống
                break;
            case 'D':
                snakeHead.move({distance, 0.f}); // Phải
                break;
        }

        window.draw(snakeHead);

        window.display();
    }
    return 0;
}