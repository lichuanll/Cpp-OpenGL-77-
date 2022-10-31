#pragma once
#include"Base.h"
using namespace glm;
class Camera
{
private:
	vec3 m_position;
	vec3 m_front;
	vec3 m_up;

	float m_speed;
	float m_pitch;
	float m_yaw;
	float m_sensitivity;

	float m_xpos;
	float m_ypos;
	bool m_firstMove;

	mat4  m_vMatrix;

public:
	Camera()
	{
		m_position = vec3(1.0f);
		m_front = vec3(1.0f);
		m_up = vec3(1.0f);

		m_speed = 0.01f;
		m_pitch = 0.01f;
		m_yaw = -90.0f;
		m_sensitivity = 0.1f;;

		m_xpos = 0.0f;
		m_ypos = 0.0f;
		m_firstMove = true;
		m_vMatrix = mat4(1.0f);
	}
	~Camera()
	{

	}
	void setviewMatrix(vec3 _pos, vec3 _front, vec3 _up);
	void updata();
	vec3 getPosition();
	vec3 getdirection();
	mat4 getMatrix();
	void setSpeed(float _speed);
	void move(int _mode);
	void pitch(float _y0ffset);

	void yaw(float _x0ffset);

	void setSensitixity(float s);

	void onMouseMove(double _xpos, double _ypos);

};

