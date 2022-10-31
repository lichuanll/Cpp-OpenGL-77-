// Renderer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"Base.h"
#include"shader.h"
#include"ffImage.h"
#include"Camera.h"
#include"Loadmodel.h"

using namespace std;
uint VAO = 0; 
uint cube_VAO = 0;
int point_num;

uint texture = 0;
uint cubetex = 0;
ffImage* pimage = NULL;
Camera _camera;
FF::Model* model;

shader _shadercubemap;
shader _shader;
shader _modelshader;
shader _shadowshader;
char _name[20];

int _width = 800;
int _height = 600;
mat4 _projMatrix = perspective(radians(45.0f), (float)_width / (float)_height, 0.1f, 100.0f);

int iftexture = 0;
float _angle = 0.01f;
uint depthMap = 0;
uint depthFBO;


uint TriangleVAO()
{
    uint _VAO = 0;
    uint _VBO = 0;

    float vertices[] = {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,0.0f, 0.0f,0.0f,1.0f,// 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f,0.0f,1.0f,// 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f,1.0f, 0.0f,0.0f,1.0f
    };
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return _VAO;

}

uint RectangularVAO()
{
    uint _VAO = 0;
    uint _VBO = 0;
    float vertices[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,      0.0f,0.0f,   0.0f,0.0f,1.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,      0.0f,1.0f,   0.0f,0.0f,1.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,      1.0f,1.0f,    0.0f,0.0f,1.0f,                                                  
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,      1.0f,0.0f,    0.0f,0.0f,1.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,      1.0f,1.0f,    0.0f,0.0f,1.0f,                                                    
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,      0.0f,0.0f,    0.0f,0.0f,1.0f,
    };

    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    return _VAO;
}

uint ModelVAO()
{

    uint _VAO = 0;
    uint _VBO = 0;


    float vertices[] =
    {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f, 0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f,     0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f, 0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,     0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,    0.0f, 0.0f, 1.0f,     0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f, 1.0f, 0.0f,     0.0f,  1.0f,  0.0f,

    };
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return _VAO;
}

uint floorVAO()
{
    uint _VAO = 0;
    uint _VBO = 0;
    float vertices[] = {
       15.0f, -2.0f,  15.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -15.0f, -2.0f, -15.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f, 1.0f, 0.0f, 0.0f,
       -15.0f, -2.0f,  15.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
       -15.0f, -2.0f, -15.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f, 1.0f, 0.0f, 0.0f,
       15.0f, -2.0f,  15.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        15.0f, -2.0f, -15.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f, 0.0f, 0.0f, 1.0f,
                              
        // positions            // normals         // texcoords
       
    };
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return _VAO;
}
uint cubemapVAO()
{
    uint _VAO = 0;
    uint _VBO = 0;

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f, 0.0f,0.0f,-1.0f,
        -1.0f, -1.0f, -1.0f, 0.0f,0.0f,-1.0f,
         1.0f, -1.0f, -1.0f, 0.0f,0.0f,-1.0f,
         1.0f, -1.0f, -1.0f, 0.0f,0.0f,-1.0f,
         1.0f,  1.0f, -1.0f, 0.0f,0.0f,-1.0f,
        -1.0f,  1.0f, -1.0f, 0.0f,0.0f,-1.0f,

        -1.0f, -1.0f,  1.0f, -1.0f,0.0f,0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,0.0f,0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,0.0f,0.0f,
        -1.0f,  1.0f, -1.0f, -1.0f,0.0f,0.0f,
        -1.0f,  1.0f,  1.0f, -1.0f,0.0f,0.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,0.0f,0.0f,

         1.0f, -1.0f, -1.0f,  1.0f,0.0f,0.0f,
         1.0f, -1.0f,  1.0f,  1.0f,0.0f,0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,0.0f,0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,0.0f,0.0f,
         1.0f,  1.0f, -1.0f,  1.0f,0.0f,0.0f,
         1.0f, -1.0f, -1.0f,  1.0f,0.0f,0.0f,

        -1.0f, -1.0f,  1.0f,  0.0f,0.0f,1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,0.0f,1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,0.0f,1.0f,
         1.0f,  1.0f,  1.0f,  0.0f,0.0f,1.0f,
         1.0f, -1.0f,  1.0f,  0.0f,0.0f,1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,0.0f,1.0f,

        -1.0f,  1.0f, -1.0f,  0.0f,1.0f,0.0f,
         1.0f,  1.0f, -1.0f,  0.0f,1.0f,0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,1.0f,0.0f,
         1.0f,  1.0f,  1.0f,  0.0f,1.0f,0.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,1.0f,0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,1.0f,0.0f,

        -1.0f, -1.0f, -1.0f,  0.0f,-1.0f,0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,-1.0f,0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,-1.0f,0.0f,
         1.0f, -1.0f, -1.0f,  0.0f,-1.0f,0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,-1.0f,0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,-1.0f,0.0f,
    };


    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return _VAO;
}
void initshader()
{
    _shader.initshader("vertexshader.glsl", "fragmentshader.glsl");
    _shadercubemap.initshader("vcubemapshader.glsl", "fcubemapshader.glsl"); 
    _modelshader.initshader("vModelshader.glsl", "fModelshader.glsl");
    _shadowshader.initshader("vshadowshader.glsl", "fshadowshader.glsl");
}
vec3 cubePositions[] = {
  vec3(0.0f,  0.0f,  0.0f),
  vec3(2.0f,  5.0f, -15.0f),
  vec3(-1.5f, 3.5f, -2.5f),
  vec3(-3.8f, 3.0f, -12.3f),
  vec3(2.4f, -0.4f, -3.5f),
  vec3(-1.7f,  3.0f, -7.5f),
  vec3(1.3f, -1.0f, -2.5f),
  vec3(1.5f,  2.0f, -2.5f),
  vec3(1.5f,  0.2f, -1.5f),
  vec3(-1.3f,  1.0f, -1.5f)
};
uint creattexture(const char* _filename)
{
    pimage = ffImage::readFromFile(_filename);
    uint _texture = 0;
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pimage->getWidth(), pimage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pimage->getdata());
    return _texture;
}
uint creatcubemapTex()
{
    uint tid = 0;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
    vector<string> facePath =
    {
        /*"skybox/o.jpg",
        "skybox/o.jpg",
       "skybox/o.jpg",
        "skybox/o.jpg",
        "skybox/o.jpg",
        "skybox/o.jpg"*/
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };

    for (int i = 0;i < 6;i++)
    {
        ffImage* _pImage = ffImage::readFromFile(facePath[i].c_str());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, _pImage->getWidth(), _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _pImage->getdata());

        delete _pImage;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return tid;
}
uint creatShadowFBO()
{
    uint FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return FBO;
}
void rend()
{
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    _camera.updata();
    glDepthFunc(GL_LEQUAL);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubetex);
    _shadercubemap.start();
    glBindVertexArray(cube_VAO);
    _shadercubemap.setMatrix("_viewMatrix", mat3(_camera.getMatrix()));
    _shadercubemap.setMatrix("_projMatrix", _projMatrix);
    _shadercubemap.setvec3("lightcolor", vec3(1.0, 1.0, 1.0));
    _shadercubemap.setvec3("lightdirction", vec3(2.0, -8.0, 6.0));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    _shadercubemap.end();


    glDepthFunc(GL_LESS);
    
    glActiveTexture(GL_TEXTURE0);
   // glBindTexture(GL_TEXTURE_2D, texture);


    mat4 _modelMatrix(1.0f);

   // float near_plane = 1.0f, far_plane = 70.5f;
   // mat4 _lightViewMat = lookAt(vec3(-20.0f, 40.0f, -10.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
   // mat4 _OrthoMat = ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
   // //glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
   // //glClear(GL_DEPTH_BUFFER_BIT);
   // //glViewport(0, 0, 1024, 1024);
   // //_shadowshader.start();
   // //_shadowshader.setMatrix("_viewMatrix", _lightViewMat);
   // //_shadowshader.setMatrix("_projMatrix", _OrthoMat);
   // //_shadowshader.setMatrix("_modelMatrix", _modelMatrix);
   // //glBindVertexArray(floorVAO());
   // //
   // //glDrawArrays(GL_TRIANGLES, 0, 6);
   // //glBindVertexArray(VAO);
   // //for (int i = 0;i < 3;i++)
   // //{
   // //    mat4 model(1.0f);
   // //    model = glm::translate(model, cubePositions[i]);
   // //    float angle = 20.0f * i;
   // //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
   // //    _shadowshader.setMatrix("_modelMatrix", model);
   // //    //glDrawArrays(GL_TRIANGLES, 0, 36);
   // //    glDrawArrays(GL_TRIANGLES, 0, point_num);
   // //}
   // //
   // //_shadowshader.end();

   // //glBindFramebuffer(GL_FRAMEBUFFER, 0);
   // //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   //// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glViewport(0, 0, _width, _height);
   // _shader.start();
  
 

   // vec3 lightColor;
   // lightColor.x = sin(glfwGetTime() * 2.0f);
   // lightColor.y = sin(glfwGetTime() * 0.7f);
   // lightColor.z = sin(glfwGetTime() * 1.3f);
   //
   ///* dirction_lightpos;
   // point_lightpos[5];
   // spot_lightpos;
   // pointlightcount;*/

   // vec4 lightpos = vec4(-2.0f, -1.0f, -3.0f, 1.0f);
   // lightpos.x = sin(radians(_angle)) * -3.0f;
   // lightpos.z = cos(radians(_angle)) * -3.0f;
   //
   // vec3 diffuseColor = lightColor * vec3(0.5f); // 降低影响
   // _shader.setMatrix("_modeMatrix", _modelMatrix);  
   // _shader.setMatrix("_viewMatrix", _camera.getMatrix());
   // _shader.setMatrix("_projMatrix", _projMatrix);
   // _shader.setvec3("material.ambient", vec3(1.0f, 1.0f, 1.0f));
   // _shader.setvec3("material.diffuse", vec3(1.0f, 0.5f, 0.31f));
   // _shader.setvec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
   // _shader.setFloat("material.shininess", 32.0f);
   // _shader.setvec3("light.ambient", vec3(0.1,0.1,0.1));
   // _shader.setvec3("light.diffuse", vec3(0.8,0.8,0.8));// // diffuseColor将光照调暗了一些以搭配场景
   // _shader.setvec3("light.specular", vec3(1.0f, 1.0f, 1.0f));
   // _shader.setvec3("light.dirction_lightpos", vec3(-2.0f, 4.0f, -1.0f));
   // _shader.setvec3("light.point_lightpos[0]", vec3(0.7f, 0.2f, 2.0f));
   // _shader.setvec3("light.spot_lightpos", _camera.getPosition());
   // _shader.setvec3("viewPos", _camera.getPosition());
   // _shader.setvec3("Lightcolor", vec3(1.0f, 1.0f, 1.0f)); 
   // _shader.setvec3("Lightdirection", _camera.getdirection());
   // //_shader.setvec4("LightPos", vec4(_camera.getPosition(),1.0f));
   // _shader.setInt("iftexture", iftexture);
   // _shader.setInt("ifusetex", 1);
   // 
   // _shader.setInt("light.pointlightcount", 1);
   // _shader.setFloat("light.constant", 1.0f);
   // _shader.setFloat("light.linear", 0.09f);
   // _shader.setFloat("light.quadratic", 0.032f);
   // _shader.setFloat("light.cutOff", cos(radians(12.5f)));
   // _shader.setFloat("light.outcutOff", cos(radians(17.5f)));
   // _shader.setMatrix("_lightSpaceMat", _OrthoMat * _lightViewMat);
   // //_shader.setInt("_shadowMap", 1);
   // glActiveTexture(GL_TEXTURE1);
   // glBindTexture(GL_TEXTURE_2D, depthMap);
   // _shader.setMatrix("_modeMatrix", mat4(1.0f));
   // glBindVertexArray(floorVAO());
   // _shader.setInt("ifusetex", 0);
   // glDrawArrays(GL_TRIANGLES, 0, 6);
   // glBindVertexArray(VAO);
   // glBindTexture(GL_TEXTURE_CUBE_MAP, cubetex);
   // for (int i = 0;i < 3;i++)
   // {
   //     mat4 model(1.0f);
   //     model = glm::translate(model, cubePositions[i]);
   //     float angle = 20.0f * i;
   //     model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
   //     _shader.setMatrix("_modeMatrix", model);

   //     //glDrawArrays(GL_TRIANGLES, 0, 36);
   //     glDrawArrays(GL_TRIANGLES, 0, point_num);
   // }

   // 

   // _shader.end();
    glActiveTexture(GL_TEXTURE0);

    
    _modelMatrix = rotate(_modelMatrix, radians(_angle), vec3(0.0f, 1.0f, 0.0f));
    _modelMatrix = scale(_modelMatrix, vec3(0.3, 0.3, 0.3));
   _modelshader.start();  
   _modelshader.setMatrix("_modeMatrix", _modelMatrix);
   _modelshader.setMatrix("_viewMatrix", _camera.getMatrix());
   _modelshader.setMatrix("_projMatrix", _projMatrix);
   _modelshader.setvec3("light.ambient", vec3(0.2, 0.2, 0.2));
   _modelshader.setvec3("light.diffuse", vec3(0.7, 0.7, 0.7));// // diffuseColor将光照调暗了一些以搭配场景
   _modelshader.setvec3("light.specular", vec3(1.0f, 1.0f, 1.0f)); 
   _modelshader.setvec3("light.Lightdirection", vec3(2.0f, -4.0f, 1.0f));
   _modelshader.setFloat("material.shininess", 32.0f);
   _modelshader.setvec3("viewPos", _camera.getPosition());
   _modelshader.setvec3("Lightcolor", vec3(1.0f, 1.0f, 1.0f));
   _modelshader.setvec3("spot_l.ambient", vec3(0.2, 0.2, 0.2));
   _modelshader.setvec3("spot_l.diffuse", vec3(0.7, 0.7, 0.7));// // diffuseColor将光照调暗了一些以搭配场景
   _modelshader.setvec3("spot_l.specular", vec3(1.0f, 1.0f, 1.0f));
   _modelshader.setFloat("spot_l.cutOff", cos(radians(12.5f)));
   _modelshader.setFloat("spot_l.outcutOff", cos(radians(17.5f)));
   _modelshader.setvec3("spot_l.spot_lightpos", _camera.getPosition());
   _modelshader.setvec3("viewPos", _camera.getPosition());
   _modelshader.setvec3("spot_l.spot_lightdir", _camera.getdirection());
    model->Draw(_modelshader);

    _modelshader.end();
    _angle += 2.0f;

}

void changeviewport(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        VAO = TriangleVAO();
        point_num = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        VAO = RectangularVAO();
        point_num = 6;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {

        VAO = ModelVAO();
        point_num = 36;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {

        _camera.move(1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {

        _camera.move(2);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {

        _camera.move(3);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {

        _camera.move(4);
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    _camera.onMouseMove(xpos, ypos);
}

 
int main(int argc,char* argv[])
{
    cout << argv[0] << endl;
    printf("请输入1或者任意表示是否需要读取图片 按1为是");
    cin >> iftexture;
    if (iftexture == 1)
    {
        printf("请输入你的图片路径（注意加上正确的后缀名）");
        cin >> _name;
    }
    
    printf("按1画三角形，按2画矩形, 按3画立方体");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                           
    GLFWwindow* window = glfwCreateWindow(800, 600, "Renderer", NULL, NULL);
    
    if (window == NULL)
    {
        cout << "创建窗口失败" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "初始化GLAD失败" << endl;
        return -1;
    }
    glViewport(0, 0, _width, _height);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetFramebufferSizeCallback(window, changeviewport);
    initshader();
    texture = creattexture(_name);
    depthFBO = creatShadowFBO();
    cube_VAO = cubemapVAO();
    cubetex = creatcubemapTex();
    _camera.setSpeed(0.1f);
    _camera.setviewMatrix(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
    model = new FF::Model("qiqi1.obj");//Texture / qiqi.obj
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        rend();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
