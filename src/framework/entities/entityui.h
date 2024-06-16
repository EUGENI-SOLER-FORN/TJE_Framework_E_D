#pragma once
#include "framework/entities/entitymesh.h"
#include "framework/audio.h"
#include <algorithm>

enum eButtonId {
	NONE_BUTTON,
	NEXT_BUTTON,
	PLAY_BUTTON,
	EXIT_BUTTON,
	PAUSE_BUTTON,
	BACKGROUND,
	HEALTHBAR
};

class EntityUI : public EntityMesh {
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
	void render(Camera* camera);
	
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

	EntityUI* icon = nullptr;
	void setSleepIcon();
	void setHungerIcon();
	void setTreeIcon();
	void setBoatIcon();

	void update_stat(float s);
	void render(Camera* camera) override;

	~StatBar() { delete icon; };
};

class TimeBar : public StatBar {
public:
	TimeBar(float s, const Vector2& position, const Vector2& size, const Material& material) : StatBar(s, position, size, material) {
		Material mat;
		this->day_icon = Texture::Get("data/textures/statbar/day_icon.tga");
		this->night_icon = Texture::Get("data/textures/statbar/night_icon.tga");
		mat.diffuse = this->day_icon;
		mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		this->icon = new EntityUI(this->position + Vector2(0.f, 0.0125f*this->height ), Vector2(this->height - 0.125f * this->height), mat);
	};
	~TimeBar();

	Texture* day_icon;
	Texture* night_icon;
	bool is_day_audio = false;
	bool is_night_audio = false; 
	Audio* day_audio;
	Audio* night_audio;
	HCHANNEL day_channel;
	HCHANNEL night_channel;

	void render(Camera* camera) override;
	void update_stat(float s);
};

class MenuGame {
public:
	MenuGame();
	EntityUI* pause;
	EntityUI* continue_button;
	EntityUI* exit_button;
	bool display_menu = false;
	bool go_menu = false;
	const Vector2& button_size = Vector2(300.f, 100.f);
	const Vector2& pause_size = Vector2(60.f);
	void render(Camera* camera);
	void update(float seconds_elapsed);
};