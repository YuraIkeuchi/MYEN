#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	
}

void ImageManager::LoadTex2D() {
	Texture::LoadTexture(Box, L"Resources/2d/Box.png");
	Texture::LoadTexture(Line, L"Resources/2d/Line.png");
}
