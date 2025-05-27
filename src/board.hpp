#ifndef BOARD_HPP
#define BOARD_HPP

#include "Tile.hpp"
#include <raylib.h>
#include <vector>

class Board
{
public:
	Board(int rows, int cols, int cellSize, int offsetX = 0, int offsetY = 0, int minesCount = 15);

	void Draw() const;
	void ClickedAt(const Vector2 &mousePos, bool &isGameOver);
	void Generate(const int &safeX, const int &safeY);
	void ToggleFlagAt(const int &x, const int &y);
	void Reset();
	int GetWellPlacedMines() const;
	int GetFlagsCountInBoard() const;
	Tile GetTileAtPos(const int &x, const int &y) const;
	std::pair<int, int> GetCellAtPosition(const Vector2 &mousePos) const;

private:
	int rows, cols;
	int cellSize;
	int offsetX, offsetY;
	int minesCount;
	int flagsCountInBoard = 0;
	int wellPlacedFlags = 0;
	std::vector<std::vector<Tile>> grid;

	void computeNeighborMinesCount();
	void revealEmptyTiles(const int &x, const int &y);
	void drawGridLines() const;
	void drawTiles() const;
	void drawSingleTile(const int &x, const int &y) const;
	void drawTileFlag(const int &x, const int &y) const;
	void drawTileNumber(const int &x, const int &y, const int &tileNumber) const;
	void drawTileMine(const int &x, const int &y) const;

	void printBoard() const;
};
#endif
