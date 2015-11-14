#include <vector>
#include "Board.hpp"
#include "utils.hpp"


Board::Board(ResourceManager &rm) : rm{rm} {

    normalState = std::unique_ptr<NormalState>(new NormalState);
    pieceSelectedState = std::unique_ptr<PieceSelectedState>(new PieceSelectedState);
    winState = std::unique_ptr<WinState>(new WinState);
    state = normalState.get();
    display_text.setFont(rm.getFont());
    display_text.setCharacterSize(12);
    display_text.setPosition(3, 600);
    display_text.setColor(sf::Color::Black);
    display_text.setString("Brancas Jogam");



}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    for (auto &s: board) {
        s->draw(target, states);
    }

    target.draw(display_text);

}

void Board::setSize(const sf::Vector2u &size) {
    this->size = size;
    for (auto &ptr: board) {
        if (ptr) {
            ptr->setBoardSize(size);
        }

    }


}

// array não é passado por referencia pois eu quero que seja feita uma cópia
// assim só preciso criar um único array
void Board::initPlace(std::array<PieceType, 8> pieces, int start_row, int black) {
    for (int i = 0; i < 8; ++i) {
        sf::Vector2i board_pos{i, start_row};
        PieceType temp = pieces[i];
        temp[0] = black;

        Piece *piece = makePiece(temp, board_pos);

        board[getArrayPos(i, start_row)] = std::unique_ptr<Square>(new Square{board_pos, size, piece});

    }
}


void Board::initBoard() {


    std::array<PieceType, 8> pieces = {
            PieceType(Pieces::ROOK),
            PieceType(Pieces::KNIGHT),
            PieceType(Pieces::BISHOP),
            PieceType(Pieces::QUEEN),
            PieceType(Pieces::KING),
            PieceType(Pieces::BISHOP),
            PieceType(Pieces::KNIGHT),
            PieceType(Pieces::ROOK),
    };


    std::array<PieceType, 8> pawns = {
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
            PieceType(Pieces::PAWN),
    };

    initPlace(pieces, 0, 0);
    initPlace(pawns, 1, 0);

    initPlace(pieces, 7, 1);
    initPlace(pawns, 6, 1);

    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            sf::Vector2i board_pos{j, i};
            board[getArrayPos(j, i)] = std::unique_ptr<Square>(new Square{board_pos, size});
        }

    }

}

void Board::setBoardState(EState state) {

    switch (state) {

        case NORMAL:
            Board::state = normalState.get();
            break;
        case PIECE_SELECTED:
            Board::state = pieceSelectedState.get();
            break;
        case WIN:
            Board::state = winState.get();
            break;
    }

}




void Board::mousePressed(sf::Event event) {


    state->handleEvent(*this, event);


}

Piece *Board::makePiece(PieceType type, sf::Vector2i board_pos) {

    static int last_pos = 0;

    std::unique_ptr<Piece> piece;

    auto out = maskGetType(type);
    switch (out) {
        case Pieces::KING:
            piece = std::unique_ptr<Piece>{new King{type, rm}};
            break;
        case Pieces::QUEEN:
            piece = std::unique_ptr<Piece>{new Queen{type, rm}};
            break;
        case Pieces::BISHOP:
            piece = std::unique_ptr<Piece>{new Bishop{type, rm}};
            break;
        case Pieces::KNIGHT:
            piece = std::unique_ptr<Piece>{new Knight{type, rm}};
            break;
        case Pieces::ROOK:
            piece = std::unique_ptr<Piece>{new Rook{type, rm}};
            break;
        case Pieces::PAWN:
            piece = std::unique_ptr<Piece>{new Pawn{type, rm}};
            break;
    }
    piece->setBoardPos(board_pos);
    Piece *tmp = piece.get();
    pieces[last_pos++] = std::move(piece);
    return tmp;

}


const std::array<squareRef, 64> &Board::getBoard() const {
    return board;
}

void Board::setDisplayText(const std::string &string) {
    display_text.setString(string);

}
