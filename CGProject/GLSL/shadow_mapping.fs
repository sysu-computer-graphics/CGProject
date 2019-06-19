#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
	vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

struct Material {
	sampler2D diffuseTexture;
	float shininess;
};

uniform sampler2D shadowMap;
uniform Material material;

uniform float ambientStrength;
uniform float specularStrength;
uniform float diffuseStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 norm)
{
	//ת����[-1, 1]
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//ת����[0,1],������ȱȽ�
	projCoords = projCoords * 0.5 + 0.5;
	//�õ�������
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	//ƬԪ�ĵ�ǰ���
	float currentDepth = projCoords.z;
	//��ͼƫ��
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
	//��Եģ��
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
	//ƬԪ�Ƿ�����Ӱ��
	//float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	if(projCoords.z > 1.0)
        shadow = 0.0;

	return shadow;
}

void main()
{
    //FragColor = vec4(1.0); // set alle 4 vector values to 1.0
	vec3 color = texture(material.diffuseTexture, fs_in.TexCoords).rgb;
	vec3 norm = normalize(fs_in.Normal);

	vec3 ambient = ambientStrength * lightColor;
	
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * diffuseStrength;

	vec3 viewDir = normalize(viewPos - fs_in.FragPos); 
	// Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	vec3 specular = specularStrength * spec * lightColor;  

	//��Ӱ
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir, norm);
	vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

	// apply gamma correction
	float gamma = 2.2f;
    result = pow(result, vec3(1.0/gamma));
	
	FragColor = vec4(result, 1.0);
}