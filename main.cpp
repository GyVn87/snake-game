#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class SnakeSegment {
    private:
        sf::CircleShape shape;
    public:
        SnakeSegment(sf::Vector2f initialPosition) : shape(18.f) {
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
        Food(sf::Vector2f initialPosition) : shape({15.f}) {
            shape.setFillColor(sf::Color::Magenta);
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
    unsigned int maxXCells = 10, maxYCells = 10, initialSegmentNumber = 5;
    float speed = 40.f, distance;
    bool isDied = false;
    char direction = 'O';
    char currentDirection = 'O';

    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<int> distX(0, maxXCells - 1);
    std::uniform_int_distribution<int> distY(0, maxYCells - 1);

    std::vector<std::vector<int>> visitedGrid(maxYCells, std::vector<int>(maxXCells, 0));

    sf::RenderWindow window(sf::VideoMode({maxXCells * 40, maxYCells * 40}), "Snake Game");
    window.setFramerateLimit(10);

    sf::CircleShape snakeHead(20.f);
    snakeHead.setFillColor(sf::Color::Red);
    snakeHead.setPosition({maxXCells / 2 * 40.f + 20.f, maxYCells / 2 * 40.f + 20.f});
    snakeHead.setOrigin(snakeHead.getLocalBounds().getCenter());
    snakeHead.setOutlineThickness(1.f);
    snakeHead.setOutlineColor(sf::Color::Black);

    std::vector<SnakeSegment> snakeSegmentVector;
    snakeSegmentVector.reserve(maxXCells * maxYCells);
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

        if ((currentDirection == 'W' && direction == 'S') || 
            (currentDirection == 'A' && direction == 'D') || 
            (currentDirection == 'S' && direction == 'W') || 
            (currentDirection == 'D' && direction == 'A'))
            direction = currentDirection;

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

        if (!isDied && direction != 'O') {
            if (snakeHead.getPosition() == snakeFood.getPosition()) {
                snakeSegmentVector.emplace_back(snakeSegmentVector.back().getPosition());
                do snakeFood.setPosition({static_cast<float>(distX(gen)) * 40.f + 20.f, static_cast<float>(distY(gen)) * 40.f + 20.f});
                while (visitedGrid.at(static_cast<int>(snakeFood.getPosition().y) / 40).at(static_cast<int>(snakeFood.getPosition().x) / 40) == 1);
            }

            for (int index = 0; index < maxYCells; index++)
                std::fill(visitedGrid.at(index).begin(), visitedGrid.at(index).end(), 0);

            sf::Vector2f tailPosition = snakeSegmentVector.back().getPosition();
            for (int index = static_cast<int>(snakeSegmentVector.size()) - 1; index > 0; index--) {
                snakeSegmentVector.at(index).setPosition(snakeSegmentVector.at(index - 1).getPosition());

                if (snakeHead.getPosition() == snakeSegmentVector.at(index).getPosition()) {
                    isDied = true;
                    // Thêm một phần tử mới để tránh bù lại bộ phận bị khi rắn cắn trúng thân, do ta không cập nhật vị trí đầu rắn sau này
                    snakeSegmentVector.emplace_back(tailPosition);
                }
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
            currentDirection = direction;

            // Cho phép đi xuyên không gian
            if (snakeHead.getPosition().x < 0.f)
                snakeHead.setPosition({maxXCells * 40.f - 20.f, snakeHead.getPosition().y});
            else if (snakeHead.getPosition().x > maxXCells * 40.f)
                snakeHead.setPosition({20.f, snakeHead.getPosition().y});
            else if (snakeHead.getPosition().y < 0.f)
                snakeHead.setPosition({snakeHead.getPosition().x, maxYCells * 40.f - 20.f});
            else if (snakeHead.getPosition().y > maxYCells * 40.f)
                snakeHead.setPosition({snakeHead.getPosition().x, 20.f});

            for (int index = 0; index < static_cast<int>(snakeSegmentVector.size()); index++)
                 visitedGrid.at(static_cast<int>(snakeSegmentVector.at(index).getPosition().y) / 40).at(static_cast<int>(snakeSegmentVector.at(index).getPosition().x) / 40) = 1;
            visitedGrid.at(static_cast<int>(snakeHead.getPosition().y) / 40).at(static_cast<int>(snakeHead.getPosition().x) / 40) = 1;
        }

        for (int index = 0; index < static_cast<int>(snakeSegmentVector.size()); index++)
            snakeSegmentVector.at(index).draw(window);

        window.draw(snakeHead);
        snakeFood.draw(window);

        window.display();
    }
    return 0;
}