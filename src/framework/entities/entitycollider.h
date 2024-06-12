#pragma once
#include "framework/entities/entitymesh.h"
#include "framework/entities/entityui.h"

class EntityCollider;

struct sCollisionData {
	Vector3 colPoint;
	Vector3 colNormal;
	float distance = 0.f;
	EntityCollider* entity_collided = nullptr;
};

class EntityCollider : public EntityMesh {
public:
	bool is_dynamic = false;
	eEntityType layer = NONE_TYPE;

	EntityCollider(Mesh* mesh, Material& material) : EntityMesh(mesh, material) { this->layer = FLOOR; this->name = "Floor"; };

	bool checkPlayerCollisions(const Vector3& target_pos, std::vector<sCollisionData>& wall_collisions, std::vector<sCollisionData>& floor_collisions, eEntityType t = FLOOR);
};

class EntityDrop : public EntityCollider {
public:
	EntityDrop(Mesh* mesh, Material& material);
	
	float mask = 0.f;
	int hitpoints = 5;
	float health = 100.f;

	StatBar* healthbar;

	void hit();
	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};