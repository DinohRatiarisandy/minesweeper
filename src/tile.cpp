#include "tile.hpp"

Texture2D Tile::flagTexture;
Texture2D Tile::mineTexture;

void Tile::Reveal()
{
    isRevealed = true;
}

void Tile::ToggleFlag()
{
    isFlagged = !isFlagged;
}