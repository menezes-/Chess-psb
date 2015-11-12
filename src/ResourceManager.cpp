#include <sstream>
#include "ResourceManager.hpp"

enum Pieces : short;

ResourceManager::ResourceManager(const std::string &resource_path) : resource_path{resource_path} {
    for (int i = 0; i < 12; ++i) {
        std::ostringstream stringStream;
        stringStream << resource_path << i << ".png";
        sf::Texture t;

        bool result = t.loadFromFile(stringStream.str());
        if (result) {
            pieces[i] = t;
        }


    }
    // carega a fonte
    std::ostringstream font_path;
    font_path << resource_path << "Calibri.ttf";
    font.loadFromFile(font_path.str());


}


ResourceManager::ResourceManager() : ResourceManager{"resources/"} {


}

sf::Sprite ResourceManager::loadPiece(const std::bitset<7> &type) {

    int index = 0;

    auto out = ((type.to_ulong()) &
                (Pieces::KING | Pieces::QUEEN | Pieces::BISHOP | Pieces::KNIGHT | Pieces::PAWN | Pieces::ROOK));
    switch (out) {
        case Pieces::KING:
            index = 0;
            break;
        case Pieces::QUEEN:
            index = 1;
            break;
        case Pieces::BISHOP:
            index = 2;
            break;
        case Pieces::KNIGHT:
            index = 3;
            break;
        case Pieces::ROOK:
            index = 4;
            break;
        case Pieces::PAWN:
            index = 5;
            break;
        default:
            break;
    }

    if (type[0]) {//black
        index += 6;
    }
    sf::Sprite sprite;
    sprite.setTexture(pieces[index]);

    return sprite;

}

const sf::Font &ResourceManager::getFont() const {
    return font;
}
