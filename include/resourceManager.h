#ifndef UGINE_RESOURCEMANAGER_H
#define UGINE_RESOURCEMANAGER_H

#include "types.h"
#include "texture.h"
#include "mesh.h"

class ResourceManager{
public:
	static const std::shared_ptr<ResourceManager>& Instance();
	std::shared_ptr<Mesh>LoadMesh(const std::string& filename);
	std::shared_ptr<Texture>LoadTexture(const std::string& filename);
	void FreeMeshes();
	void FreeTextures();
	void FreeResources();

protected:
	ResourceManager() {}
	~ResourceManager() { FreeResources(); }
	static void Delete(ResourceManager*rm) { delete rm; }
private:
	static std::shared_ptr<ResourceManager> mInstance;
	std::map<std::string,std::shared_ptr<Mesh>> mMeshes;
	std::map<std::string,std::shared_ptr<Texture>> mTextures;
};


inline const std::shared_ptr<ResourceManager>& ResourceManager::Instance() {
	if (!mInstance)
		mInstance = std::shared_ptr<ResourceManager>(new ResourceManager(), Delete);
	return mInstance;
}

#endif // UGINE_RESOURCEMANAGER_H