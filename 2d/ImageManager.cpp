#include "ImageManager.h"

ImageManager* ImageManager::GetIns() {
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D() {
	
}

void ImageManager::LoadTex2D() {
	//Texture::LoadTexture(whiteeffect, L"Resources/2d/effect/ChargeEffect.png");
}
