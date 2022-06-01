#pragma once
#include <vector>
#include <fstream>
#include <unordered_map>
#include <tuple>
#include <SFML/Graphics.hpp>
#include "Random.h"
#include "TextureManager.h"
using std::vector;
using std::ifstream;
using std::unordered_map;


class Board
{
	struct Tile {
		enum class Texture {hidden, revealed, flag, number, bomb};

		bool hasBomb;
		Texture currentTexture;
		sf::Sprite sprite;
		sf::Sprite flagSprite;

		Tile() : hasBomb(false) , currentTexture(Texture::hidden)
		{
			this->sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
		};
		Tile(bool hasBomb);
	};

	int bombs;
	bool debugMode = false;
	bool alive = true;
	sf::Sprite face;
	vector<vector<Tile>> board;
	unordered_map<int, sf::Sprite> sprites;
	unordered_map<int, sf::Sprite> debugSprites; //I added this after the fact because I didn't know you could have a bomb on the same square as a flag

	void boom();
	void resetBoard();
	void retainDebugState();
	void clearTile(int row, int col);

public:
	unsigned int cols;
	unsigned int rows;
	void loadBoard(ifstream& file);
	void loadCFG(ifstream& file);

	void drawBoard(sf::RenderWindow& window);
	void toggleDebug();
	void handleClick(sf::Event event);
	void checkWin();
	int badNeighbors(int row, int col);
};

