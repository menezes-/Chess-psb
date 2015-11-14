#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Pieces.hpp"

class Piece;


class Square : public sf::Drawable {


private:


    Piece *piece = nullptr;
    sf::Vector2i board_pos;
    sf::Vector2u board_size;
    sf::RectangleShape sprite;

    sf::Color highlightColor = sf::Color::Yellow;


    void setSpritePosition();


public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    ~Square() = default;


    Piece *getPiece() const;

    void setPiece(Piece *);

    void setBoardSize(const sf::Vector2u &board_size);

    const sf::RectangleShape &getSprite() const;

    sf::Vector2i getBoardPos() const;

    void setHighlight(const sf::Color &);
    void setHighlight(bool v);

    Square(const sf::Vector2i &board_pos, const sf::Vector2u &board_size);

    Square(const sf::Vector2i &board_pos, const sf::Vector2u &board_size, Piece *);

};


