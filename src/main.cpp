#include <algorithm>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Board.hpp"

using namespace std;

int main(int argc, char **argv) {
    auto height = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode(600, std::min(height, 600u)), "Chess PSB",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    ResourceManager rm{};
    Board board{rm};
    board.setSize(window.getSize());
    board.initBoard();


    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {

                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    board.setSize(window.getSize());
                    break;
                case sf::Event::MouseButtonPressed:
                    board.mousePressed(event);
                    break;

                default:
                    break;
            }

        }
        sf::Time elapsed = clock.restart();
        board.update(elapsed);
        window.clear(sf::Color::White);
        window.draw(board);
        window.display();
    }

    return 0;
}


#if defined(_WIN32) && defined(NDEBUG)
#include <windows.h>

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {
    return main(__argc, __argv);
}

#endif

