#include "../include/submesh.h"

std::shared_ptr<Submesh> Submesh::Create(const std::shared_ptr<Texture>& tex) {
	return std::shared_ptr<Submesh>(new Submesh(tex) , Delete);
}


void Submesh::AddVertex(const Vertex& v) {
	mVertices.push_back(v);
}
void Submesh::AddTriangle(uint16 v0, uint16 v1, uint16 v2)
{
	mIndices.push_back(v0);
	mIndices.push_back(v1);
	mIndices.push_back(v2);
};

const std::shared_ptr<Texture>& Submesh::GetTexture() const {
	return mTexture;
};
void Submesh::SetTexture(const std::shared_ptr<Texture>& tex) {
	mTexture = tex;
  if (tex) hayTextura = true;
  else  hayTextura = false;
}

bool Submesh::HayTextura() {
  return hayTextura;
}


const std::vector<Vertex>& Submesh::GetVertices() const 
{
	return mVertices;
};
std::vector<Vertex>& Submesh::GetVertices() 
{
	return mVertices;
};

void Submesh::Rebuild() {
	Renderer::Instance()->SetVertexBufferData(mVertexBuffer, &mVertices[0], sizeof(Vertex) * mVertices.size());
	Renderer::Instance()->SetIndexBufferData(mIndexBuffer, &mIndices[0], sizeof(uint16) * mIndices.size());
  //de momento ponemos aqui la inicializaci�n del color. Se podr�a poner en muchos sitios.
  mColor = glm::vec3(1, 1, 1);
  mShininess = 255;

}



void Submesh::Render() {
	
	if (mTexture) {
		Renderer::Instance()->SetTexture(mTexture.get()->GetHandle(),HayTextura());
	}
	else 
	{
		Renderer::Instance()->SetTexture(0,HayTextura());
	}
	Renderer::Instance()->SetDiffuse(glm::vec3(mColor));
	Renderer::Instance()->SetShininess(mShininess);
	Renderer::Instance()->DrawBuffers(mVertexBuffer, mIndexBuffer, mIndices.size());
}

Submesh::Submesh(const std::shared_ptr<Texture>& tex) {
	mVertexBuffer = Renderer::Instance()->CreateBuffer();
	mIndexBuffer = Renderer::Instance()->CreateBuffer();
	SetTexture(tex);
}


Submesh::~Submesh() {
	Renderer::Instance()->FreeBuffer(mVertexBuffer);
	Renderer::Instance()->FreeBuffer(mIndexBuffer);
};


//practica 3
const glm::vec3& Submesh::GetColor() const {
	return mColor;
};
void Submesh::SetColor(const glm::vec3& color) {
	mColor = color;
};
uint8 Submesh::GetShininess() const {
	return mShininess;
};
void Submesh::SetShininess(uint8 shininess) {
	mShininess = shininess;
};


/*std::shared_ptr<Texture> mTexture;
uint32 mVertexBuffer;
uint32 mIndexBuffer;
std::vector<Vertex> mVertices;
std::vector<uint16> mIndices;
*/




