#ifndef UGINE_TEXTURE
#define UGINE_TEXTURE

#include "types.h"
#include "renderer.h"

class Texture {
public:
	static std::shared_ptr<Texture> Create(const std::string& filename);
	const std::string& GetFilename() const;
	uint32 GetHandle() const;
	uint32 GetWidth() const;
	uint32 GetHeight() const;
protected:
	Texture(const std::string& filename);
	~Texture();
	static void Delete(Texture* t) { delete t; }
private:
	std::string mFilename;
	uint32 mHandle;
	uint32 mWidth;
	uint32 mHeight;
};


#endif // UGINE_TEXTURE