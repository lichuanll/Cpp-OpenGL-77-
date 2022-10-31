#include "Camera.h"
#include<iostream>
void Camera::setviewMatrix(vec3 _pos, vec3 _front, vec3 _up)
{
	m_position = _pos;
	m_front = normalize(_front);
	m_up = _up;

}
void Camera::updata()
{
	std::cout << "(" << getPosition().x << "," << getPosition().y << "," << getPosition().z << ") " << "dir:";
	std::cout << "(" << getdirection().x << "," << getdirection().y << "," << getdirection().z << ")" << std::endl;
	m_vMatrix = lookAt(m_position, m_position + m_front, m_up);
}
vec3 Camera::getPosition()
{
	return m_position;
}
vec3 Camera::getdirection()
{
	return m_front;
}
mat4 Camera::getMatrix()
{
	return m_vMatrix;
}
void Camera::setSpeed(float _speed)
{
	m_speed = _speed;
}
void Camera::move(int _mode)
{
	switch (_mode)
	{
	case 1:
		m_position += m_front * m_speed;
		break;
	case 2:
		m_position -= m_front * m_speed;
		break;
	case 3:
		m_position -= normalize(cross(m_front, m_up)) * m_speed;
		break;
	case 4:
		m_position += normalize(cross(m_front, m_up)) * m_speed;
		break;
	default:
		break;
	}
}
void Camera::pitch(float _y0ffset)
{
	m_pitch += _y0ffset * m_sensitivity;
	if (m_pitch >= 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch <= -89.0f)
	{
		m_pitch = -89.0f;
	}

	m_front.y = sin(radians(m_pitch));
	m_front.x = cos(radians(m_yaw)) * cos(radians(m_pitch));
	m_front.z = sin(radians(m_yaw)) * cos(radians(m_pitch));

	m_front = normalize(m_front);
}
void Camera::yaw(float _x0ffset)
{
	m_yaw += _x0ffset * m_sensitivity;

	m_front.y = sin(radians(m_pitch));
	m_front.x = cos(radians(m_yaw)) * cos(radians(m_pitch));
	m_front.z = sin(radians(m_yaw)) * cos(radians(m_pitch));

	m_front = normalize(m_front);
}

void Camera::setSensitixity(float s)
{
	m_sensitivity = s;
}

void Camera::onMouseMove(double _xpos, double _ypos)
{
	if (m_firstMove)
	{
		m_xpos = _xpos;
		m_ypos = _ypos;
		m_firstMove = false;
		return;
	}

	float _x0ffset = _xpos - m_xpos;
	float _y0ffset = -(_ypos - m_ypos);

	m_xpos = _xpos;
	m_ypos = _ypos;

	pitch(_y0ffset);
	yaw(_x0ffset);
}