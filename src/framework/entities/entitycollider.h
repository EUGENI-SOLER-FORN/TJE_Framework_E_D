#pragma once
#include "entitymesh.h"

struct sCollisionData {
	Vector3 colPoint;
	Vector3 colNormal;
	bool is_collided;
};

class EntityCollider : public EntityMesh {

public:

	bool is_dynamic = false;
	eEntityType layer = NONE_TYPE;

	EntityCollider(Mesh* mesh, Material& material) : EntityMesh(mesh, material) { this->layer = FLOOR; this->name = "Floor"; };
	bool checkPlayerCollisions(const Vector3& target_pos, std::vector<sCollisionData>& collisions);
};
