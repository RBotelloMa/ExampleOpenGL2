#include "../include/texture.h"



std::shared_ptr<Texture> Texture::Create(const std::string& filename) {
	std::shared_ptr<Texture> ret = std::shared_ptr<Texture>(new Texture(filename), Delete);
	if (ret->GetHandle() == 0) return nullptr;
	else return ret;
};

const std::string& Texture::GetFilename() const {
	return mFilename;
}

uint32 Texture::GetHandle() const {
	return mHandle;
}
uint32 Texture::GetWidth() const {
	return mWidth;
}
uint32 Texture::GetHeight() const {
	return mHeight;
}


Texture::Texture(const std::string& filename) {
	mFilename = filename;
	mHandle = Renderer::Instance()->Renderer::LoadTexture(filename, mWidth, mHeight);
};

Texture::~Texture() {};



