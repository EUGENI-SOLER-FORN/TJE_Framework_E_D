#pragma once
#include "framework/entities/entitymesh.h"
#include "framework/includes.h"
#include <algorithm>

enum eButtonId {
	NONE_BUTTON,
	NEXT_BUTTON,
	EXIT_BUTTON,
	BACKGROUND,
	HEALTHBAR
};

class EntityUI : public EntityMesh
{
public:
	~EntityUI() {};
	EntityUI(const Vector2& position, const Vector2& size, const Material& material);
	
	eButtonId type = NONE_BUTTON;
	Vector2 position;
	Texture* regularTexture;
	Texture* textureOnHover;
	//Texture* onClicked;
	bool is3D;
	float height;
	float width;

	void setType(eButtonId t) { this->type = t; };
	void setHoverTexture(Texture* t) { this->textureOnHover = t; };
	bool isHover();

	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};

enum eInventoryElements {
	WOOD,
	MEAT,
	INVENTORY_SIZE
};

class Inventory {
public:
	Inventory();

	// background hud
	Vector2 background_size = Vector2(300.0f, 96.f);
	EntityUI* background;
	// images for each element
	Vector2 img_size = Vector2(56.f);
	std::vector<EntityUI*> imgs;
	// quantities of each element
	std::vector<int> elements;

	// render all elements
	void render(Camera* camera) { this->background->render(camera); for (int i = 0; i < INVENTORY_SIZE; i++) if (this->elements[i]) this->imgs[i]->render(camera); };
	
	// add/substract elements
	void add(eInventoryElements i) { this->elements[i] += 1; };
	void substract(eInventoryElements i) { if(this->elements[i]) this->elements[i] -= 1; };
};
