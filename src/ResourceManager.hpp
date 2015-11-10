#pragma once

#include <string>
#include <array>
#include <bitset>
#include <SFML/Graphics.hpp>
#include "Pieces.hpp"


class ResourceManager {
private:
    const std::string resource_path;
    std::array<sf::Texture, 12> pieces;

public:
    ResourceManager(const std::string &resource_path);

    ResourceManager();

    //sf::Sprite é uma classe super leve então não é necessário
    //retornar por valor não impacta performance
    sf::Sprite loadPiece(const std::bitset<7> &type);

};


