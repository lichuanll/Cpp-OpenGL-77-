#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aUV;


out vec2 outUV;
out vec3 outNormal;
out vec3 outFragPos;


uniform mat4 _modeMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;
void main()
{

    gl_Position = _projMatrix * _viewMatrix * _modeMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0);
   
    outUV=aUV;
   
    outNormal=mat3(transpose(inverse(_modeMatrix))) * aNormal;
    outFragPos=vec3(_modeMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0));

};