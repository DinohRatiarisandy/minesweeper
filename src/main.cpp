#include <raylib.h>
#include "board.hpp"
#include <bits/stdc++.h>
#include "button.hpp"

int main()
{
    // Initialisation
    const int WIND_WIDTH = 500;
    const int WIND_HEIGHT = 500;
    InitWindow(WIND_WIDTH, WIND_HEIGHT, "Minesweeper");

    const int CELL_SIZE = 40;
    const int ROWS = 10;
    const int COLS = 10;
    const int OFFSET_X = 50;
    const int OFFSET_Y = 80;
    const int MINES_COUNT = 20;

    Color BG_COLOR = {160, 160, 160, 255};
    bool isFirstClick = true;
    bool isGameOver = false;
    bool isWin = false;

    SetTargetFPS(30);

    // Buttons
    Board board(ROWS, COLS, CELL_SIZE, OFFSET_X, OFFSET_Y, MINES_COUNT);
    Button startButton("assets/start.png", Vector2({50, 30}), 0.6);

    // Texture for flag
    Image flagImg = LoadImage("assets/white_flag.png");
    Image mineImg = LoadImage("assets/mine.png");
    Tile::flagTexture = LoadTextureFromImage(flagImg);
    Tile::mineTexture = LoadTextureFromImage(mineImg);
    UnloadImage(flagImg);
    UnloadImage(mineImg);

    // Game loop
    while (!WindowShouldClose())
    {
        // Event Handling
        // cliquer sur le button start (start un nouveau jeu)
        if (startButton.isPressed(GetMousePosition(), IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            board.Reset();
            isFirstClick = true;
            isGameOver = false;
            isWin = false;
        };

        if (isGameOver == false && isWin == false)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // afficher la tuile cliquée
            {
                const Vector2 &mousePos = GetMousePosition();

                if (isFirstClick)
                {
                    const auto &[safeX, safeY] = board.GetCellAtPosition(mousePos);
                    if (safeX != -1 && safeY != -1)
                    {
                        if (!board.GetTileAtPos(safeX, safeY).isFlagged)
                        {
                            board.Generate(safeX, safeY);
                            isFirstClick = false;
                        }
                    }
                }
                else
                {
                    board.ClickedAt(mousePos, isGameOver);
                }
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) // mettre la tuille selectionnée un drapeau
            {
                const Vector2 &mousePos = GetMousePosition();
                const auto &[x, y] = board.GetCellAtPosition(mousePos);
                if (x != -1 && y != -1)
                {
                    board.ToggleFlagAt(x, y);

                    if (board.GetWellPlacedMines() == MINES_COUNT)
                    {
                        std::cout << "YOU WIN ! \n";
                        isWin = true;
                    }
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BG_COLOR);

        board.Draw();
        startButton.Draw();
        DrawText(TextFormat("Mines: %d / %d", board.GetFlagsCountInBoard(), MINES_COUNT), 300, 40, 20, RED);

        EndDrawing();
    }

    UnloadTexture(Tile::flagTexture);
    UnloadTexture(Tile::mineTexture);
    CloseWindow();
    return 0;
}
