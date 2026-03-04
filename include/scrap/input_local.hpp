#include <stdint.h>

enum class InputAction {
    PRESS,
    LEFT,
    RIGHT
};

struct event_input {
    InputAction action;
    uint32_t timestamp;
};

