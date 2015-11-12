#include "Square.hpp"


Square::Square(const sf::Vector2i &board_pos, const sf::Vector2u &board_size) : Square(board_pos, board_size, nullptr) {
}

Square::Square(const sf::Vector2i &board_pos, const sf::Vector2u &board_size, Piece *piece) : piece{piece},
                                                                                              board_pos{board_pos} {
    setBoardSize(board_size);
}

Piece *Square::getPiece() const {
    return piece;
}


void Square::setBoardSize(const sf::Vector2u &board_size) {
    Square::board_size = board_size;
    //redimensiona o quadrado

    sf::Color color;
    if (board_pos.y % 2 == 0) {
        color = (board_pos.x % 2) == 0 ? sf::Color{229, 229, 229}/*WHITE*/ : sf::Color{127, 127, 127}/*BLACK*/;
    } else {
        color = (board_pos.x % 2) == 0 ? sf::Color{127, 127, 127} : sf::Color{229, 229, 229};
    }
    sprite.setSize(sf::Vector2f{board_size.x / 8.0f, board_size.y / 8.0f});
    sprite.setFillColor(color);

    // caso eu receba uma atualização de tamanho, e tem uma peça no meu quadrado, propaga o evento para a peça
    if (piece) {

        piece->setBoardSize(board_size);
    }

    setSpritePosition();


}

void Square::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite, states);
    if (piece) {
        target.draw(*piece, states);
    }
}

void Square::setPiece(Piece *ptr) {

    piece = ptr;


}

void Square::setSpritePosition() {

    sprite.setPosition(board_pos.x * (board_size.x / 8), (board_size.y / 8) * board_pos.y);

}

const sf::RectangleShape &Square::getSprite() const {
    return sprite;
}

sf::Vector2i Square::getBoardPos() const {
    return board_pos;
}

void Square::setHighlight(bool v) {
    if (v) {
        sprite.setOutlineColor(sf::Color::Yellow);
        sprite.setOutlineThickness(-3);
    } else {
        sprite.setOutlineThickness(0);
    }

}


