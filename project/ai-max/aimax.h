#pragma once

#include "board.h"

#include <limits>
#include <optional>

constexpr const int INT_NEG_INF = std::numeric_limits<int>::min();

namespace othello {
namespace AIMax {

template <Color color>
int evaluate(const GameBoard& board) {
    GameBoard::Bits& my_pieces  = board.tpieces_<color>();
    GameBoard::Bits& opp_pieces = board.tpieces_<opposite<color>()>();
    return (bit_board::count(my_pieces) - bit_board::count(opp_pieces));
}

struct State {
    Position move;
    int value;
};

template <Color color>
std::optional<State> _best_move_inner(const GameBoard& board, size_t depth = 0) {
    std::vector<Position> potential_moves = board.valid_moves(color);
    if (potential_moves.empty() || depth == 0)
        return {};

    State current;
    current.value = INT_NEG_INF;
    for (auto move : potential_moves) {
        GameBoard next_board{board};
        next_board.place_piece(color, move);
        auto next = _best_move_inner<opposite<color>>(next_board, depth - 1);
        if (!next) {
            current.value = evaluate<color>(next_board);
        } else {
            next.value *= -1;
            if (next->value > current.value) {
                current.value = next->value;
                current.move  = move;
            }
        }
    }
    return current;
}

template <Color color>
std::optional<Position> best_move(const GameBoard& board) {
    if (auto state = _best_move_inner<color>(board, 5))
        return state->move;
    else
        return {};
}

} // namespace AIMax
} // namespace othello
