#pragma once
#include "entityai.h"

void EntityAI::update(float seconds_elapsed) {
	float moving_speed = this->speed;
	const Vector3 current_position = this->position();
	Vector3 moving_direction = this->direction;
	
	if (this->state == IDLE) {
		moving_direction = .9f*this->direction + .1f*Vector3(random(1.f) * 2-1, random(1.f) * 2 - 1, random(1.f) * 2 - 1);
	}
	else if (this->state == HIT) {
		// If hit, scape from player
		moving_direction = PlayStage::player->model.frontVector();
		moving_speed *= 2;
		this->cooldown -= seconds_elapsed;
	}
	else if (this->state == REST) {
		moving_speed = 0;
		this->cooldown -= seconds_elapsed;
		return;
	}
	const Vector3 next_position = current_position + moving_direction * moving_speed * seconds_elapsed;
	this->enemy_position = next_position;
	double angle = acos( this->direction.dot(moving_direction) );

	this->model.setTranslation(next_position);
	if(angle) this->model.rotate((float)angle *seconds_elapsed, Vector3::UP);

	this->direction = moving_direction;

	if (0.f >= this->cooldown) this->changeState();
}

void EntityAI::changeState() { 
	if (this->state == IDLE) {
		this->cooldown = 5.0f;
		this->state = HIT;
	}
	else if (this->state == HIT && this->cooldown <= 0.0f) {
		this->state = IDLE;
	}
	else if (this->state == HIT) {
		this->cooldown = 5.0f;
	}
	else if (this->state == REST) {
		this->state = IDLE;
		this->cooldown = 5.0f;
	}
}
