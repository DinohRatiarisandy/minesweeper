#include "board.hpp"
#include <iostream>
#include <raylib.h>
#include <array>
#include <vector>
#include <chrono>
#include <set>

Board::Board(int rows, int cols, int cellSize, int offsetX, int offsetY, int minesCount) : rows(rows), cols(cols), cellSize(cellSize), offsetX(offsetX), offsetY(offsetY), minesCount(minesCount)
{
	grid.resize(rows, std::vector<Tile>(cols));
}

void Board::Draw() const
{
	drawGridLines();
	drawTiles();
}

void Board::Generate(const int &safeX, const int &safeY)
{
	std::vector<std::pair<int, int>> positions;

	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < cols; y++)
		{
			// Exclure la zone de sécurité autour du premier clic
			if (abs(x - safeX) <= 1 && abs(y - safeY) <= 1)
				continue;

			positions.push_back({x, y});
		}
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	SetRandomSeed(seed);

	// Placer les mines au nombre de mineCount
	int minesToPlace = minesCount;
	std::set<int> seen;
	while (minesToPlace)
	{
		int randPos = GetRandomValue(0, positions.size() - 1);
		if (!seen.count(randPos))
		{
			const int x = positions[randPos].first;
			const int y = positions[randPos].second;

			if ((x < rows && x >= 0) && (y < cols && y >= 0))
			{
				grid[x][y].isMine = true;
				seen.emplace(randPos);
				minesToPlace -= 1;
			}
		}
	}

	computeNeighborMinesCount();
	revealEmptyTiles(safeX, safeY);
}

void Board::ToggleFlagAt(const int &x, const int &y)
{
	Tile &tile = grid[x][y];

	if (tile.isRevealed)
		return;

	tile.ToggleFlag();

	if (tile.isFlagged)
	{
		flagsCountInBoard += 1;
	}
	else
	{
		flagsCountInBoard -= 1;
	}

	if (tile.isMine && tile.isFlagged)
	{
		wellPlacedFlags += 1;
	}
	else if (tile.isMine && !tile.isFlagged)
	{
		wellPlacedFlags -= 1;
	}
}

void Board::Reset()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j].isRevealed = false;
			grid[i][j].isMine = false;
			grid[i][j].isFlagged = false;
			grid[i][j].neighborMinesCount = 0;
		}
	}
	wellPlacedFlags = 0;
	flagsCountInBoard = 0;
}

std::pair<int, int> Board::GetCellAtPosition(const Vector2 &mousePos) const
{
	if (
		(mousePos.x >= offsetX &&
		 mousePos.x < offsetX + cols * cellSize) &&
		(mousePos.y >= offsetY &&
		 mousePos.y < offsetY + rows * cellSize))
	{
		int col = (mousePos.x - offsetX) / cellSize;
		int row = (mousePos.y - offsetY) / cellSize;

		if ((row >= 0 && row < rows) && (col >= 0 && col < cols))
		{
			return {row, col};
		}
	}

	return {-1, -1};
}

int Board::GetFlagsCountInBoard() const
{
	return flagsCountInBoard;
}

Tile Board::GetTileAtPos(const int &x, const int &y) const
{
	return grid[x][y];
}

void Board::ClickedAt(const Vector2 &mousePos, bool &isGameOver)
{
	const auto &[x, y] = GetCellAtPosition(mousePos);
	Tile &tile = grid[x][y];

	if (x != -1 and y != -1)
	{
		if (tile.isFlagged)
			return;
		if (tile.isMine)
		{
			tile.Reveal();
			isGameOver = true;
			return;
		}
		else // If not mine
		{
			if (tile.neighborMinesCount)
			{
				tile.isRevealed = true;
			}
			else
			{
				// Si la case est vide faire le DFS pour propager l'affichage
				// des cases vide
				revealEmptyTiles(x, y);
			}
		}
	}
}

void Board::computeNeighborMinesCount()
{
	const std::array<std::pair<int, int>, 8> directions = {{{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};

	for (int cx = 0; cx < rows; cx++)
	{
		for (int cy = 0; cy < cols; cy++)
		{
			for (auto &[dX, dY] : directions)
			{
				const int neigX = cx + dX;
				const int neigY = cy + dY;
				if (((0 <= neigX && neigX < rows) && (0 <= neigY && neigY < cols)) and grid[neigX][neigY].isMine)
				{
					grid[cx][cy].neighborMinesCount += 1;
				}
			}
		}
	}
}

void Board::revealEmptyTiles(const int &x, const int &y)
{
	// Faire le DFS si la case cliquée est vide
	std::vector<std::pair<int, int>> toReveal;
	toReveal.push_back({x, y});

	while (!toReveal.empty())
	{
		auto [cx, cy] = toReveal.back();
		toReveal.pop_back();

		if (grid[cx][cy].isRevealed || grid[cx][cy].isFlagged)
			continue;

		grid[cx][cy].isRevealed = true;

		if (grid[cx][cy].neighborMinesCount == 0 && !grid[cx][cy].isMine)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				for (int dy = -1; dy <= 1; dy++)
				{
					if (dx != 0 || dy != 0)
					{
						int newX = cx + dx;
						int newY = cy + dy;
						if ((newX < rows && newX >= 0) && (newY < cols && newY >= 0))
							toReveal.push_back({newX, newY});
					}
				}
			}
		}
	}
}

void Board::drawGridLines() const
{
	// Dessiner les lignes
	for (int i = 0; i <= rows; i++)
	{
		DrawLine(
			offsetX,
			offsetY + i * cellSize,
			offsetX + cols * cellSize,
			offsetY + i * cellSize,
			GRAY);
	}

	// Dessiner les colonnes
	for (int j = 0; j <= cols; j++)
	{
		DrawLine(
			offsetX + j * cellSize,
			offsetY,
			offsetX + j * cellSize,
			offsetY + rows * cellSize,
			GRAY);
	}
}

void Board::drawTiles() const
{
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			drawSingleTile(x, y);
		}
	}
}

void Board::drawSingleTile(const int &x, const int &y) const
{
	const Tile &tile = grid[y][x];
	const int pX = offsetX + x * cellSize;
	const int pY = offsetY + y * cellSize;

	if (tile.isFlagged)
	{
		drawTileFlag(pX, pY);
	}
	else if (tile.isRevealed)
	{
		if (tile.isMine)
		{
			drawTileMine(pX, pY);
		}
		else
		{
			drawTileNumber(pX, pY, tile.neighborMinesCount);
		}
	}
	else
	{
		DrawRectangle(pX + 1, pY + 1, cellSize - 2, cellSize - 2, Color{200, 200, 200, 80});
	}
}

void Board::drawTileFlag(const int &x, const int &y) const
{
	Vector2 position = {static_cast<float>(x), static_cast<float>(y)};

	DrawTextureV(grid[x][y].flagTexture, position, WHITE);
}

void Board::drawTileNumber(const int &x, const int &y, const int &tileNumber) const
{
	DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, Color{160, 160, 160, 255});
	if (tileNumber)
	{
		DrawText(
			TextFormat("%d", tileNumber),
			x + cellSize / 3,
			y + cellSize / 3,
			20,
			DARKGRAY);
	}
}

void Board::drawTileMine(const int &x, const int &y) const
{
	Vector2 position = {static_cast<float>(x), static_cast<float>(y)};
	DrawRectangle(x + 1, y + 1, cellSize - 2, cellSize - 2, Color{224, 54, 69, 75});
	DrawTextureV(grid[x][y].mineTexture, position, WHITE);
}

int Board::GetWellPlacedMines() const
{
	return wellPlacedFlags;
}

void Board::printBoard() const
{
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < cols; y++)
		{
			if (grid[x][y].neighborMinesCount > 0 and !grid[x][y].isMine)
				std::cout << grid[x][y].neighborMinesCount << "  ";
			else if (grid[x][y].isMine)
				std::cout << "*  ";
			else
				std::cout << ".  ";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}
