#version 330 core

in vec2 TexCoords;
flat in int Index;
in vec4 Color;
flat in int MaterialIndex;
in vec2 FragPos;
out vec4 FragColor;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS]; // no tex, tex1, tex...

struct PointLight
{
	vec3 LightColor;
	vec3 LightPos;
	float Attenuation;
};

struct SpotLight
{
	vec3 LightPos;
	vec3 LightColor;
	float Attenuation;
	vec2 LightDirection;
	float InnerCutOff;
	float OuterCutOff;
};


struct DirectionalLight
{
	vec2 LightDirection;
	vec3 LightColor;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

uniform Material u_Materials[32];

uniform vec2 u_ViewPos;

uniform sampler2D u_PointLights;
uniform sampler2D u_SpotLights;
uniform sampler2D u_DirLights;

vec3 CalculateLight(vec3 lightPos, float attenuation, vec3 lightColor,
					float diffuseStrength, float specularStrength, vec2 viewPos, vec3 color);

vec3 CalculateDirectionalLight(vec2 lightDir, vec2 viewPos, vec3 lightColor,
							   float specStrength, float diffuseStrength, vec3 baseColor);

vec4 SumAllLights(vec3 baseColor, float alpha);

PointLight FetchPointLight(int index);
SpotLight FetchSpotLight(int index);
DirectionalLight FetchDirectionalLight(int index);

uniform float u_AmbientStrength;

void main()
{
	float specularStrength = 0.5f;
	float diffuseStrength = 10.f;


	if(Index != 0)
	{
        /*texture(u_Textures[Index], vec2(TexCoords.x, 1 - TexCoords.y))*/
		vec4 color = SampleTexture(Index, vec2(TexCoords.x, 1 - TexCoords.y));

		if(u_AmbientStrength != 1.f)
		{
			FragColor = SumAllLights(color.rgb * Color.rgb, Color.a * color.a);
		}
		else
		{
			FragColor = color * Color;
		}
	}
	else
	{
		if(u_AmbientStrength != 1.f)
		{
			FragColor = SumAllLights(Color.rgb, Color.a);
		}
		else
		{
			FragColor = Color;
		}
	}

}

vec3 CalculateLight(vec3 lightPos, float attenuation, vec3 lightColor,
					float diffuseStrength, float specularStrength, vec2 viewPos, vec3 color)
{
	vec3 lightDir = normalize(lightPos - vec3(FragPos, 0.f));

	float dist = length(lightPos - vec3(FragPos, 0.f));
	float diff = 1.0 / (1.0 + attenuation * dist * dist);
	vec3 diffuse = diff * lightColor * diffuseStrength * u_Materials[MaterialIndex].diffuse;

	vec3 viewDir = normalize(vec3(viewPos, 6.f) - vec3(FragPos, 0.f));;
	vec3 reflectDir = reflect(-lightDir, vec3(0.f, 0.f, 1.f));

	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(vec3(0.f, 0.f, 1.f), halfDir), 0.f), u_Materials[MaterialIndex].shininess);
	vec3 specular = spec * specularStrength * lightColor * u_Materials[MaterialIndex].specular;

	return min(color * (u_AmbientStrength * u_Materials[MaterialIndex].ambient + diffuse + specular), vec3(1.f));
}

vec4 SumAllLights(vec3 baseColor, float alpha)
{

	vec3 pointLightWeight = vec3(0);

	float specularStrength = 0.5f;
	float diffuseStrength = 10.f;

	for(int i = 0; i < (textureSize(u_PointLights, 0).x / 2); i++)
	{
		PointLight pointLight = FetchPointLight(i);

		if(pointLight.LightColor == vec3(0.f))
		{
			break;
		}

		 pointLightWeight += CalculateLight(pointLight.LightPos, pointLight.Attenuation,
											   pointLight.LightColor,
											   diffuseStrength, specularStrength, u_ViewPos,
											   baseColor);
	}


	vec3 spotLightWeight = vec3(0.f);

	for(int i = 0; i < textureSize(u_SpotLights, 0).x / 3; i++)
	{
		SpotLight spotLight = FetchSpotLight(i);

		if(spotLight.LightColor == vec3(0.f))
		{
			break;
		}

		vec2 spotLightDirection = normalize(spotLight.LightDirection);

		vec2 lightDir = vec2(normalize(spotLight.LightPos - vec3(FragPos, 0.f)));

		float theta = dot(lightDir, normalize(-spotLightDirection));
		float epsilon = spotLight.InnerCutOff - spotLight.OuterCutOff;
		float intesity = clamp((theta - spotLight.OuterCutOff) / epsilon, 0.f, 1.f);

		vec3 localSpotLightWeight = CalculateLight(spotLight.LightPos, spotLight.Attenuation,
												   spotLight.LightColor, 2.f, specularStrength,
												   u_ViewPos, baseColor);

		localSpotLightWeight = localSpotLightWeight * intesity;

		spotLightWeight += localSpotLightWeight;
	}


	vec3 dirLightWeight = vec3(0.f);

	for(int i = 0; i < textureSize(u_DirLights, 0).x / 2; i++)
	{
		DirectionalLight dirLight = FetchDirectionalLight(i);

		if(dirLight.LightColor == vec3(0.f))
		{
			break;
		}

		dirLightWeight += CalculateDirectionalLight(dirLight.LightDirection,
														  u_ViewPos,
														  dirLight.LightColor, specularStrength, 1.f, baseColor);
	}



	return  vec4(pointLightWeight + spotLightWeight  + dirLightWeight, alpha);
}

vec3 CalculateDirectionalLight(vec2 lightDir, vec2 viewPos, vec3 lightColor,
							   float specStrength, float diffuseStrength, vec3 baseColor)
{

	vec3 N = vec3(0.f, 0.f, 1.f);
	vec3 L = normalize(vec3(lightDir, 0.4f));
	vec3 V = vec3(0.f, 0.f, 1.f);

	float diff = max(dot(N, L), 0.0);

	vec3 H = normalize(L + V);
	float spec = 0.0;
	if (diff > 0.0) {
		spec = pow(max(dot(N, H), 0.0), 32);
	}

	vec3 ambient = u_AmbientStrength * u_Materials[MaterialIndex].ambient;
	vec3 diffuse = diff * u_Materials[MaterialIndex].diffuse * lightColor * diffuseStrength;
	vec3 specular = specStrength * spec * lightColor * u_Materials[MaterialIndex].specular;

	return min((ambient + diffuse + specular) * baseColor, 1.f);
}

PointLight FetchPointLight(int index)
{
	int x = index * 2; // point light occupates 2 texels;

	vec4 t0 = texelFetch(u_PointLights, ivec2(x, 0), 0);
	vec4 t1 = texelFetch(u_PointLights, ivec2(x + 1, 0), 0);

	PointLight light;

	light.LightColor = t0.rgb;
	light.LightPos = vec3(t0.a, t1.r, t1.g);
	light.Attenuation = t1.b;

	return light;
}

SpotLight FetchSpotLight(int index)
{
	int x = index * 3; // spot light occupatess 3 texels

	vec4 t0 = texelFetch(u_SpotLights, ivec2(x, 0), 0);
	vec4 t1 = texelFetch(u_SpotLights, ivec2(x + 1, 0), 0);
	vec4 t2 = texelFetch(u_SpotLights, ivec2(x + 2, 0), 0);

	SpotLight light;

	light.LightColor = t0.rgb;
	light.LightPos = vec3(t0.a, t1.r, t2.b);
	light.LightDirection = t1.gb;
	light.Attenuation = t1.a;
	light.InnerCutOff = t2.r;
	light.OuterCutOff = t2.g;

	return light;
}

DirectionalLight FetchDirectionalLight(int index)
{
	int x = index * 2; // directional light occupates 2 texels ( 5 byte )

	vec4 t0 = texelFetch(u_DirLights, ivec2(x, 0), 0);
	vec4 t1 = texelFetch(u_DirLights, ivec2(x + 1, 0), 0);

	DirectionalLight light;

	light.LightColor = t0.rgb;
	light.LightDirection = vec2(t0.a, t1.r);

	return light;
}