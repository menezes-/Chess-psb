#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#include "ResourceManager.hpp"
#include "Square.hpp"


/////////////////////////ESTADOS

class Board;

enum EState : short {
    NORMAL,
    PIECE_SELECTED,
    WIN,
};

class BoardState {

public:
    virtual ~BoardState() = default;

    virtual void handleEvent(Board &board, sf::Event event) { }

    Square *getClickedSquare(Board &board, sf::Event event) const;

};


class NormalState : public BoardState {

public:


    virtual void handleEvent(Board &board, sf::Event event);
};

class PieceSelectedState : public BoardState {

private:
    std::vector<sf::Vector2i> last_valid_positions;
    Square* selected;

    // lembra quem está jogando brancas (0) ou pretas (1)
    bool playing = 0;


public:
    void setLastValidPos(const std::vector<sf::Vector2i> &last_valid_positions);
    void setSelectedPiece(Square*);

    virtual void handleEvent(Board &board, sf::Event event);

};

class WinState : public BoardState {

    /*
     * Estado final
     * Ele não faz nada.
     *
     */

public:

    virtual void handleEvent(Board &board, sf::Event event);

};


//////////////////////////////////ESTADOS

using squareRef = std::unique_ptr<Square>;
using pieceRef = std::unique_ptr<Piece>;


class Board : public sf::Drawable {
private:
    ResourceManager &rm;
    std::array<squareRef, 64> board;
    std::array<pieceRef, 32> pieces;
    sf::Vector2u size;

    sf::Text display_text;



    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void initPlace(std::array<PieceType, 8> pieces, int start_pos, int black);


public:

    // estados de transição do tabuleiro
    std::unique_ptr<NormalState> normalState;
    std::unique_ptr<PieceSelectedState> pieceSelectedState;
    std::unique_ptr<WinState> winState;
    BoardState *state;

    void setBoardState(EState state);

    void setSize(const sf::Vector2u &size);

    void setDisplayText(const std::string &);

    void initBoard();

    Board(ResourceManager &rm);

    void mousePressed(sf::Event);

    const std::array<squareRef, 64> &getBoard() const;

    Piece *makePiece(PieceType type, sf::Vector2i vector2);


};