#include "entityai.h"

void EntityAI::update(float seconds_elapsed) {
	float moving_speed = this->speed;
	const Vector3 current_position = this->position;
	Vector3 moving_direction = this->direction;

	/*
	* CHECK COLLISIONS WITH PLAYSTAGE::PLAYER AND CHANGE STATE
	*/
	
	if (this->state == IDLE) {
		moving_direction = .9*this->direction + .1*Vector3(random(1.f) * 2-1, random(1.f) * 2 - 1, random(1.f) * 2 - 1);
	}
	else if (this->state = HIT) {
		// If hit, scape from player
		moving_direction = PlayStage::player->model.frontVector();
		moving_speed *= 2;
	}

	const Vector3 next_position = current_position + moving_direction * moving_speed * seconds_elapsed;
	this->position = next_position;
	double angle = acos( this->direction.dot(moving_direction) );

	this->model.setTranslation(next_position);
	if(angle) this->model.rotate((float)angle, Vector3::UP);

	this->direction = moving_direction;
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
}
