#include "Trinacria/Texture.h"
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <iostream>

bool TRCN_CORE_NAMESPACE::Texture::_isTextureSlotOccupied[32];

TRCN_CORE_NAMESPACE::Texture::Texture(uint32_t id, int width, int height, int channels) : _id(id),
	_width(width), _height(height), _channels(channels)
{

}

void Trinacria::DSL::Texture::Cleanup()
{
	glDeleteTextures(1, &_id);
}

void Trinacria::DSL::Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	if (_textureSlotChosenIndex != -1)
	{
		_isTextureSlotOccupied[_textureSlotChosenIndex] = false;
	}
}

void TRCN_CORE_NAMESPACE::Texture::LoadTexture(const std::string& path, uint32_t filter)
{
	uint8_t* data = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);

	if (!data)
	{
		std::cout << "Path probabilmente invalido: Texture non trovata!\n";
		return;
	}

	glGenTextures(1, &_id);

	Bind();

	uint32_t format = 0;

	switch (_channels)
	{
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter == GL_LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);  // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(data);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Trinacria::DSL::Texture::GenerateTexture()
{
	glGenTextures(1, &_id);
}

void Trinacria::DSL::Texture::TexImage(uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data, uint32_t filter)
{
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

    _width = width;
    _height = height;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void Trinacria::DSL::Texture::BoundTexImage(uint32_t internalFormat, uint32_t format,
	uint32_t width, uint32_t height, uint32_t type, void* data, uint32_t filter)
{
	Bind();
	TexImage(internalFormat, format, width, height, type, data, filter);
}

void Trinacria::DSL::Texture::SubImage(uint32_t format, uint32_t width, uint32_t height,
	uint32_t type, void* data)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data);
}

void Trinacria::DSL::Texture::BoundSubImage(uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data)
{
	Bind();
	SubImage(format, width, height, type, data);
}

void TRCN_CORE_NAMESPACE::Texture::Bind(uint32_t textureSlot)
{
	glActiveTexture(textureSlot);
	glBindTexture(GL_TEXTURE_2D, _id);

	_isTextureSlotOccupied[textureSlot - GL_TEXTURE0] = true;

	_textureSlotChosenIndex = textureSlot;
}

void Trinacria::DSL::Texture::Bind()
{
	if (_textureSlotChosenIndex != -1)
	{
		Bind(_textureSlotChosenIndex);
		return;
	}

	for (int i = 0; i < 32; i++)
	{
		if (!_isTextureSlotOccupied[i])
		{
			Bind(i + GL_TEXTURE0);
			break;
		}
	}
}

void Trinacria::DSL::Texture::ClearTextureSlots()
{
	for (int i = 0; i < 32; i++)
	{
		_isTextureSlotOccupied[i] = false;
	}
}
