#include <ostream>

enum ColourCode {
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_YELLOW   = 33,
    FG_MAGENTA  = 35,
    FG_CYAN     = 36,
    FG_WHITE    = 37,
    FG_DEFAULT  = 39,
    BG_RED      = 41,
    BG_GREEN    = 42,
    BG_BLUE     = 44,
    BG_YELLOW   = 43,
    BG_MAGENTA  = 45,
    BG_CYAN     = 46,
    BG_WHITE    = 47,
};

class Modifier {
    ColourCode colour;
public:
    Modifier(ColourCode colour) : colour(colour) {}
    friend std::ostream& operator<<(std::ostream& os, const Modifier& modifier) {
        return os << "\033[" << modifier.colour << "m";
    }
};