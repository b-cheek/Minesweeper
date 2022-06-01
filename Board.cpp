#include "Board.h"
#include <iostream>
using std::to_string;
using std::getline;
using std::stoi;

Board::Tile::Tile(bool hasBomb) {
	this->hasBomb = hasBomb;
	this->currentTexture = Texture::hidden;
	this->sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
}

void Board::loadBoard(ifstream& file) {
	this->resetBoard();

	this->face.setTexture(TextureManager::GetTexture("face_happy"));

	string line;
	while (getline(file, line)) {
		vector<Tile> temp;
		for (int i = 0; i < line.size(); i++) {
			temp.push_back((line[i] == '0') ? Tile(false) : Tile(true));
			this->bombs += (line[i] == '1');
		}
		this->board.push_back(temp);
	}

	this->cols = this->board.at(0).size();
	this->rows = this->board.size();

	this->retainDebugState();
}

void Board::loadCFG(ifstream& file) {
	this->resetBoard();

	this->face.setTexture(TextureManager::GetTexture("face_happy"));

	file >> this->cols;
	file >> this->rows;
	file >> this->bombs;

	// filling the board so it has the right amount of tiles
	for (int row = 0; row < rows; row++)
	{
		board.push_back(vector<Tile>(cols, Tile()));
	}

	// setting the bombs on the board
	int row;
	int col;

	for (int bomb = 0; bomb < this->bombs; bomb++)
	{
		do 
		{
			row = Random::Int(0, this->rows - 1);
			col = Random::Int(0, this->cols - 1);
		} while (this->board.at(row).at(col).hasBomb);

		this->board.at(row).at(col).hasBomb = true;
	}

	this->retainDebugState();

}

void Board::drawBoard(sf::RenderWindow& window) {
	int flags = 0;

	for (int row = 0; row < this->rows; row++) //Drawing the main board
	{
		for (int col = 0; col < this->cols; col++)
		{
			this->board.at(row).at(col).sprite.setPosition(col * 32, row * 32); //Since 32 is the width of the tiles
			window.draw(this->board.at(row).at(col).sprite);
			flags += (this->board.at(row).at(col).currentTexture == Tile::Texture::flag); //this will be used later to calculate the number of bombs left
		}
	}

	//Switch the order of these to change if mines or flags appear on top
	for (auto iter = this->sprites.begin(); iter != this->sprites.end(); iter++) window.draw(iter->second); //drawing the extra sprites
	for (auto iter = this->debugSprites.begin(); iter != this->debugSprites.end(); iter++) window.draw(iter->second); //drawing the debug sprites

	int bombsLeft = this->bombs - flags;
	string strBombsLeft = to_string(bombsLeft);
	bool negativeFix = false;
	for (int i = 0; i < strBombsLeft.size(); i++) if (strBombsLeft[i] == '-') {
		strBombsLeft[i] = ':'; 
		if (strBombsLeft.size() == 2) negativeFix = true;
	} //This will make it so my later ASCII magic ('1' - '0' = 1) will make it so the '-' will come out to ten, which corresponds to the digit in the digits image with the negative symbol

	sf::Sprite digit0;
	sf::Sprite digit1;
	sf::Sprite digit2;

	sf::Texture texture0;
	sf::Texture texture1;
	sf::Texture texture2;

	switch (strBombsLeft.size()) { // there is a smarter way to do this with stringstream. I have chosen not to do this
	case 1:
		texture0.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
		texture1.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
		texture2.loadFromFile("images/digits.png", sf::IntRect((bombsLeft) * 21, 0, 21, 32)); // digit is 21x32 dimension
		break;

	case 2:
		texture0.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
		texture1.loadFromFile("images/digits.png", sf::IntRect((strBombsLeft[0] - '0') * 21, 0, 21, 32)); // '1' - '0' = 1. ASCII magic
		texture2.loadFromFile("images/digits.png", sf::IntRect((strBombsLeft[1] - '0') * 21, 0, 21, 32));
		break;

	case 3:
		texture0.loadFromFile("images/digits.png", sf::IntRect((strBombsLeft[0] - '0') * 21, 0, 21, 32));
		texture1.loadFromFile("images/digits.png", sf::IntRect((strBombsLeft[1] - '0') * 21, 0, 21, 32));
		texture2.loadFromFile("images/digits.png", sf::IntRect((strBombsLeft[2] - '0') * 21, 0, 21, 32));
		break;
	}

	digit0.setTexture(texture0);
	digit1.setTexture(texture1);
	digit2.setTexture(texture2);

	digit0.setPosition(0, this->rows * 32);
	digit1.setPosition(21, this->rows * 32);
	digit2.setPosition(42, this->rows * 32);

	if (!negativeFix) window.draw(digit0);
	window.draw(digit1);
	window.draw(digit2);

	//drawing menu options
	//sf::Sprite face(TextureManager::GetTexture("face_happy")); I made this a class member of the board so it's easy to change
	sf::Sprite debug(TextureManager::GetTexture("debug"));
	sf::Sprite test1(TextureManager::GetTexture("test_1"));
	sf::Sprite test2(TextureManager::GetTexture("test_2"));
	sf::Sprite test3(TextureManager::GetTexture("test_3"));

	this->face.setPosition(this->cols * 16 - 32, this->rows * 32); //The *16 here is because everything is based on the horizontal midpoint of the minefield
	debug.setPosition(this->cols * 16 + 96, this->rows * 32);
	test1.setPosition(this->cols * 16 + 160, this->rows * 32);
	test2.setPosition(this->cols * 16 + 224, this->rows * 32);
	test3.setPosition(this->cols * 16 + 288, this->rows * 32);

	window.draw(this->face);
	window.draw(debug);
	window.draw(test1);
	window.draw(test2);
	window.draw(test3);
}

void Board::toggleDebug() {
	if (!this->debugMode)
	{
		for (int row = 0; row < this->rows; row++) for (int col = 0; col < this->cols; col++) 
		{
			if (this->board.at(row).at(col).hasBomb)
			{
				this->debugSprites.emplace(row * this->cols + col, sf::Sprite(TextureManager::GetTexture("mine")));
				this->debugSprites[row * this->cols + col].setPosition(col * 32, row * 32);
			}
		}
	}

	else
	{
		for (int row = 0; row < this->rows; row++) for (int col = 0; col < this->cols; col++) 
		{
			if (this->board.at(row).at(col).hasBomb) this->debugSprites.erase(row * this->cols + col);
		}
	}

	this->debugMode = !this->debugMode;
}

void Board::boom() {
		for (int row = 0; row < this->rows; row++) for (int col = 0; col < this->cols; col++) 
		{
			if (this->board.at(row).at(col).hasBomb)
			{
				this->debugSprites.emplace(row * this->cols + col, sf::Sprite(TextureManager::GetTexture("mine")));
				this->debugSprites[row * this->cols + col].setPosition(col * 32, row * 32);
				this->board.at(row).at(col).sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
			}
		}

		this->face.setTexture(TextureManager::GetTexture("face_lose"));
		this->alive = false;
}

void Board::resetBoard() 
{
	this->board.clear();
	this->alive = true;
	this->sprites.clear();
	this->debugSprites.clear();
	this->bombs = 0;
	this->cols = 0;
	this->rows = 0;
}

void Board::retainDebugState()
{
	if (this->debugMode) //this is so debug will stay on while resetting, and the mines will be in correct places
	{
		this->toggleDebug();
		this->toggleDebug();
	}
}

void Board::handleClick(sf::Event event) //This function handles the player's interaction with the board. I should have split this into multiple functions, But I just don't want to
{
		if (event.mouseButton.button == sf::Mouse::Left) //If it's a left click, you uncover the mine
		{
			if (this->alive) { 

				if (event.mouseButton.y < this->rows * 32 && this->board.at(event.mouseButton.y / 32).at(event.mouseButton.x / 32).currentTexture != Tile::Texture::flag) //This check is to see whether the user is clicking the board, or an option in the menu and does nothing if flag
				{
					if (this->board.at(event.mouseButton.y / 32).at(event.mouseButton.x / 32).hasBomb)
					{
						this->boom();
					}
					else if (this->board.at(event.mouseButton.y / 32).at(event.mouseButton.x / 32).currentTexture == Tile::Texture::hidden) //Since the board takes up all the space above this line, this assumption is safe (you can't click outside the board horizontally)
					{
						clearTile(event.mouseButton.y / 32, event.mouseButton.x / 32);
					}
				}
			}

			if (event.mouseButton.y > this->rows * 32) //My code is awful. Not really this line in particular, but I've just been writing a lot of garbage around these features. Please pretend this is at the end of every line.
			{
				if (event.mouseButton.x >= this->cols * 16 - 32 && event.mouseButton.x < this->cols * 16 + 32) {
					ifstream file("boards/config.cfg");
					this->loadCFG(file);
				}
				else if (this->alive && event.mouseButton.x >= this->cols * 16 + 96 && event.mouseButton.x < this->cols * 16 + 160) this->toggleDebug();
				else if (event.mouseButton.x >= this->cols * 16 + 160 && event.mouseButton.x < this->cols * 16 +224) {
					ifstream file("boards/testboard1.brd");
					this->loadBoard(file);
				}
				else if (event.mouseButton.x >= this->cols * 16 + 224 && event.mouseButton.x < this->cols * 16 + 288) { 
					ifstream file("boards/testboard2.brd");
					this->loadBoard(file);
				}
				else if (event.mouseButton.x >= this->cols * 16 + 288 && event.mouseButton.x < this->cols * 16 + 352) {
					ifstream file("boards/testboard3.brd");
					this->loadBoard(file);
				}
			}
		}

		else if (this->alive &&
			event.mouseButton.y<=this->rows*32 &&	//safety first
			(this->board.at(event.mouseButton.y/32).at(event.mouseButton.x/32).currentTexture == Tile::Texture::hidden ||
			 this->board.at(event.mouseButton.y/32).at(event.mouseButton.x/32).currentTexture == Tile::Texture::flag)) //if it's a right click, you attempt to flag the mine. I will add checks later to see if the mine is revealed or has a number on it. 
		{
			int temp = event.mouseButton.y / 32 * this->cols + event.mouseButton.x / 32;
			if (this->board.at(event.mouseButton.y / 32).at(event.mouseButton.x / 32).currentTexture == Tile::Texture::hidden)
			{
				this->sprites.emplace(temp, sf::Sprite(TextureManager::GetTexture("flag")));
				this->sprites[temp].setPosition(event.mouseButton.x - event.mouseButton.x % 32, event.mouseButton.y - event.mouseButton.y % 32);
				this->board.at(event.mouseButton.y / 32).at(event.mouseButton.x / 32).currentTexture = Tile::Texture::flag;
			}


			else
			{
				this->sprites.erase(temp);
				this->board.at(event.mouseButton.y / 32).at(event.mouseButton.x / 32).currentTexture = Tile::Texture::hidden;
			}
		}
}

void Board::checkWin() {
	int prevFlags = 0; //this is an annoying var I have to make for my hack to make the bomb count say 0 when you win
	bool hasWon = true; //until shown to be otherwise
	for (int row = 0; row < this->rows; row++) for (int col = 0; col < this->cols; col++)
	{
		if (!this->board.at(row).at(col).hasBomb && 
			(this->board.at(row).at(col).currentTexture == Tile::Texture::hidden || 
			this->board.at(row).at(col).currentTexture == Tile::Texture::flag))
				hasWon = false;

		prevFlags += (this->board.at(row).at(col).currentTexture == Tile::Texture::flag);
	}

	if (hasWon) 
	{
		if (this->debugMode) this->toggleDebug(); //This was unclear in instructions, but I think it should be done

		for (int row = 0; row < this->rows; row++) for (int col = 0; col < this->cols; col++) 
		{
			if (this->board.at(row).at(col).hasBomb)
			{
				this->sprites.emplace(row * this->cols + col, sf::Sprite(TextureManager::GetTexture("flag")));
				this->sprites[row * this->cols + col].setPosition(col * 32, row * 32);
			}
		}

		this->face.setTexture(TextureManager::GetTexture("face_win"));
		this->alive = false;

		this->bombs = prevFlags;
	}
}

void Board::clearTile(int row, int col)
{
	if (this->board.at(row).at(col).currentTexture == Tile::Texture::hidden) {
		this->board.at(row).at(col).sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
		this->board.at(row).at(col).currentTexture = Tile::Texture::revealed;

		if (this->badNeighbors(row, col) > 0)
		{
			this->sprites.emplace(
				(row)*this->cols + col,
				sf::Sprite(TextureManager::GetTexture("number_" +
					to_string(this->badNeighbors(row, col)))));

			this->sprites[(row)*this->cols + col].setPosition(
				col * 32, row * 32); //the modulus just makes it so the sprite is placed in the correct part so it is aligned with other sprites
		}

		else
		{
			vector<int> neighborsArr;
			for (int i = row - 1; i < row + 2; i++) for (int j = col - 1; j < col + 2; j++) //So the idea with this fn is that I'm finding if the neighbors have bombs each time I click, checking to see that they are in range.
			{
				if (i >= 0 && i < this->rows && j >= 0 && j < this->cols)
				{
					neighborsArr.push_back(i);
					neighborsArr.push_back(j);
				}
			}

			for (int i = 0; i < neighborsArr.size() - 1; i += 2) {
				clearTile(neighborsArr[i], neighborsArr[i + 1]);
			}
		}
	}
}

int Board::badNeighbors(int row, int col) {
	int badNeighbors = 0;
	vector<int> neighborsArr;
	for (int i = row - 1; i < row + 2; i++) for (int j = col - 1; j < col + 2; j++) //So the idea with this fn is that I'm finding if the neighbors have bombs each time I click, checking to see that they are in range.
	{
		if (i >= 0 && i < this->rows && j>=0 && j < this->cols)
		{
			neighborsArr.push_back(i);
			neighborsArr.push_back(j);
		}
	}

	for (int i = 0; i < neighborsArr.size()-1; i += 2) {
		badNeighbors += (this->board.at(neighborsArr[i]).at(neighborsArr[i + 1]).hasBomb);
	}

	return badNeighbors;
}
