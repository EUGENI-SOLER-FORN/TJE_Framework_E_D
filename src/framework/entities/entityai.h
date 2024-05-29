#pragma once
#include "framework/entities/entitycollider.h"
#include "game/stage.h"

enum eState{ IDLE, HIT, REST, STATE_NUM };
class EntityAI : public EntityCollider{
public:
	EntityAI(Mesh* mesh, Material& material) : EntityCollider(mesh, material) { this->type = ENEMY; };
	~EntityAI() {};

	eState state = IDLE;
	float cooldown;
	float speed = 0.01f;
	float health = 100.0f;
	Vector3 direction = Vector3();
	Vector3 enemy_position = Vector3();
	void render(Camera* camera) override { EntityMesh::render(camera); };
	void update(float seconds_elapsed) override;
	void changeState();
};
