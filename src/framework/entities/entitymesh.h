#pragma once
#include "framework/entities/entity.h"
#include "framework/camera.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "graphics/shader.h"

enum eEntityType {
	NONE_TYPE = 0,
	FLOOR = 1,
	ENEMY = 2,
	TREE = 3, HOUSE = 4, BOAT = 5
};

class EntityMesh : public Entity {
public:
	EntityMesh() {
		this->mesh = new Mesh();
		this->material = Material();
		this->name = "emptymesh";
	}
	EntityMesh(Mesh* mesh, Material& material, const std::string& name = "") : Entity() {
		this->mesh = mesh;
		this->material = material;
		this->name = name;
	};
	// helper method
	Vector3 position() { return this->model.getTranslation(); };

	Mesh* mesh;
	Material material;

	eEntityType type = NONE_TYPE;
	
	// If is instanced we need the array of models to render the mesh
	bool isVisible = true;
	bool isInstanced = false;
	std::vector<Matrix44> models;

	void setType(eEntityType t) { this->type = type; }
	void changeVisibility() { this->isVisible = !this->isVisible; };

	void render(Camera* camera);
	void update(float seconds_elapsed) { for (int i = 0; i < this->children.size(); ++i) this->children[i]->update(seconds_elapsed); };
};