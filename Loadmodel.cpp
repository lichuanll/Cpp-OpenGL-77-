#include "Loadmodel.h"
namespace FF
{
	Mesh::Mesh(vector<Vertex> _vertices, vector<uint> _indices, vector<Texture> _textures)
	{
		this->vertices = _vertices;
		this->indices = _indices;
		this->textures = _textures;
		setupMesh();
	}



	void Mesh::Draw(shader& _shader)
	{
		uint diffuseNr = 1;
		uint specularNr = 1;

		for (uint i = 0;i < textures.size();i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			string number;
			string name = textures[i].type;

			if (name == "texture_diffuse")
				number = to_string(diffuseNr++);

			else if (name == "texture_specular")
				number = to_string(specularNr++);

			_shader.setInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::setupMesh()
	{
		uint m_VBO;
		uint m_EBO;
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &m_EBO);



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		glBindVertexArray(0);
	}

	//float vertices[] =
	//{
	//-0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.0f, 0.0f,
	// 0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       1.0f, 0.0f,
	// 0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       1.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       1.0f, 1.0f,
	//-0.5f,  0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,       0.0f,  0.0f, -1.0f,       0.0f, 0.0f,
	//-0.5f, -0.5f,  0.5f,       0.0f,  0.0f, 1.0f,        0.0f, 0.0f,
	// 0.5f, -0.5f,  0.5f,       0.0f,  0.0f, 1.0f,        1.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,       0.0f,  0.0f, 1.0f,        1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,       0.0f,  0.0f, 1.0f,        1.0f, 1.0f,
	//-0.5f,  0.5f,  0.5f,       0.0f,  0.0f, 1.0f,        0.0f, 1.0f,
	//-0.5f, -0.5f,  0.5f,       0.0f,  0.0f, 1.0f,        0.0f, 0.0f,
	//-0.5f,  0.5f,  0.5f,       -1.0f,  0.0f,  0.0f,      1.0f, 0.0f,
	//-0.5f,  0.5f, -0.5f,       -1.0f,  0.0f,  0.0f,      1.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,       -1.0f,  0.0f,  0.0f,      0.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,       -1.0f,  0.0f,  0.0f,      0.0f, 1.0f,
	//-0.5f, -0.5f,  0.5f,       -1.0f,  0.0f,  0.0f,      0.0f, 0.0f,
	//-0.5f,  0.5f,  0.5f,       -1.0f,  0.0f,  0.0f,      1.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,       1.0f,  0.0f,  0.0f,       1.0f, 0.0f,
	// 0.5f,  0.5f, -0.5f,       1.0f,  0.0f,  0.0f,       1.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,       1.0f,  0.0f,  0.0f,       0.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,       1.0f,  0.0f,  0.0f,       0.0f, 1.0f,
	// 0.5f, -0.5f,  0.5f,       1.0f,  0.0f,  0.0f,       0.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,       1.0f,  0.0f,  0.0f,       1.0f, 0.0f,
	//-0.5f, -0.5f, -0.5f,        0.0f, -1.0f,  0.0f,      0.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,        0.0f, -1.0f,  0.0f,      1.0f, 1.0f,
	// 0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,      1.0f, 0.0f,
	// 0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,      1.0f, 0.0f,
	//-0.5f, -0.5f,  0.5f,        0.0f, -1.0f,  0.0f,      0.0f, 0.0f,
	//-0.5f, -0.5f, -0.5f,        0.0f, -1.0f,  0.0f,      0.0f, 1.0f,
	//-0.5f,  0.5f, -0.5f,         0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,         0.0f,  1.0f,  0.0f,     1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,         0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,         0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
	//-0.5f,  0.5f,  0.5f,         0.0f,  1.0f,  0.0f,     0.0f, 0.0f,
	//-0.5f,  0.5f, -0.5f,         0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
	//
	//};
	//Mesh processMesh()
	//{
	//	vector<Vertex> _vertices;
	//	vector<uint> _indices;
	//	vector<Texture> _texture;
	//	uint difffuseMap;
	//	uint specularMap;
	//	for (int i = 0;i < 36; i++)
	//	{
	//		Vertex vert;
	//		vert.Position[0] = vertices[i * 8 + 0];
	//		vert.Position[1] = vertices[i * 8 + 1];
	//		vert.Position[2] = vertices[i * 8 + 2];
	//		vert.Normal[0] = vertices[i * 8 + 3];
	//		vert.Normal[1] = vertices[i * 8 + 4];
	//		vert.Normal[2] = vertices[i * 8 + 5];
	//		vert.UV[0] = vertices[i * 8 + 6];
	//		vert.UV[1] = vertices[i * 8 + 7];
	//		_vertices.push_back(vert);
	//		_indices.push_back(i);
	//	}
	//	Texture tex;
	//	tex.id = difffuseMap;
	//	tex.type = "texture_diffuse";
	//	_texture.push_back(tex);
	//	tex.id = specularMap;
	//	tex.type = "texture_specular";
	//	_texture.push_back(tex);
	//	return Mesh(_vertices, _indices, _texture);
	//}

	void Model::Draw(shader& _shader)
	{
		for (uint i = 0;i < meshes.size();i++)
		{
			meshes[i].Draw(_shader);
		}
	}

	void Model::loadModel(string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "read Model fail" << endl;
			return;
		}

		dir = path.substr(0, path.find_last_not_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		for (uint i = 0;i < node->mNumMeshes;i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		cout << node->mName.data << endl;
		for (uint i = 0;i < node->mNumChildren;i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<Vertex> m_vertexVec;
		vector<uint> m_indexVec;
		vector<Texture> m_texVec;
		for (uint i = 0;i < mesh->mNumVertices;i++)
		{
			Vertex _vertex;
			vec3 _pos;

			_pos.x = mesh->mVertices[i].x;
			_pos.y = mesh->mVertices[i].y;
			_pos.z = mesh->mVertices[i].z;

			_vertex.Position = _pos;

			vec3 _normal;

			_normal.x = mesh->mNormals[i].x;
			_normal.y = mesh->mNormals[i].y;
			_normal.z = mesh->mNormals[i].z;

			_vertex.Normal = _normal;

			if (mesh->mTextureCoords[0])
			{
				vec2 _texCoord;
				_texCoord.x = mesh->mTextureCoords[0][i].x;
				_texCoord.y = mesh->mTextureCoords[0][i].y;

				_vertex.UV = _texCoord;
			}
			m_vertexVec.push_back(_vertex);
		}

		for (uint i = 0;i < mesh->mNumFaces;i++)
		{

			aiFace _face = mesh->mFaces[i];

			for (uint j = 0;j < _face.mNumIndices;j++)
			{
				m_indexVec.push_back(_face.mIndices[j]);
			}
		}
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* _mat = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture> _diffuseVec = loadMaterialTexture(_mat, aiTextureType_DIFFUSE, "texture_diffuse");
			m_texVec.insert(m_texVec.end(), _diffuseVec.begin(), _diffuseVec.end());

			vector<Texture> _specularVec = loadMaterialTexture(_mat, aiTextureType_SPECULAR, "texture_specular");
			m_texVec.insert(m_texVec.end(), _specularVec.begin(), _specularVec.end());
		}
		Mesh mesh1 = Mesh(m_vertexVec, m_indexVec, m_texVec);
		return  mesh1;

	}

	vector<Texture> Model::loadMaterialTexture(aiMaterial* _mat, aiTextureType _type, string _typename)
	{
		vector<Texture> _texVec;
		for (uint i = 0;i < _mat->GetTextureCount(_type);i++)
		{
			Texture _tex;
			aiString _path;

			_mat->GetTexture(_type, i, &_path);
			_tex.id = createTexture(_path.C_Str(), dir);
			_tex.path = _path.C_Str();
			_tex.type = _typename;
			_texVec.push_back(_tex);
		}
		return _texVec;
	}

	uint Model::createTexture(std::string _path)
	{
		map<string, uint>::iterator _it = m_texMap.find(_path);
		if (_it != m_texMap.end())
		{
			return _it->second;
		}

		ffImage* _image = ffImage::readFromFile(_path.c_str());

		uint _texID = 0;
		glGenTextures(1, &_texID);
		glBindTexture(GL_TEXTURE_2D, _texID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _image->getWidth(), _image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _image->getdata());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		delete _image;
		m_texMap[_path] = _texID;
		return _texID;
	}

	uint Model::createTexture(std::string _path, std::string _dir)
	{
		cout << endl;
		cout << _dir + '/' + _path << endl;
		for (int i = 0;i < _path.size();i++)
		{
			if (_path[i] == '\\')
			{
				_path[i] = '/';
			}
		}
		cout << _dir + '/' + _path << endl;
		cout << _path << endl;
		return createTexture(_path);
	}

}