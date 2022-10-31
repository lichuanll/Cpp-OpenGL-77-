#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aUV;
layout (location=3) in vec3 aNormal;
out vec3 outColor;
out vec2 outUV;
out vec3 outNormal;
out vec3 outFragPos;
uniform mat4 _modeMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

out vec4 _posLightSpace;
uniform mat4 _lightSpaceMat;

void main()
{

    gl_Position = _projMatrix * _viewMatrix * _modeMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0);
    outColor=aColor;
    outUV=aUV;
   
    outNormal=mat3(transpose(inverse(_modeMatrix))) * aNormal;
    _posLightSpace = _lightSpaceMat * _modeMatrix * vec4(aPos,1.0);
    outFragPos=vec3(_modeMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0));
};