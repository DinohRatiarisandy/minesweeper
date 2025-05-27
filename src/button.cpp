#include "button.hpp"
#include <raylib.h>

Button::Button(const char *imagePath, Vector2 imagePosition, float scale)
{
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = imagePosition;
}

Button::~Button()
{
    UnloadTexture(texture);
}

void Button::Draw() const
{
    DrawTextureV(texture, position, WHITE);
}

bool Button::isPressed(const Vector2 &mousePos, const bool &mousePressed) const
{
    const Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};

    if (CheckCollisionPointRec(mousePos, rect) && mousePressed)
    {
        return true;
    }

    return false;
}
