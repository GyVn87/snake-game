#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class SnakeSegment {
    private:
        sf::RectangleShape shape;
    public:
        SnakeSegment(sf::Vector2f initialPosition) : shape({40.f, 40.f}) {
            shape.setFillColor(sf::Color::Blue);
            shape.setOrigin(shape.getLocalBounds().getCenter());
            shape.setOutlineThickness(1.f);
            shape.setOutlineColor(sf::Color::Black);
            shape.setPosition(initialPosition);
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

class Food {
    private:
        sf::CircleShape shape;
    public:
        Food(sf::Vector2f initialPosition) : shape({20.f}) {
            shape.setFillColor(sf::Color::Red);
            shape.setOrigin(shape.getLocalBounds().getCenter());
            shape.setPosition(initialPosition);
        }

        void draw(sf::RenderWindow &window) {
            window.draw(shape);
        }

        void setPosition(sf::Vector2f position) {
            shape.setPosition(position);
        }

        sf::Vector2f getPosition() {
            return shape.getPosition();
        }
};

int main() {
    // Những giá trị khởi đầu
    unsigned int maxXCells = 10, maxYCells = 10, initialSegmentNumber = 3;
    float speed = 40.f, distance;
    bool isDied = false;
    char direction = 'O';

    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<int> distX(0, maxXCells - 1);
    std::uniform_int_distribution<int> distY(0, maxYCells - 1);

    sf::RenderWindow window(sf::VideoMode({maxXCells * 40, maxYCells * 40}), "Snake Game");
    window.setFramerateLimit(10);

    sf::CircleShape snakeHead(20.f);
    snakeHead.setFillColor(sf::Color::Red);
    snakeHead.setPosition({maxXCells / 2.f * 40.f + 20.f, maxYCells / 2.f * 40.f + 20.f});
    snakeHead.setOrigin(snakeHead.getLocalBounds().getCenter());
    snakeHead.setOutlineThickness(1.f);
    snakeHead.setOutlineColor(sf::Color::Black);

    std::vector<SnakeSegment> snakeSegmentVector;
    for (int index = 0; index < initialSegmentNumber; index++) {
        sf::Vector2f initalSegmentPosition({snakeHead.getPosition().x - index * 40.f - 40.f, snakeHead.getPosition().y});
        snakeSegmentVector.emplace_back(initalSegmentPosition);
    }

    Food snakeFood({static_cast<float>(distX(gen)) * 40.f + 20.f, static_cast<float>(distY(gen)) * 40.f + 20.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::W && direction != 'S')
                    direction = 'W';
                else if (keyPressed->scancode == sf::Keyboard::Scancode::A && direction != 'D' && direction != 'O')
                    direction = 'A';
                else if (keyPressed->scancode == sf::Keyboard::Scancode::S && direction != 'W')
                    direction = 'S';
                else if (keyPressed->scancode == sf::Keyboard::Scancode::D && direction != 'A')
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

        if (isDied == false) {
            if (snakeHead.getPosition() == snakeFood.getPosition()) {
                snakeFood.setPosition({static_cast<float>(distX(gen)) * 40.f + 20.f, static_cast<float>(distY(gen)) * 40.f + 20.f});
                snakeSegmentVector.emplace_back(snakeSegmentVector.back().getPosition());
            }

            if (direction != 'O') {
                for (int index = static_cast<int>(snakeSegmentVector.size()) - 1; index > 0; index--) {
                        snakeSegmentVector.at(index).setPosition(snakeSegmentVector.at(index - 1).getPosition());

                        if (snakeHead.getPosition() == snakeSegmentVector.at(index).getPosition()) 
                            isDied = true;
                }

                snakeSegmentVector.at(0).setPosition(snakeHead.getPosition());
            
                if (isDied)
                    continue;
            
                switch (direction) {
                    case 'W':
                        snakeHead.move({0.f, -speed}); // Lên
                        break;
                    case 'A':
                        snakeHead.move({-speed, 0.f}); // Trái
                        break;
                    case 'S':
                        snakeHead.move({0.f, speed}); // Xuống
                        break;
                    case 'D':
                        snakeHead.move({speed, 0.f}); // Phải
                        break;
                }
            }
        }

        for (int index = static_cast<int>(snakeSegmentVector.size()) - 1; index >= 0; index--)
            snakeSegmentVector.at(index).draw(window);

        window.draw(snakeHead);
        snakeFood.draw(window);

        window.display();
    }
    return 0;
}