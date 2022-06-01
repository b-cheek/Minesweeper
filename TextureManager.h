#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
using std::unordered_map;
using std::string;

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;
	static void LoadTexture(string textureName);

public:
	static sf::Texture& GetTexture(string textureName);
	static void Clear(); //Call this once at the end of main()
};

