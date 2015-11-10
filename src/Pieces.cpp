#include "Pieces.hpp"
#include "utils.hpp"

Piece::Piece(PieceType piece, ResourceManager &manager) : type{piece}, manager{manager},
                                                          sprite{manager.loadPiece(type)} {
}

const PieceType &Piece::getType() const {
    return type;
}

King::King(const PieceType &piece, ResourceManager &manager) : Piece{piece, manager} {

}

Queen::Queen(const PieceType &piece, ResourceManager &manager) : Piece{piece, manager} {

}

Bishop::Bishop(const PieceType &piece, ResourceManager &manager) : Piece{piece, manager} {

}

Knight::Knight(const PieceType &piece, ResourceManager &manager) : Piece{piece, manager} {

}

Rook::Rook(const PieceType &piece, ResourceManager &manager) : Piece{piece, manager} {

}

Pawn::Pawn(const PieceType &piece, ResourceManager &manager) : Piece{piece, manager} {

}

std::vector<sf::Vector2i> King::validPositions(const InternalBoard &board) {
    return std::vector<sf::Vector2i>();
}

std::vector<sf::Vector2i> Queen::validPositions(const InternalBoard &board) {
    return std::vector<sf::Vector2i>();
}

std::vector<sf::Vector2i> Bishop::validPositions(const InternalBoard &board) {
    return std::vector<sf::Vector2i>();
}

std::vector<sf::Vector2i> Knight::validPositions(const InternalBoard &board) {
    return std::vector<sf::Vector2i>();
}

std::vector<sf::Vector2i> Rook::validPositions(const InternalBoard &board) {
    return std::vector<sf::Vector2i>();
}

std::vector<sf::Vector2i> Pawn::validPositions(const InternalBoard &board) {
    std::vector<sf::Vector2i> valid_positions = std::vector<sf::Vector2i>{};
    //PieceType type = getType();
    int modifier = type[0] ? -1 : 1;
    auto x = board_pos.x;
    auto y = board_pos.y;

    if (!board[getArrayPos(x, y + modifier)]->getPiece()) { // se não tem nenhuma peça minha frente
        if (y != 8) {
            valid_positions.push_back(sf::Vector2i{x, y + modifier});
        }

        if (y <= 6) {
            valid_positions.push_back(
                    sf::Vector2i{x, y + (modifier) * (first_move ? 2 : 1)/*Primeira jogada*/});
        }
    }

    //diagonal esquerda
    if (x > 0) {
        Piece *target = board[getArrayPos(x - 1, y + modifier)]->getPiece();
        if (target && target->getType()[0] != type[0]) {

            // emplace back faz perfect forwarding direto pra dentro do container

            valid_positions.emplace_back(x - 1, y + modifier);
        }
    }
    //diagonal direita
    if (x < 7) {
        Piece *target = board[getArrayPos(x + 1, y + modifier)]->getPiece();
        if (target && target->getType()[0] != type[0]) {
            valid_positions.emplace_back(x + 1, y + modifier);
        }
    }


    return valid_positions;
}

void Piece::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite, states);

}

void Piece::setBoardPos(const sf::Vector2i &board_pos) {

    Piece::board_pos = board_pos;
    setSpritePosition();
}

void Piece::setBoardSize(const sf::Vector2u &u) {
    Piece::board_size = u;
    setSpritePosition();

}

void Piece::setSpritePosition() {
    sprite.setPosition(board_pos.x * (board_size.x / 8), (board_size.y / 8) * board_pos.y);

}

void Piece::moveTo(Square *square) {

    setBoardPos(square->getBoardPos());
    square->setPiece(this);


}

void Pawn::moveTo(Square *square) {
    first_move = false;
    Piece::moveTo(square);

}

sf::Vector2i Piece::getBoardPos() const {
    return board_pos;

}
