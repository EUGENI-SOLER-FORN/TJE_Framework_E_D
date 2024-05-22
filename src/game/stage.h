#pragma once
#include "framework/entities/entityui.h"
#include "world.h"

class EntityUI;
enum eButtonID;

class Stage
{
public:
	Stage() {};
	~Stage() {};

	virtual void onEnter() {};
	virtual void onLeave() {};

};

class StageManager
{
public:
	StageManager();
	~StageManager();
};

class MenuStage
{
public:
	MenuStage();
	~MenuStage();

	EntityUI* background;
	EntityUI* startbutton;


	void onButtonPressed(eButtonID buttonID) {};
};