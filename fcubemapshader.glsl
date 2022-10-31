#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
in vec3 outNormal;
uniform samplerCube skybox;
uniform vec3 lightcolor;
uniform vec3 lightdirction;




void main()
{    
    
    float ambientness = 0.3f;
    vec3 ambient = lightcolor * ambientness;

    vec3 norm = normalize(outNormal);

    vec3 lightdir = normalize(-lightdirction);
    float diffuseness = 0.8f;
    float diff = max(dot(norm,lightdir),0);
    vec3 diffuse = lightcolor * diff * diffuseness;
    vec3 result = ambient + diffuse;
    FragColor = texture(skybox, TexCoords);
}