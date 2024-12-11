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


#define SWORD "../../assets/items/sword.png"
#define PICKAXE "../../assets/items/pickaxe.png"
#define IRONORE "../../assets/items/iron_ore.png"

#define TERRAIN_ATLAS "../../assets/terrain/test_tilemap.png"
#define TERRAIN_ATLAS "../../assets/terrain/terrain.png"


#define BACKGROUND_MUSIC "../../assets/sound/Background2.mp3"
#define WALKSOUND "../../assets/sound/WalkSound3.ogg"

#endif //DEFINITIONS_HPP
