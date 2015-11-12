#include <algorithm>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Board.hpp"

using namespace std;

int main(int argc, char **argv) {
    auto height = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode(600, std::min(height, 615u)), "Chess PSB",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(true);
    window.setKeyRepeatEnabled(false);
    ResourceManager rm{};
    Board board{rm};
    board.setSize(sf::Vector2u{600, 600});
    board.initBoard();


    sf::Clock clock;
    /*
     * Esse código só roda se o programa for compilado em modo de bug
     * ele exibe um texto do lado do ponteiro do mouse mostrando a posição
     * atual do mouse (util para ter noção de onde posicionar os objetos na tela)
     */
#ifndef NDEBUG
    sf::Text debug;
    debug.setFont(rm.getFont());
    debug.setColor(sf::Color::Red);
    debug.setCharacterSize(12);
    std::ostringstream mousePos;

#endif
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
                    board.handleEvent(event);
                    break;
                case sf::Event::KeyPressed:
                    board.handleEvent(event);
                    break;
#ifndef NDEBUG
                case sf::Event::MouseMoved:
                    mousePos.str("");
                    mousePos << '(' << event.mouseMove.x << ',' << event.mouseMove.y << ')';
                    debug.setString(mousePos.str());
                    debug.setPosition(event.mouseMove.x + 12, event.mouseMove.y);
                    break;
#endif

                default:
                    break;
            }

        }
        sf::Time elapsed = clock.restart();
        board.update(elapsed);
        window.clear(sf::Color::White);
        window.draw(board);
#ifndef NDEBUG
        window.draw(debug);
#endif
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

