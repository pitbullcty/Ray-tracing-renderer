#include "Mesh.h"


void Mesh::destoryTextures()
{
	for (auto& texture : textures) {
		texture.texture->destroy();
	}
}