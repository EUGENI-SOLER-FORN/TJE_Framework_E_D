#pragma once
#include "entityplayer.h"
#include "game/game.h"

EntityPlayer::EntityPlayer() : EntityMesh(){
	this->name = "player";
	this->model = Matrix44::IDENTITY;
	this->model.setTranslation(this->player_height + Vector3(34.5f, 0.5f, 7.5f));
	this->inventory = new Inventory();
	this->point = new PointCross();
	this->minimap = new MiniMap();
	this->menu_game = new MenuGame();

	float h = (float)Game::instance->window_height / 2.f;
	float w = (float)Game::instance->window_width / 2.f;
	Material mat1, mat2, mat3;
	mat1.color = Vector4(0.f, 1.f, 0.f, 1.f);
	mat1.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/statbar.fs");
	this->hunger = new StatBar(this->player_hunger, Vector2(30.f, h), Vector2(25.f, 200.f), mat1);
	this->hunger->setHungerIcon();
	mat2.color = Vector4(0.f, 0.f, 1.f, 1.f);
	mat2.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/statbar.fs");
	this->stamina = new StatBar(this->player_sleepiness, Vector2(60.f, h), Vector2(25.f, 200.f), mat2);
	this->stamina->setSleepIcon();

	mat3.color = Vector4(0.f, 0.f, 1.f, 1.f);
	mat3.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/timebar.fs");
	this->timebar = new TimeBar(Game::instance->time, Vector2(w, 96.f + 35.f), Vector2(300.f, 25.f), mat3);

	this->player_camera2D = new Camera();
	this->player_camera2D->view_matrix = Matrix44();
	this->player_camera2D->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);
}

EntityDrop* EntityPlayer::getEntityPointingAt(Camera* camera) {
	PlayStage* ps = dynamic_cast<PlayStage*>(Game::instance->manager->current);
	if (!ps) return nullptr;
	World* current_world = ps->scene;
	const Vector3& camera_front = (camera->center - camera->eye).normalize();
	Vector3 col_point;
	Vector3 col_normal;

	for (auto entity : current_world->root->children) {
		EntityDrop* entity_drop = dynamic_cast<EntityDrop*>(entity);
		if (!entity_drop) continue;
		if (!entity_drop->isVisible) continue;
		if (entity_drop->mesh->testRayCollision(entity_drop->model, camera->eye, camera_front, col_point, col_normal, 1.5f, false)) {
			entity_drop->mask = 1.f;
			entity_drop->healthbar->position = Vector2(Game::instance->window_width, Game::instance->window_height);
			entity_drop->healthbar->update_stat(entity_drop->health);
			return entity_drop;
		}
	}
	return nullptr;
}

void EntityPlayer::update(float seconds_elapsed){
	// lose directly
	if (Input::isKeyPressed(SDL_SCANCODE_M)) {
		this->days_counter = 30;
		StageManager::goTo("game_over");
		Game::instance->manager->current->setBackground(this->days_counter);
	}
	//win directly
	if (Input::isKeyPressed(SDL_SCANCODE_N)) {
		this->days_counter = (int)(random() * 20);
		StageManager::goTo("game_over");
		Game::instance->manager->current->setBackground(this->days_counter);
	}

	if (this->sleep_cooldown) {
		this->sleep_cooldown = clamp(this->sleep_cooldown - seconds_elapsed, 0.f, 3.f);
		return;
	}

	if (this->days_counter == 30) {
		StageManager::goTo("game_over");
		Game::instance->manager->current->setBackground(this->days_counter);
	}

	// Check if we are in a playstage, if not return;
	PlayStage* ps = dynamic_cast<PlayStage*>(Game::instance->manager->current);
	if (!ps) return;
	// @TODO : create loose and win stage
	if (ps->scene->boat) {
		if (ps->scene->boat->health >= 100.f) {
			StageManager::goTo("game_over");
			Game::instance->manager->current->setBackground(this->days_counter);
		}
	}
	// Set some variables
	float moving_speed = this->player_speed;
	Vector3 moving_direction;

	const Vector3& player_front = this->model.frontVector().normalize();
	const Vector3& player_right = this->model.rightVector().normalize();
	Vector3 current_position = position();

	// Code adapted from Camera controls given in the framework skeleton
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) && this->player_sleepiness > 10.f) moving_speed *= 2.f;
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))	moving_direction += player_front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))	moving_direction -= player_front;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))	moving_direction += player_right;
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) moving_direction -= player_right;
	// Normalize so we don't move faster diagonally and multiply with speed
	moving_direction = moving_direction.normalize() * moving_speed;
	// Set velocity vector
	this->velocity += moving_direction;

	// Check and manage possible collisions
	World* current_world = ps->scene;
	std::vector<sCollisionData> floor_collisions;
	std::vector<sCollisionData> wall_collisions;

	const Vector3 possible_position = current_position + this->velocity * seconds_elapsed;
	for (auto entity : current_world->root->children) {
		EntityCollider* entity_collider = dynamic_cast<EntityCollider*>(entity);
		EntityDrop* entity_drop = dynamic_cast<EntityDrop*>(entity);
		if (entity_collider && entity_collider->isVisible) entity_collider->checkPlayerCollisions(possible_position, wall_collisions, floor_collisions);
		if (entity_drop && entity_drop->isVisible) entity_drop->checkPlayerCollisions(possible_position, wall_collisions, floor_collisions, TREE);
	}
	// Avoid collisions with walls (GLITCHY)
	if(!wall_collisions.empty()) {
		this->velocity.x *= 0.f;
		this->velocity.z *= 0.f;
	}
	
	bool grounded = false;
	for (sCollisionData& col : floor_collisions) {
		// If normal is pointing upwards, it means it's a floor collision
		float up_factor = col.colNormal.dot(Vector3::UP);
		if (up_factor > 0.8f) {
			grounded = true;
			const Vector3 slide_direction = velocity.dot(col.colNormal) * col.colNormal;
			this->velocity.x -= slide_direction.x;
			this->velocity.y = 0.f >  this->velocity.y ? 0.f : this->velocity.y;
			this->velocity.z -= slide_direction.z;
		}
	}
	if (grounded && Input::isKeyPressed(SDL_SCANCODE_SPACE)) this->velocity.y = this->player_speed;
	//DEBUG if falling
	if (Input::isKeyPressed(SDL_SCANCODE_TAB)) this->velocity.y = 3.0f;
	const Vector3& new_position = current_position + this->velocity * seconds_elapsed;

	// Reduce velocity for next frame stopping
	this->velocity.x *= 0.5f;
	if(!grounded && this->position().y > - 10.f) this->velocity.y -= 9.8f * seconds_elapsed;
	this->velocity.z *= 0.5f;

	// Rotate to look at camera is poining
	this->model.setTranslation(new_position);
	this->model.rotate(ps->camera_yaw, Vector3::UP);
	
	this->pointingAt = getEntityPointingAt(ps->stageCamera);

	// Check "attack"
	if (this->pointingAt) {
		switch (this->pointingAt->type) {
			case TREE:
				if (Input::wasKeyPressed(SDL_SCANCODE_F) && this->player_sleepiness > 10.f) {
					this->player_sleepiness -= 10.f;
					this->pointingAt->hit();
					Audio* cut_tree;
					cut_tree->Init();
					cut_tree->Get("data/audio/cuttree_audio.wav");
					cut_tree->Play("data/audio/cuttree_audio.wav", 1.0, true);
				}
				break;
			case HOUSE:
				if (Input::wasKeyPressed(SDL_SCANCODE_F)) {
					this->sleep();
					Audio* sleep;
					sleep->Init();
					sleep->Get("data/audio/sleep_audio.wav");
					sleep->Play("data/audio/sleep_audio.wav", 1.0, true);
				}
				break;
			case BOAT:
				if (Input::wasKeyPressed(SDL_SCANCODE_F)) {
					this->pointingAt->repair();
					Audio* repair;
					repair->Init();
					repair->Get("data/audio/repair_audio.wav");
					repair->Play("data/audio/repair_audio.wav", 1.0, true);
				}
				break;
		}
	}

	// Update sleep and stamina
	this->player_sleepiness = clamp(this->player_sleepiness - 0.1f * seconds_elapsed, 0.f, 100.f);
	this->stamina->update_stat(this->player_sleepiness);
	this->player_hunger = clamp(this->player_hunger - 0.2f * seconds_elapsed, 0.f, 100.f);
	this->hunger->update_stat(this->player_hunger);
	
	
	float t = (float)((int)Game::instance->time % World::days_length) * 100.f/World::days_length;
	this->days_counter = (int)Game::instance->time / World::days_length;
	this->timebar->update_stat(t);

	this->menu_game->update(seconds_elapsed);
}

void EntityPlayer::render(Camera* camera) {
	// Render mesh (not necessary, since it's first person)
	// EntityMesh::render(camera);
	{
	//// Load shaders and mesh, get model matrix and enable shader
	//Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
	//Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");
	//Matrix44 m = this->model;
	//shader->enable();
	//// Do translation and scaling transformations
	//m.translate(this->player_height);
	//m.scale(sphere_radius, sphere_radius, sphere_radius);
	//// Set basic uniforms values
	//shader->setUniform("u_color", Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	//shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//shader->setUniform("u_model", m);
	//// Render mesh
	//mesh->render(GL_LINES);
	//m = this->model;
	//m.translate(Vector3(0.f, this->sphere_radius, 0.f));
	//m.scale(sphere_radius, sphere_radius, sphere_radius);
	//// Set basic uniforms values
	//shader->setUniform("u_color", Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	//shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//shader->setUniform("u_model", m);
	//mesh->render(GL_LINES);
	//// Disable shader
	//shader->disable();
	}

	this->menu_game->render(this->player_camera2D);
	this->hunger->render(this->player_camera2D);
	this->stamina->render(this->player_camera2D);
	this->inventory->render(this->player_camera2D);
	this->timebar->render(this->player_camera2D);
	this->point->render(this->player_camera2D);
	this->minimap->render();

	float w = (float)Game::instance->window_width / 2.f;
	float h = (float)Game::instance->window_height / 2.f;
	if (this->pointingAt) {
		switch (this->pointingAt->type) {
		case HOUSE:
			drawText(w - 60.f, h - this->inventory->background_size.y - this->timebar->height - 3.f * this->minimap->margin, "Press 'F' to restore stamina", Vector3(1.f), 2.f);
			break;
		case TREE:
			if (this->player_sleepiness > 10.f) drawText(w - 60.f, h - this->inventory->background_size.y - this->timebar->height - 3.f * this->minimap->margin, "Press 'F' to cut the tree down", Vector3(1.f), 2.f);
			else drawText(w - 40.f, h + 20.f, "NOT ENOUGH STAMINA, REST!", Vector3(1.f, 0.f, 0.f), 2.f);
			break;
		case BOAT:
			if (this->inventory->elements[WOOD]) drawText(w - 60.f, h - this->inventory->background_size.y - this->timebar->height - 3.f * this->minimap->margin, "Press 'F' to repair the boat", Vector3(1.f), 2.f);
			else drawText(w - 40.f, h + 20.f, "GO FIND SOME MORE WOOD!", Vector3(1.f, 0.f, 0.f), 2.f);
			break;
		}
	}

	drawText(w - 150.f, h*2 - 175.f, "DAYS LEFT: " + std::to_string(30-this->days_counter), Vector3(1.f), 2.f);

	if (this->sleep_cooldown) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawText(w - 240.f, h, "REST A BIT AND KEEP REPAIRING!", Vector3(1.f), 3.f);
	}
}

void EntityPlayer::sleep() {
	sleep_cooldown = 3.f;
	this->player_sleepiness = std::min(100.0f, this->player_sleepiness + 40.0f);
	this->days_counter += 1;
	Game::instance->time = this->days_counter * World::days_length;
}