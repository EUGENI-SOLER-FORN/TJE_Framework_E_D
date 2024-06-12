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

class PointCross {
public:
	PointCross();
	EntityUI* cross;
	const Vector2& cross_size = Vector2(30.f);
	void render(Camera* camera) { this->cross->render(camera); };
};

class MiniMap {
public:
	MiniMap();
	
	static Camera* minimapCamera;
	
	EntityUI* background;
	const Vector2& minimap_size = Vector2(96.f);
	const int& margin = 20;
	const Vector3& cameraheight = Vector3(0.f, 10.f, 0.f);

	void render();
};

class StatBar : public EntityUI{
public:
	float stat;
	StatBar(float s, const Vector2& position, const Vector2& size, const Material& material) : EntityUI(position, size, material) {
		this->stat = s/100.f;
	};

	EntityUI* icon;
	void setSleepIcon();
	void setHungerIcon();

	void update_stat(float s);
	void render(Camera* camera) override;
	~StatBar() { delete icon; };
};

class HealthBar {
public:
	HealthBar() {
		Material mat;
		mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/healthbar.fs");
		this->bar = new EntityUI(Vector2(0.f), Vector2(100.f, 25.f), mat);
	};
	EntityUI* bar;
	float health = 100.f;

	// TODO
	void render(Camera* camera) {};
	void update(Vector3 position, float health) {};
	~HealthBar() { delete bar; }
};