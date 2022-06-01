#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Random.h"
#include "TextureManager.h"
using namespace std;

int main() {
    Board gameBoard;
    ifstream file("boards/config.cfg");
    gameBoard.loadCFG(file);

    sf::RenderWindow window(sf::VideoMode(gameBoard.cols*32, gameBoard.rows*32+64), "Minesweeper");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased)
            {
                gameBoard.handleClick(event);
                gameBoard.checkWin();
            }
        }

        window.clear();
        //Do rendering here
		gameBoard.drawBoard(window);

        window.display();

    }
	TextureManager::Clear();
}