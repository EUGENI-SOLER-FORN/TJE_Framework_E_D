#pragma once
#include "framework/entities/entity.h"
#include "framework/entities/entitymesh.h"

class World {
public:
	World();

	Entity* root;
	static EntityMesh* sky;
	
	static Camera camera;
	static Camera camera2D;

	bool parseScene();

	void update(float seconds_elapsed) { World::sky->update(seconds_elapsed); this->root->update(seconds_elapsed); };
	void render(Camera* camera) { World::sky->render(camera); this->root->render(camera); };

	void ray_cast() {};
};


