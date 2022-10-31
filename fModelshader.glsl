#version 330 core
out vec4 FragColor;

in vec2 outUV;
in vec3 outNormal;
in vec3 outFragPos;

struct Material 
{
    
    float shininess;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;
struct dirLight
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 Lightdirection;

};
uniform dirLight light;

struct spotlight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 spot_lightpos;
    vec3 spot_lightdir;
    float cutOff;
    float outcutOff;
};
uniform spotlight spot_l;
uniform vec3 viewPos;

uniform vec3 Lightcolor;



vec3 spot_light(vec3 norm,spotlight l,vec3 viewdirct,vec3 fragpos)
{
   vec3 lightDir = normalize(l.spot_lightpos-fragpos);
   float theta = dot(lightDir,normalize(-l.spot_lightdir));

   float eplison = l.cutOff - l.outcutOff;

   float intensity = clamp((theta-l.outcutOff)/eplison,0.0,1.0);
   float diff = max(dot(lightDir,norm),0);
   vec3 reflectdir = reflect(-lightDir,norm); 
   float _spec = pow(max(dot(viewdirct, reflectdir),0), material.shininess);

     vec3 ambient = l.ambient;

     vec3 diffuse = l.diffuse * diff * vec3(texture(material.texture_diffuse1, outUV));

  
    vec3 specular = l.specular * _spec * vec3(texture(material.texture_specular1, outUV));
    
    
       
    vec3 result = ambient;
   
     if(theta > l.outcutOff)
     result += intensity * diffuse +  intensity * specular ;
     

    return result;
}


void main()
{
   vec3 norm = normalize(outNormal);
 
   vec3 viewline=normalize(viewPos-outFragPos);
   
   vec3 result1=spot_light(norm,spot_l,viewline,outFragPos);

   vec3 lightDir = -normalize(light.Lightdirection);
   float diff = max(dot(lightDir,norm),0);
   
   vec3 reflectdir = reflect(-lightDir,norm);
   float _spec = pow(max(dot(lightDir, reflectdir),0), material.shininess);


   vec3 ambient = light.ambient * Lightcolor;
   
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, outUV));
   
   diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, outUV));

   vec3 specular = light.specular * (_spec * vec3(texture(material.texture_specular1, outUV)));
  
  vec3 result =  ambient + (diffuse + specular) ;

  
  FragColor = vec4(result+result1,1.0f);
};