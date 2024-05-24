#pragma once
#include "framework/entities/entitymesh.h"
#include "framework/includes.h"

enum eButtonId {
	NONE,
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
	
	eButtonId type = NONE;
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
	void action();

	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};