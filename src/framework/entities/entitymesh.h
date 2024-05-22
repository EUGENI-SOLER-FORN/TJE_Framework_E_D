#pragma once
#include "entity.h"
#include "graphics/material.h"

class EntityMesh : public Entity {
	Mesh* mesh;
	Material material;
};