#include "TextureManager.h"

//Declaring is necessary because it is a static variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
	string path = "images/"; 
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	// if the texture DOESN'T exist...
	if (textures.find(textureName) == textures.end()) // we didn't find it
		LoadTexture(textureName);

	return textures[textureName];
}

void TextureManager::Clear()
{
	textures.clear(); //get rid of all stored objects
}