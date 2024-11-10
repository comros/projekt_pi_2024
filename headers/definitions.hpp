#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

// Needed to adjust for the player's sprite white space calculated from the middle of a sprite
#define PLAYER_WIDTH 48.0f
#define PLAYER_HEIGHT 64.0f
#define PLAYER_PADDING_UP 12.0f
#define PLAYER_PADDING_RIGHT 6.0f
#define PLAYER_PADDING_DOWN 11.0f
#define PLAYER_PADDING_LEFT 5.0f

#define WINDOW_WIDTH 1920.0f
#define WINDOW_HEIGHT 1080.0f

// Project build is in ./cmake-build-debug/bin, so in order to access the assets folder "../../" is needed
#define PLAYER_WALK_ATLAS "../../assets/player/Walk/walk.png"
#define PLAYER_IDLE_ATLAS "../../assets/player/Idle/idle.png"

#define TERRAIN_ATLAS "../../assets/terrain/test_tilemap.png"

#define BACKGROUND_MUSIC "../../assets/sound/Background.ogg"
#define STEPSOUND "../../assets/sound/StepSound.ogg"

#endif //DEFINITIONS_HPP
