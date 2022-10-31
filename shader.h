#pragma once
#include"Base.h"
using namespace std;
using namespace glm;
class shader
{
private:
	unsigned int m_shaderProgram;

public:
	shader()
	{
		m_shaderProgram = 0;
	}
	~shader()
	{

	}
	
	uint compileShader(const char* _shaderPath, GLint _shaderType);
	void initshader(const char* _vertexPath, const char* _fragPath);

    void intshader(const char* _vertexPath, const char* _fragPath, const char* _geoPath);
	void start()
	{
		glUseProgram(m_shaderProgram);
	}
	void end()
	{
		glUseProgram(0);
	}

	uint getProgram()
	{
		return m_shaderProgram;
	}

	//void setvec3(const string& _name, vec3 _vec3)const;
	void setFloat(const string& _name, float _f)const;

	void setInt(const string& _name, int _i)const;

	void setMatrix(const string& _name, mat4 _matrix)const;

	void setvec3(const string& _name, vec3 _vec3)const;
	void setvec4(const string& _name, vec4 _vec4)const;

};

