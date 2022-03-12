#include "board.h"

namespace othello {

Direction opposite(const Direction& d) {
    switch (d) {
    case RIGHT:
        return LEFT;
    case UP:
        return DOWN;
    case LEFT:
        return RIGHT;
    case DOWN:
        return UP;
    default:
        return MAX_DIRECTION;
    }
}

namespace bit_board {

template <>
bool on_edge<UP>(const uint64_t& bits) {
    return bits & TOP_EDGE;
}
template <>
bool on_edge<DOWN>(const uint64_t& bits) {
    return bits & BOT_EDGE;
}
template <>
bool on_edge<LEFT>(const uint64_t& bits) {
    return bits & LEFT_EDGE;
}
template <>
bool on_edge<RIGHT>(const uint64_t& bits) {
    return bits & RIGHT_EDGE;
}

template <>
uint64_t shift<UP>(const uint64_t& bits, const size_t& n) {
    return bits << (8 * n);
}
template <>
uint64_t shift<DOWN>(const uint64_t& bits, const size_t& n) {
    return bits >> (8 * n);
}
template <>
uint64_t shift<RIGHT>(const uint64_t& bits, const size_t& n) {
    uint64_t shifted = bits;
    uint64_t wall    = 0;
    for (int i = 0; i < n; i++)
        wall |= (LEFT_EDGE >> i);
    wall = ~wall;
    shifted >>= (1 * n);
    shifted &= wall;
    return shifted;
}
template <>
uint64_t shift<LEFT>(const uint64_t& bits, const size_t& n) {
    uint64_t shifted = bits;
    uint64_t wall    = 0;
    for (int i = 0; i < n; i++)
        wall |= (RIGHT_EDGE << i);
    wall = ~wall;
    shifted <<= (1 * n);
    shifted &= wall;
    return shifted;
}

} // namespace bit_board

std::ostream& operator<<(std::ostream& os, const GameBoard& board) {
    os << "\n";
    Position p;
    for (size_t row = 0; row < 8; row++) {
        for (size_t col = 0; col < 8; col++) {
            p.row   = row;
            p.col   = col;
            Color c = board.at(p);
            switch (c) {
            case NONE:
                os << "#";
                break;
            case WHITE:
                os << "W";
                break;
            case BLACK:
                os << "B";
                break;
            case MAX_COLOR:
                os << "X";
                break;
            default:
                os << "X";
                break;
            }
        }
        os << "\n";
    }
    return os;
}

} // namespace othello
