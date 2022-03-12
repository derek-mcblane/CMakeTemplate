#include <array>
#include <bitset>
#include <cstddef>
#include <iostream>

namespace othello {

enum Color {
    NONE,
    BLACK,
    WHITE,
    MAX_COLOR
};

enum Direction {
    MIN_DIRECTION = 0,
    RIGHT         = MIN_DIRECTION,
    UP,
    LEFT,
    DOWN,
    MAX_DIRECTION
};

Direction opposite(const Direction& d);

struct Position {
    size_t row;
    size_t col;
};

namespace bit_board {

inline void display(const uint64_t& bits) {
    std::cout << std::bitset<64>(bits).to_string() << std::endl;
}

constexpr const uint64_t TOP_LEFT   = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
constexpr const uint64_t TOP_EDGE   = 0b11111111'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
constexpr const uint64_t BOT_EDGE   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'11111111;
constexpr const uint64_t LEFT_EDGE  = 0b10000000'10000000'10000000'10000000'10000000'10000000'10000000'10000000;
constexpr const uint64_t RIGHT_EDGE = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001;
constexpr const uint64_t NEG_SLOPE  = 0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;
constexpr const uint64_t POS_SLOPE  = 0b00000001'00000010'00000100'00001000'00010000'00100000'01000000'10000000;

inline constexpr uint64_t position_mask(const Position& p) {
    return (TOP_LEFT >> (p.row * 8) >> (p.col));
}

inline constexpr bool test(const uint64_t& bits, const Position& p) {
    return bits & position_mask(p);
}

inline void set(uint64_t& bits, const Position& p) {
    bits |= position_mask(p);
}

template <Direction>
bool on_edge(const uint64_t& bits);

template <Direction D>
uint64_t shift(const uint64_t& bits, const size_t& n = 1);

template <Direction D>
uint64_t dilate(const uint64_t& bits, const size_t& n = 1) {
    uint64_t dilated = bits;
    for (int i = 0; i < n; i++) {
        dilated |= shift<D>(dilated, n);
    }
    return dilated;
}

} // namespace bit_board

class GameBoard {
  public:
    using Bits = uint64_t;

    GameBoard() {
        set_up();
    }

    void set_up() {
        white = 0;
        bit_board::set(white, {3, 3});
        bit_board::set(white, {4, 4});
        black = 0;
        bit_board::set(black, {4, 3});
        bit_board::set(black, {3, 4});
    }

    Color at(const Position& p) const {
        bool is_white = bit_board::test(white, p);
        bool is_black = bit_board::test(black, p);
        if (!is_white && !is_black)
            return NONE;
        else if (is_white)
            return WHITE;
        else if (is_black)
            return BLACK;
        else
            return MAX_COLOR;
    }

    Bits vacant() const {
        return !(white | black);
    }

    Bits valid_moves(const Color& c) {
        return (directional_valid_moves_<RIGHT>(c) | directional_valid_moves_<UP>(c) |
                directional_valid_moves_<LEFT>(c) | directional_valid_moves_<DOWN>(c));
    }

    bool place_piece(Color c, const Position& p) {
        bool captured = capture_(c, p);
        if (captured) {
            pieces_(c) |= bit_board::position_mask(p);
            opposite_pieces_(c) |= ~bit_board::position_mask(p);
        }
        return captured;
    }

  private:
    template <Direction D>
    class State;

    Bits white;
    Bits black;

    Bits& pieces_(Color c) {
        return (c == WHITE) ? white : black;
    }
    const Bits& pieces_(Color c) const {
        return pieces_(c);
    }
    Bits& opposite_pieces_(Color c) {
        return (c == WHITE) ? black : white;
    }
    const Bits& opposite_pieces_(Color c) const {
        return pieces_(c);
    }

    template <Direction D>
    Bits directional_valid_moves_(const Color& c);

    template <Direction D>
    bool directional_capture_(const Color& c, const Position& p);

    bool capture_(const Color& c, const Position& p) {
        bool valid_move = false;
        valid_move |= directional_capture_<RIGHT>(c, p);
        valid_move |= directional_capture_<UP>(c, p);
        valid_move |= directional_capture_<LEFT>(c, p);
        valid_move |= directional_capture_<DOWN>(c, p);
        return valid_move;
    }

    friend std::ostream& operator<<(std::ostream& os, const GameBoard& board);
};

template <Direction D>
GameBoard::Bits GameBoard::directional_valid_moves_(const Color& c) {
    Bits moves      = 0;
    Bits candidates = opposite_pieces_(c) & bit_board::shift<D>(pieces_(c));
    while (candidates) {
        Bits shifted = bit_board::shift<D>(candidates);
        moves |= vacant() & shifted;
        candidates = opposite_pieces_(c) & shifted;
    }
    return moves;
}

template <Direction D>
bool GameBoard::directional_capture_(const Color& c, const Position& p) {
    State<D> s{c, *this, p};
    bool did_commit = false;
    while (s.should_keep_dilating()) {
        s.dilate();
    }
    if (s.should_commit()) {
        pieces_(c) |= s.bits();
        opposite_pieces_(c) &= ~s.bits();
        did_commit = true;
    }
    return did_commit;
}

template <Direction D>
class GameBoard::State {
  public:
    State(const Color& color, GameBoard& board, const Position& start)
        : my_pieces_(board.pieces_(color)), vacant_(board.vacant()), start(start) {}

    void dilate();
    bool should_commit();
    bool should_keep_dilating();
    Bits bits();

  private:
    Position start;
    Bits& my_pieces_;
    Bits vacant_;

    Bits bits_;
    bool capped;
    bool on_edge;
    bool on_empty;
};

template <Direction D>
void GameBoard::State<D>::dilate() {
    bits_ = bit_board::dilate<D>(bits_);
}

template <Direction D>
bool GameBoard::State<D>::should_commit() {
    return capped;
}

template <Direction D>
bool GameBoard::State<D>::should_keep_dilating() {
    Bits selected = ~(bit_board::position_mask(start) & bits_);
    on_edge       = on_edge<D>(bits_);
    capped        = my_pieces_ & selected;
    on_empty      = vacant_ & selected;

    return !(on_edge || capped || on_empty);
}

template <Direction D>
GameBoard::Bits GameBoard::State<D>::bits() {
    return bits_;
}

} // namespace othello
