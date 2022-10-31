#pragma once
#include"Base.h"
#include"shader.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffImage.h"
using namespace std;
using namespace glm;

namespace FF
{


	struct Vertex
	{
		vec3 Position;
		vec3 Normal;
		vec2 UV;
		Vertex()
		{
			Position = vec3(0.0f);
			Normal = vec3(0.0f);
			UV = vec2(0.0f);
		}
		~Vertex()
		{

		}
	};
	struct Texture
	{
		uint id;
		string type;
		string path;
	};
	class Mesh
	{
	private:
		vector<Vertex> vertices;
		vector<uint> indices;
		vector<Texture>textures;

		void setupMesh();
		
		GLuint m_VAO;

	public:
		Mesh(vector<Vertex> _vertices, vector<uint> _indices, vector<Texture> _texture);


		void Draw(shader& _shader);
	};
	class Model
	{
	public:
		Model(const char* path)
		{
			loadModel(path);
		}
		~Model()
		{

		}
		void Draw(shader& _shader);
	private:
		vector<Mesh> meshes;
		string dir;


		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture> loadMaterialTexture(aiMaterial* _mat, aiTextureType _type, string _typename);
		uint createTexture(std::string _path);
		uint createTexture(std::string _path, std::string _dir);
		map<string, uint>	m_texMap;
	};
}