#version 330 core
out vec4 FragColor;
in vec3 outColor;
in vec2 outUV;
in vec3 outNormal;
in vec3 outFragPos;
in vec4 _posLightSpace;

uniform sampler2D _shadowMap;
struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffusetexture;
   // sampler2D speculartexture;
}; 

uniform Material material;
struct Light
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 dirction_lightpos;
  vec3 point_lightpos[5];
  vec3 spot_lightpos;
  int  pointlightcount;
  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outcutOff;
};
uniform Light light;

//uniform sampler2D ourTexture;
uniform int iftexture;
uniform int ifLight;
uniform int ifusetex;
uniform vec3 Lightcolor;
//uniform vec4 LightPos;
uniform vec3 Lightdirection;
uniform vec3 viewPos;
uniform samplerCube skybox;

float caculateShadow(vec4 _posInLight)
{
    vec3 _posInLightNDC = _posInLight.xyz/_posInLight.w;
    vec3 _uvd = _posInLightNDC * 0.5 + 0.5;
    float _oldDepth = texture(_shadowMap,_uvd.xy).r;
    float _currDepth = _uvd.z;
    return _currDepth > _oldDepth ? 1 : 0;
}
vec3 dirction_light(vec3 norm,Light l,vec3 viewdirect)
{
    vec3 lightDir = normalize(l.dirction_lightpos);
    float diff = max(dot(lightDir,norm),0);
    
    vec3 reflectdir = reflect(-lightDir,norm);
    float _spec = pow(max(dot(viewdirect, reflectdir),0), material.shininess);


    vec3 ambient = l.ambient * material.ambient;
    
    vec3 diffuse = l.diffuse * (diff * material.diffuse);
    if(iftexture==1) 
    diffuse = l.diffuse * diff * vec3(texture(material.diffusetexture, outUV));

    vec3 specular = l.specular * (_spec * material.specular);

    return ambient+(diffuse+specular);// * (caculateShadow(_posLightSpace));
}

vec3 point_light(vec3 norm,Light l,vec3 viewdirct,vec3 fragpos,int i)
{
     vec3 lightDir = normalize(l.point_lightpos[i]-fragpos);
     float distance = length(l.point_lightpos[i]-fragpos);
     float attenuation = 1.0/(l.constant + l.linear * distance + l.quadratic * (distance * distance));
     float diff = max(dot(lightDir,norm),0);
      vec3 reflectdir = reflect(-lightDir,norm); 
     float _spec = pow(max(dot(viewdirct, reflectdir),0), material.shininess);

      vec3 ambient = l.ambient * material.ambient;

      vec3 diffuse = l.diffuse * (diff * material.diffuse);

    if(iftexture==1) 
    diffuse = l.diffuse * diff * vec3(texture(material.diffusetexture, outUV));
    vec3 specular = l.specular * (_spec * material.specular);
    
    vec3 result = ambient+diffuse+specular;
    return result * attenuation;

}

vec3 spot_light(vec3 norm,Light l,vec3 viewdirct,vec3 fragpos)
{
   vec3 lightDir = normalize(l.spot_lightpos-fragpos);
   float theta = dot(lightDir,normalize(-Lightdirection));

   float eplison = l.cutOff - l.outcutOff;

   float intensity = clamp((theta-l.outcutOff)/eplison,0.0,1.0);
   float diff = max(dot(lightDir,norm),0);
   vec3 reflectdir = reflect(-lightDir,norm); 
   float _spec = pow(max(dot(viewdirct, reflectdir),0), material.shininess);

     vec3 ambient = l.ambient * material.ambient;

     vec3 diffuse = l.diffuse * (diff * material.diffuse);

    if(iftexture==1) 
    diffuse = l.diffuse * diff * vec3(texture(material.diffusetexture, outUV));
    vec3 specular = l.specular * (_spec * material.specular);
    
    
       
    vec3 result = ambient;
   
     if(theta > light.outcutOff)
     result += intensity * diffuse +  intensity * specular ;


    return result;
}


void main()
{
   vec3 norm = normalize(outNormal);
 
    
   vec3 viewline=normalize(viewPos-outFragPos);
    float ratio = 1.00 / 1.20;
   vec3 Iray = normalize(outFragPos - viewPos);
   vec3 Rray = refract(Iray,norm,ratio);
   vec4 skyboxcolor = vec4(texture(skybox,Rray).rgb,1.0);
   vec3 output;
   output+=dirction_light(norm,light,viewline);

   /*for(int i=0;i<light.pointlightcount;i++)
    output+=point_light(norm,light,viewline,outFragPos,i);

   output+=spot_light(norm,light,viewline,outFragPos);*/
   if(iftexture==0)
   {
     if(ifusetex==1)
     FragColor = vec4(outColor * output,1.0f) * skyboxcolor;
     else if(ifusetex==0)
     FragColor =  vec4(outColor * output,1.0f);
   }

    if(iftexture==1)
   {
   if(ifusetex==1)
   FragColor = texture(material.diffusetexture,outUV) * vec4(output,1.0f) * skyboxcolor;
   else if(ifusetex==0)
   FragColor =  skyboxcolor; //vec4(outColor,1.0f);

   }
};