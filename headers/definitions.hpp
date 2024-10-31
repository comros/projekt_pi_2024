#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#define SPRITE_SCALE 6

// Needed to adjust for the player's sprite white space, remember sprite's origin is in the middle: (32/2, 32/2)
#define SPRITE_SIZE 32 // NOTE: SPRITE_SIZE SHOULD BE DIVISIBLE BY 2
#define SPRITE_PADDING_UP (11 * SPRITE_SCALE)
#define SPRITE_PADDING_RIGHT (6 * SPRITE_SCALE)
#define SPRITE_PADDING_DOWN (9 * SPRITE_SCALE)
#define SPRITE_PADDING_LEFT (6 * SPRITE_SCALE)

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

// Project build is in ./cmake-build-debug/bin, so in order to access the assets folder "../../" is needed
#define CHAR_ATLAS_PATH "../../assets/fantasy_txtpack/Player/Player.png"

#endif //DEFINITIONS_HPP
