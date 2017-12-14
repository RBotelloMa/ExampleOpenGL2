#include "../include/mesh.h"


std::shared_ptr<Mesh> Mesh::Create() {
	return std::shared_ptr<Mesh>(new Mesh(), Delete);
};

std::shared_ptr<Mesh> Mesh::Create(const std::string& filename) {
	return std::shared_ptr<Mesh>(new Mesh(filename), Delete);

};

const std::string& Mesh::GetFilename() const {
	return mFilename;
};

void Mesh::AddSubmesh(const std::shared_ptr<Submesh>& submesh) {
	mSubmeshes.push_back(submesh);
	submesh->Rebuild();
};

void Mesh::RemoveSubmesh(uint32 i) {
	mSubmeshes.erase(mSubmeshes.begin() + i);
};

void Mesh::RemoveSubmesh(const std::shared_ptr<Submesh>& submesh) {
	mSubmeshes.erase(std::find(mSubmeshes.begin(), mSubmeshes.end(), submesh));
};

uint32 Mesh::NumSubmeshes() const {
	return mSubmeshes.size();
};

const std::shared_ptr<Submesh>& Mesh::GetSubmesh(uint32 i) {
	return mSubmeshes[i];
};

void Mesh::Render() {
	
	for (auto it = mSubmeshes.begin(); it != mSubmeshes.end();++it) {
		
		it->get()->Render();
		
	}
};


Mesh::Mesh() {};

Mesh::Mesh(const std::string& filename) {
	mFilename = filename;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(mFilename.c_str());
	if (result) {
		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node submeshesNode = doc.child("mesh").child("submeshes");
		for (pugi::xml_node submeshNode = submeshesNode.child("submesh");
			submeshNode;
			submeshNode = submeshNode.next_sibling("submesh"))
			{
				// Iteramos por todos los elementos “submesh” contenidosen el elemento “submeshes”
				//textura
				std::string textureStr = submeshNode.child("texture").text().as_string();
				std::string path = ExtractPath(filename);
				std::shared_ptr<Texture> textura = Texture::Create(path+"/"+textureStr);
				std::shared_ptr<Submesh> subMesh = Submesh::Create(textura);


				//indices
					
				std::vector<std::string> indicesVec = SplitString(submeshNode.child("indices").text().as_string(), ',');
				for (int i = 0; i < indicesVec.size(); i = (i + 3)) {
					uint16 indice0 = NumberFromString<uint16>(indicesVec[i]);
					uint16 indice1 = NumberFromString<uint16>(indicesVec[i + 1]);
					uint16 indice2 = NumberFromString<uint16>(indicesVec[i + 2]);
					subMesh.get()->AddTriangle(indice0, indice1, indice2);
				}
				//coords 
				
        std::vector<std::string> coordsVec = SplitString(submeshNode.child("coords").text().as_string(), ',');
				std::vector<std::string> texcoordsVec = SplitString(submeshNode.child("texcoords").text().as_string(), ',');
				std::vector<std::string> normsVec = SplitString(submeshNode.child("normals").text().as_string(), ',');
        std::vector<std::string> boneIndex = SplitString(submeshNode.child("bone_indices").text().as_string(), ',');
        std::vector<std::string> boneWeights = SplitString(submeshNode.child("bone_weights").text().as_string(), ',');
				
        int n1 = coordsVec.size()/3;
				int n2 = texcoordsVec.size() / 2;
				int n3 = normsVec.size() / 3;
        int n4 = boneIndex.size() / 4;
        int n5 = boneWeights.size() / 4;
				int maxim = std::max(std::max(std::max(n1, n2), std::max(n3, n2)),n5);
				
        for (int i = 0; i < maxim; i++) {
					float coord0, coord1, coord2;
					float texcoord0, texcoord1;
					float norm0, norm1, norm2;
          float boneIdnx0, boneIdnx1, boneIdnx2, boneIdnx3;
          float boneW0, boneW1, boneW2, boneW3;
					
          if (i < n1) {
						coord0 = NumberFromString<float>(coordsVec[3 * i]);
						coord1 = NumberFromString<float>(coordsVec[3 * i + 1]);
						coord2 = NumberFromString<float>(coordsVec[3 * i + 2]);
					}
					else {
						coord0 = coord1 = coord2 = 0;
					}

					if (i < n2) {
						texcoord0 = NumberFromString<float>(texcoordsVec[2 * i]);
						texcoord1 = NumberFromString<float>(texcoordsVec[2 * i + 1]);
					}
					else {
            texcoord0 = i;
            texcoord1 = 0;
					}

					if (i < n3) {
						norm0 = NumberFromString<float>(normsVec[3 * i]);
						norm1 = NumberFromString<float>(normsVec[3 * i + 1]);
						norm2 = NumberFromString<float>(normsVec[3 * i + 2]);
					}
					else {
						norm0 = norm1 = norm2 = 0;
					}

          if (i < n4) {
            boneIdnx0 = NumberFromString<float>(boneIndex[4 * i]);
            boneIdnx1 = NumberFromString<float>(boneIndex[4 * i+1]);
            boneIdnx2 = NumberFromString<float>(boneIndex[4 * i+2]);
            boneIdnx3 = NumberFromString<float>(boneIndex[4 * i+3]);
          }
          else {
            boneIdnx0 = boneIdnx1 = boneIdnx2 = boneIdnx3 = -1;
          }

          if (i < n5) {
            boneW0 = NumberFromString<float>(boneWeights[4 * i]);
            boneW1 = NumberFromString<float>(boneWeights[4 * i + 1]);
            boneW2 = NumberFromString<float>(boneWeights[4 * i + 2]);
            boneW3 = NumberFromString<float>(boneWeights[4 * i + 3]);
          }
          else {
            boneW0 = boneW1 = boneW2 = boneW3 = 0;
          }
					subMesh.get()->AddVertex(Vertex(
            glm::vec3(coord0,coord1,coord2), 
            glm::vec2(texcoord0, texcoord1), 
            glm::vec3(norm0, norm1, norm2),
            glm::vec4(boneIdnx0,boneIdnx1,boneIdnx2,boneIdnx3),
            glm::vec4(boneW0,boneW1,boneW2,boneW3)));
				}
				AddSubmesh(subMesh);
			}

   //practica 5
   pugi::xml_node lastFrameNode = doc.child("mesh").child("last_frame");
   mLastFrame = lastFrameNode.text().as_int();
   
   pugi::xml_node bonesNode = doc.child("mesh").child("bones");
   for (pugi::xml_node boneNode = bonesNode.child("bone");
      boneNode;
      boneNode = boneNode.next_sibling("bone"))
    {
      std::string boneName = boneNode.child("name").text().as_string();
      std::string dadName = boneNode.child("parent").text().as_string();
      int dadId = GetBoneIndex(dadName);
      std::shared_ptr<Bone> bone = Bone::Create(boneName, dadId);
      
      //inv pose
      std::vector<std::string> invPoseVec = SplitString(boneNode.child("inv_pose").text().as_string(), ',');
      glm::mat4 InvP;
      for (int i = 0; i < invPoseVec.size()/4; i = i++) {
        InvP[i][0] = NumberFromString<float>(invPoseVec[4*i]);
        InvP[i][1] = NumberFromString<float>(invPoseVec[4 * i+1]);
        InvP[i][2] = NumberFromString<float>(invPoseVec[4 * i+2]);
        InvP[i][3] = NumberFromString<float>(invPoseVec[4 * i+3]);
      }
      bone->SetInversePoseMatrix(InvP);
      
      //positions
      std::vector<std::string> positionsVec = SplitString(boneNode.child("positions").text().as_string(), ',');
      for (int i = 0; i < positionsVec.size(); i = (i + 4)) {
        int f = NumberFromString<float>(positionsVec[i]);
        glm::vec3 pos = glm::vec3(NumberFromString<float>(positionsVec[i + 1]), NumberFromString<float>(positionsVec[i + 2]), NumberFromString<float>(positionsVec[i + 3]));
        bone->AddPosition(f, pos);
      }

      //rotations
      std::vector<std::string> rotationsVec = SplitString(boneNode.child("rotations").text().as_string(), ',');
      for (int i = 0; i < rotationsVec.size(); i = (i + 5)) {
        int f = NumberFromString<float>(rotationsVec[i]);
        glm::quat rot = glm::quat(NumberFromString<float>(rotationsVec[i + 1]), NumberFromString<float>(rotationsVec[i + 2]), NumberFromString<float>(rotationsVec[i + 3]), NumberFromString<float>(rotationsVec[i + 4]));
        bone->AddRotation(f, rot);
      }

      //scale
      std::vector<std::string> scalesVec = SplitString(boneNode.child("scales").text().as_string(), ',');
      for (int i = 0; i < scalesVec.size(); i = (i + 4)) {
        int f = NumberFromString<float>(scalesVec[i]);
        glm::vec3 scl = glm::vec3(NumberFromString<float>(scalesVec[i + 1]), NumberFromString<float>(scalesVec[i + 2]), NumberFromString<float>(scalesVec[i + 3]));
        bone->AddScale(f, scl);
      }

      AddBone(bone);
    }
	}
};

//practica 5

void Mesh::CalculateAnimMatrices(float frame, std::vector<glm::mat4>& animMatrices) {
  for (int i = 0; i < mBones.size(); ++i) {
    int dad = mBones[i]->GetParentIndex();
    if (dad == -1) animMatrices[i] = mBones[i]->CalculateAnimMatrix(frame);
    else animMatrices[i] = animMatrices[dad] * mBones[i]->CalculateAnimMatrix(frame);
  }
  for (int i = 0; i< animMatrices.size(); ++i) {
    animMatrices[i] = animMatrices[i] * mBones[i]->GetInversePoseMatrix();
  }
};


int Mesh::GetBoneIndex(const std::string& name) {
  int i;
  for (i = 0; i < mBones.size(); ++i) {
    if (mBones[i]->GetName() == name) return i;
  }
  return -1;
};


/*private:
	std::string mFilename;
	std::vector<std::shared_ptr<Submesh>> mSubmeshes;
*/