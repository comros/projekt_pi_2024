#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define SPRITE_SCALE 5

// Needed to adjust for the player's sprite white space, remember sprite's origin is in the middle: (32/2, 32/2)
#define SPRITE_SIZE_X 48 // NOTE: SPRITE_SIZE SHOULD BE DIVISIBLE BY 2
#define SPRITE_SIZE_Y 64
#define SPRITE_PADDING_UP (12 * SPRITE_SCALE)
#define SPRITE_PADDING_RIGHT (6 * SPRITE_SCALE)
#define SPRITE_PADDING_DOWN (11 * SPRITE_SCALE)
#define SPRITE_PADDING_LEFT (5 * SPRITE_SCALE)

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

// Project build is in ./cmake-build-debug/bin, so in order to access the assets folder "../../" is needed
#define P_WALK_ATLAS "../../assets/player/Walk/walk.png"
#define P_IDLE_ATLAS "../../assets/player/Idle/idle.png"

#endif //DEFINITIONS_HPP
