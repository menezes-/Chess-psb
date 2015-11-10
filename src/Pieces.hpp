#pragma once

#include <bitset>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "Square.hpp"


class ResourceManager;

class Square;

enum Pieces : short {
    BLACK = 1, //modificador pra peças pretas
    KING = 2,
    QUEEN = 4,
    BISHOP = 6,
    KNIGHT = 8,
    ROOK = 10,
    PAWN = 12,
};


//desde o novo padrão using é considerado melhor que typedef

// 6 bits representam o tipo da peça e um modificador pra branco e preto
// o LSB represneta a cor, o resto representa as peças
using PieceType = std::bitset<7>;

using InternalBoard = std::array<std::unique_ptr<Square>, 64>;

class Piece : public sf::Drawable {

protected:
    PieceType type;

    ResourceManager &manager;
    sf::Sprite sprite;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


protected:
    sf::Vector2i board_pos;
    sf::Vector2u board_size;


public:

    sf::Vector2i getBoardPos() const;

    void setBoardPos(const sf::Vector2i &board_pos);

    void setBoardSize(const sf::Vector2u &);

    void setSpritePosition();

    Piece(PieceType piece, ResourceManager &manager);

    const PieceType &getType() const;

    virtual void moveTo(Square *);


    virtual std::vector<sf::Vector2i> validPositions(const InternalBoard &) = 0;

    // permite que o destruidor da classe derivada seja chamado a partir de um ponteiro
    // para a classe pai
    virtual ~Piece() = default;


};


class King final : public Piece {

public:
    King(const PieceType &piece, ResourceManager &manager);

    std::vector<sf::Vector2i> validPositions(const InternalBoard &);

};


class Queen final : public Piece {

public:
    Queen(const PieceType &piece, ResourceManager &manager);

    std::vector<sf::Vector2i> validPositions(const InternalBoard &);
};


class Bishop final : public Piece {

public:
    Bishop(const PieceType &piece, ResourceManager &manager);

    std::vector<sf::Vector2i> validPositions(const InternalBoard &);
};

class Knight final : public Piece {

public:
    Knight(const PieceType &piece, ResourceManager &manager);

    std::vector<sf::Vector2i> validPositions(const InternalBoard &);
};

class Rook final : public Piece {

public:
    Rook(const PieceType &piece, ResourceManager &manager);

    std::vector<sf::Vector2i> validPositions(const InternalBoard &);
};

class Pawn final : public Piece {

private:
    bool first_move = true;

public:
    Pawn(const PieceType &piece, ResourceManager &manager);

    virtual void moveTo(Square *);

    std::vector<sf::Vector2i> validPositions(const InternalBoard &);
};