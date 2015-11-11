#include "Board.hpp"
#include "utils.hpp"
#include <algorithm>


/*
 *  Apesar de todas as definições de classe estarem em Board.hpp, devido a um série de problemas
 *  com dependências ciclicas, a implementação das classes de estado foram colocadas neste arquivo
 *  para deixar o código mais organizado. Assim o header Board.hpp é implementado em dois locais diferentes
 *
 */



void NormalState::handleEvent(Board &board, sf::Event event) {

    if (event.mouseButton.button == sf::Mouse::Left) {

        Square *res = getClickedSquare(board, event);
        if (res && res->getPiece()) {
            auto &internal_board = board.getBoard();
            std::vector<sf::Vector2i> pos = res->getPiece()->validPositions(internal_board);

            for (auto &i: pos) {
                internal_board[getArrayPos(i.x, i.y)]->setHighlight(true);
            }

            board.setBoardState(PIECE_SELECTED);
            board.pieceSelectedState->setLastValidPos(pos);
            board.pieceSelectedState->setSelectedPiece(res);
        }
    }
}

void PieceSelectedState::handleEvent(Board &board, sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        Square *target = getClickedSquare(board, event);
        if (target && selected->getPiece()->getType()[0] == playing /*garante que é a minha vez*/) {

            //garente que eu estou movendo a peça para um lugar válido
            auto result = std::find(last_valid_positions.begin(), last_valid_positions.end(), target->getBoardPos());
            if (result != std::end(last_valid_positions)) {

                // Move a peça de lugar e remove
                // a referencia do quadrado original
                selected->getPiece()->moveTo(target);
                selected->setPiece(nullptr);

                /*
                 * poderia pegar uma  const  referencia ao unique_ptr (ao invés do get), procurar ele no array de peças
                 * e setar sua posição para nullptr, o que em prática o deletaria (automáticamente). Porém como o numero de peças
                 * é sempre o mesmo não tem pq
                 */

            }

            // troca para o próximo jogador
            playing = !playing;

        }

        //limpa quadrados selecionados
        for (auto &i: board.getBoard()) {
            i->setHighlight(false);
        }

        board.setBoardState(NORMAL);


    }
}

void PieceSelectedState::setLastValidPos(const std::vector<sf::Vector2i> &last_valid_positions) {
    PieceSelectedState::last_valid_positions = last_valid_positions;
}

void PieceSelectedState::setSelectedPiece(Square *s) {
    selected = s;

}


Square *BoardState::getClickedSquare(Board &board, sf::Event event) const {
    Square *res = nullptr;
    if (event.mouseButton.button == sf::Mouse::Left) {


        auto &internal_board = board.getBoard();
        for (auto &square: internal_board) {
            if (square->getSprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                res = square.get();
                break;
            }
        }

    }
    return res;

}
