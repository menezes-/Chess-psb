#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <sstream>
#include <utility>
#include "ResourceManager.hpp"
#include "Square.hpp"


/////////////////////////ESTADOS

class Board;

enum EState : short {
    NORMAL,
    MOVING_PIECE,
    PIECE_SELECTED,
    WIN,
};

class BoardState {

public:
    virtual ~BoardState() = default;

    virtual void handleEvent(Board &board, sf::Event event) { }

    Square *getClickedSquare(Board &board, sf::Event event) const;

    virtual void update(Board &board, sf::Time) { };


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
public:
    void setLastValidPos(const std::vector<sf::Vector2i> &last_valid_positions);
    void setSelectedPiece(Square*);

    virtual void handleEvent(Board &board, sf::Event event);

};


//////////////////////////////////ESTADOS

using squareRef = std::unique_ptr<Square>;
using pieceRef = std::unique_ptr<Piece>;
using his_item = std::pair<Square *, Piece *>;


class Board : public sf::Drawable {
private:
    ResourceManager &rm;
    std::array<squareRef, 64> board;
    std::array<pieceRef, 32> pieces;
    sf::Vector2u size;

    sf::Text display_text;

    std::vector<his_item> history;



    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void initPlace(std::array<PieceType, 8> pieces, int start_pos, int black);


public:

    // estados de transição do tabuleiro
    std::unique_ptr<NormalState> normalState;
    std::unique_ptr<PieceSelectedState> pieceSelectedState;
    BoardState *state;

    void setBoardState(EState state);

    void setSize(const sf::Vector2u &size);

    void setDisplayText(const std::string &);

    void initBoard();

    Board(ResourceManager &rm);

    void handleEvent(sf::Event);

    void update(sf::Time);

    const std::array<squareRef, 64> &getBoard() const;

    Piece *makePiece(PieceType type, sf::Vector2i vector2);

    void restore();

    void addHistory(Square *, Piece *);


};