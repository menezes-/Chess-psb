#include "Board.hpp"
#include "utils.hpp"


/*
 *  Apesar de todas as definições de classe estarem em Board.hpp, devido a um série de problemas
 *  com dependências ciclicas, a implementação das classes de estado foram colocadas neste arquivo
 *  para deixar o código mais organizado. Assim o header Board.hpp é implementado em dois locais diferentes
 *
 */



void NormalState::handleEvent(Board &board, sf::Event event) {

    if (event.mouseButton.button == sf::Mouse::Left) {

        Square *res = getClickedSquare(board, event);
        if ((res != nullptr) && (res->getPiece() != nullptr)) {
            const auto &internal_board = board.getBoard();
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
        bool win = false;
        if ((target != nullptr) && selected->getPiece()->getType()[0] == playing /*garante que é a minha vez*/) {

            //garente que eu estou movendo a peça para um lugar válido
            auto result = std::find(last_valid_positions.begin(), last_valid_positions.end(), target->getBoardPos());
            if (result != std::end(last_valid_positions)) {


                Piece *targetPiece = target->getPiece();
                if (targetPiece != nullptr) {
                    // verifica se o jogador ganhou
                    auto type = targetPiece->getType();
                    if (maskGetType(type) == KING && type[0] != playing) {
                        win = true;
                    }

                }


                // verifica promoção do peão (somente para rainha)
                // (teoricamente o usuário poderia selecionar uma peça)
                Piece *selectedPiece = selected->getPiece();
                auto stype = selectedPiece->getType();
                if (maskGetType(stype) == PAWN && (target->getBoardPos().y == 0 || target->getBoardPos().y == 7)) {

                    // brancas
                    if (!stype[0] && target->getBoardPos().y == 7) {
                        auto nt = PieceType{QUEEN};
                        selectedPiece->setType(nt);

                    } else if (stype[0] && target->getBoardPos().y == 0) {
                        auto nt = PieceType{(QUEEN | BLACK)};
                        selectedPiece->setType(nt);

                    }

                }


                // troca para o próximo jogador
                playing = !playing;
                // atualiza texto informativo
                if (playing) {
                    board.setDisplayText("Pretas Jogam");
                } else {
                    board.setDisplayText("Brancas Jogam");
                }

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


        }
        const auto &internal_board = board.getBoard();

        // Desmarca as peças previamente marcadas
        for (auto &i:last_valid_positions) {
            internal_board[getArrayPos(i.x, i.y)]->setHighlight(false);
        }

        EState to_state = NORMAL;

        if (win) {


            /*
             * o código que lida com um clique válido sempre inverte os jogadores
             * (pois se eu invertesse os jogadores aqui a pessoa poderia NÃO fazer uma jogada (ex: cliando em um quadrado
             * inválido) e o código ia mudar o jogador atual, o que seria errado) e como
             * a variável win só é setada em um código válido, podemos assumir que o cara que fez a jogada
             * é o jogador anterior, ou seja, a negação da variável playing
             *
             */
            playing = !playing;

            for (const auto &i:internal_board) {
                //marca todas as peças do jogador que ganhou
                if ((i->getPiece() != nullptr) && i->getPiece()->getType()[0] == playing) {
                    i->setHighlight(sf::Color::Green);
                }

            }

            if (playing) {
                board.setDisplayText("Pretas Ganharam!");
            } else {
                board.setDisplayText("Brancas Ganharam!");
            }

            to_state = WIN;
        }
        board.setBoardState(to_state);



    }
}

void PieceSelectedState::setLastValidPos(const std::vector<sf::Vector2i> &l_valid_positions) {
    last_valid_positions = l_valid_positions;
}

void PieceSelectedState::setSelectedPiece(Square *s) {
    selected = s;

}


Square *BoardState::getClickedSquare(Board &board, sf::Event event) const {
    Square *res = nullptr;
    if (event.mouseButton.button == sf::Mouse::Left) {


        const auto &internal_board = board.getBoard();
        for (const auto &square: internal_board) {
            if (square->getSprite().getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                res = square.get();
                break;
            }
        }

    }
    return res;

}

void WinState::handleEvent(Board &board, sf::Event event) {

}