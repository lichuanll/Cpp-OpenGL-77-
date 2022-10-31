#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
out vec3 TexCoords;
out vec3 outNormal;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main()
{
    TexCoords = aPos;
    
    outNormal = aNormal;
    vec4 Pos =  _projMatrix * _viewMatrix * vec4(aPos, 1.0);
    gl_Position = Pos.xyww;
}