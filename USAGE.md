# Usage

Everything reguarding usage in c++ is written at the end of the README.md file. To know more information about the classes and the functions, see documentantion by opening the doxygen folder going into html and openning the index.html file.

## Write custom shaders

Trinacria diesel is a batch renderer, it means that first it puts every vertex it wants to draw in a buffer, and then it draws them all at once with one draw call. The shaders have to consider that factor and should adapt.

### Textures

How we deal with textures is actually pretty simple. In the core fragment shader (the one that draws all batched things) you can just add

 `uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS]`, we will define `MAX_TEXTURE_SLOTS` before compiling the shader.

Anytime you wanna acces a index in that array use `SampleTexture`

`vec4 color = SampleTexture([name for the index variable], [the tex coordinates]);`, SampleTexture is a function wrote runtime before compiling the shader. 

### Vertices

```glsl
layout(location = 0) in vec2 position;
layout(location = 1) in int texture_index;
layout(location = 2) in vec2 texture_coordinates;
layout(location = 3) in vec3 color;
layout(location = 4) in int material_index;
```

* **position:** the position of the vertex in world space

* **texture_index:** the index of the texture in the array `u_Textures`

* **texture_coordinates:** the coordinates in [0, 1] [0, 1] in the texture

* **color:** the color of the object (use white for having a normal texture)

* **material_index:** the index of the material in the `u_Materials` array

### Materials

the materials are handled in the same way the textures are but the limit of materials is ensured in the c++ renderer code and it is 32, there's no SampleMaterial function and there's no MAX_MATERIALS_SLOTS macro, it's just a plain array of the struct Material that should be copied as it is in the custom fragment shader: 

```glsl
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;
};

uniform Material u_Materials[32];
```



# Have a good time writing your custom shader!


