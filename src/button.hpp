#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>

class Button
{
public:
    Button(const char *imagePath, Vector2 imagePosition, float scale);
    ~Button();
    void Draw() const;
    bool isPressed(const Vector2 &mousePos, const bool &mousePressed) const;

private:
    Texture2D texture;
    Vector2 position;
};

#endif