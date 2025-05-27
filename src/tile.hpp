#ifndef TILE_HPP
#define TILE_HPP
#include <raylib.h>

class Tile
{
public:
    bool isRevealed = false;
    bool isMine = false;
    bool isFlagged = false;
    int neighborMinesCount = 0;
    static Texture2D flagTexture;
    static Texture2D mineTexture;

    void Reveal();
    void ToggleFlag();
};

#endif