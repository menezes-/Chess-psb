#include "Pieces.hpp"
#include "utils.hpp"


unsigned long int maskGetType(PieceType type) {

    static auto mask = PieceType{
            Pieces::KING | Pieces::QUEEN | Pieces::BISHOP | Pieces::KNIGHT | Pieces::PAWN | Pieces::ROOK};

    return ((type) & mask).to_ulong();

}

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

    //precisa de {{ até o c++14 (http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3526.html)
    static std::array<std::array<int, 2>, 8> offsets = {{
                                                                {1, 0},
                                                                {0, 1},
                                                                {-1, 0},
                                                                {0, -1},
                                                                {1, 1},
                                                                {-1, 1},
                                                                {-1, -1},
                                                                {1, -1}
                                                        }};

    return Piece::computeOffsetPositions(board, board_pos, type, offsets);
}

std::vector<sf::Vector2i> Queen::getPositions(int x, int y, PieceType type, const InternalBoard &board) {
    auto valid = std::vector<sf::Vector2i>();

    auto r = Rook::getPositions(x, y, type, board);
    auto b = Bishop::getPositions(x, y, type, board);

    std::move(r.begin(), r.end(), std::back_inserter(valid));
    std::move(b.begin(), b.end(), std::back_inserter(valid));


    return valid;

}

std::vector<sf::Vector2i> Queen::validPositions(const InternalBoard &board) {
    return Queen::getPositions(board_pos.x, board_pos.y, type, board);
}


std::vector<sf::Vector2i> Bishop::getPositions(int x, int y, PieceType type, const InternalBoard &board) {
    auto valid = std::vector<sf::Vector2i>{};


    //lambdas \o/
    auto valid_pos = [&type, &valid, &board](int j, int i) -> bool {

        Piece *piece = board[getArrayPos(j, i)]->getPiece();
        if (!piece) {
            valid.push_back(sf::Vector2i{j, i});
            return false; // não da break ainda
        } else if (piece->getType()[0] != type[0]) {
            valid.push_back(sf::Vector2i{j, i});
            return true; // da break
        } else {
            return true; // da break
        }


    };

    //diagonal positiva pra baixo
    for (int j = x + 1, i = y + 1; j < 8 && i < 8; j++, i++) {

        if (valid_pos(j, i)) {
            break;
        }


    }

    // diagonal positiva pra cima
    for (int j = x - 1, i = y + 1; j > -1 && i < 8; j--, i++) {
        if (valid_pos(j, i)) {
            break;
        }

    }

    //diagonal negativa pra cima
    for (int j = x - 1, i = y - 1; j > -1 && i > -1; j--, i--) {
        if (valid_pos(j, i)) {
            break;
        }
    }

    //diagonal positiva pra baixo
    for (int j = x + 1, i = y - 1; j < 8 && i > -1; j++, i--) {
        if (valid_pos(j, i)) {
            break;
        }

    }


    return valid;
}


std::vector<sf::Vector2i> Bishop::validPositions(const InternalBoard &board) {
    return getPositions(board_pos.x, board_pos.y, type, board);
}

std::vector<sf::Vector2i> Knight::validPositions(const InternalBoard &board) {

    static std::array<std::array<int, 2>, 8> offsets = {{
                                                                {-2, 1},
                                                                {-1, 2},
                                                                {1, 2},
                                                                {2, 1},
                                                                {2, -1},
                                                                {1, -2},
                                                                {-1, -2},
                                                                {-2, -1}
                                                        }};

    return Piece::computeOffsetPositions(board, board_pos, type, offsets);
}


std::vector<sf::Vector2i> Rook::getPositions(int x, int y, PieceType type, const InternalBoard &board) {
    auto valid = std::vector<sf::Vector2i>{};
    //esse algoritmo pode ser simplificado
    // frente



    //lambdas \o/
    auto valid_pos = [&type, &valid, &board](int j, int i) -> bool {

        Piece *piece = board[getArrayPos(j, i)]->getPiece();
        if (!piece) {
            valid.push_back(sf::Vector2i{j, i});
            return false; // não da break ainda
        } else if (piece->getType()[0] != type[0]) {
            valid.push_back(sf::Vector2i{j, i});
            return true; // da break
        } else {
            return true; // da break
        }


    };


    for (int i = 1; i < 8; ++i) { // um passinho pra frente...

        auto y1 = (y + i) > 7 ? 7 : (y + i); // verifica que eu não passei das bordas

        if (valid_pos(x, y1)) break;
    }

    for (int i = 1; i < 8; ++i) { // um passinho pra trás...
        auto y1 = (y - i) < 0 ? 0 : y - i;
        if (valid_pos(x, y1)) break;

    }

    for (int i = 1; i < 8; ++i) { // girando, girando, girando pro lado
        auto x1 = (x - i) < 0 ? 0 : x - i;
        if (valid_pos(x1, y)) break;
    }

    for (int i = 1; i < 8; ++i) { // girando, girando, girando pro outro
        auto x1 = (x + i) > 7 ? 7 : x + i;
        if (valid_pos(x1, y)) break;
    }

    return valid;


}

std::vector<sf::Vector2i> Rook::validPositions(const InternalBoard &board) {
    return getPositions(board_pos.x, board_pos.y, type, board);


}

std::vector<sf::Vector2i> Pawn::validPositions(const InternalBoard &board) {
    std::vector<sf::Vector2i> valid_positions = std::vector<sf::Vector2i>{};

    /*
     *  Gambiarra para lidar com a promoção pra rainha
     *
     */

    // crise de identidade
    if (maskGetType(type) == QUEEN) {

        // se eu sou uma rainha então se comporte como uma rainha
        return Queen::getPositions(board_pos.x, board_pos.y, type, board);
    }


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

std::vector<sf::Vector2i> Piece::computeOffsetPositions(const InternalBoard &board, const sf::Vector2i &board_pos,
                                                        const PieceType &type,
                                                        std::array<std::array<int, 2>, 8> &offsets) {
    auto valid = std::vector<sf::Vector2i>();
    for (auto i: offsets) {
        // garante que eu não passe dos limites
        // das bordas
        int x = i[0] + board_pos.x;
        int y = i[1] + board_pos.y;
        // pula posições inválidas
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            continue;
        }

        Piece *piece = board[getArrayPos(x, y)]->getPiece();
        if (piece) {
            if (piece->getType()[0] != type[0]) {// se a peça contida aqui for inimiga é uma posição válida
                valid.push_back(sf::Vector2i{x, y});
            }

        } else {
            valid.push_back(sf::Vector2i{x, y});
        }


    }


    return valid;

}

void Piece::setType(const PieceType &type) {

    this->type = type;
    sprite = manager.loadPiece(type);

}