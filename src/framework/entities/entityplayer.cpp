#pragma once
#include "entityplayer.h"

void EntityPlayer::update(float seconds_elapsed)
{
	float moving_speed = this->player_speed;
	const Vector3& player_front = this->model.frontVector().normalize();
	const Vector3& player_right = this->model.rightVector().normalize();

	//Code adapted from Camera controls given in the framework skeleton
	// Run with shift
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) moving_speed *= 10;
	
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) this->model.translate(- player_front * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) this->model.translate( player_front * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) this->model.translate( - player_right * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) this->model.translate(player_right * moving_speed);

	
}
