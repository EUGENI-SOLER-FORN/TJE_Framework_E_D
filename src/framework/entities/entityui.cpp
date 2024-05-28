#pragma once
#include "entityui.h"
#include "game/game.h"
EntityUI::EntityUI(const Vector2& position, const Vector2& size, const Material& material) {
    this->position = position;
    this->width = size.x;
    this->height = size.y;
    // Store textures
    this->regularTexture = material.diffuse;
    this->textureOnHover = material.diffuse;

    this->mesh = new Mesh();
    this->mesh->createQuad(this->position.x, this->position.y, this->width, this->height, true);
    this->material = material;
}

bool EntityUI::isHover() {
    if (this->type == NONE_BUTTON || this->type == BACKGROUND) return false;
    Vector2 mouse_pos = Input::mouse_position;
    if (this->position.x - this->width/2.f > mouse_pos.x &&
        mouse_pos.y > this->position.y - this->height / 2.f &&
        mouse_pos.x > this->position.x + this->width / 2.f &&
        this->position.x + this->width / 2.f > mouse_pos.y) {
        return true;
    }
	return false;
}

void EntityUI::render(Camera* camera){
    if (!this->is3D) glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // opcional (transparència)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->material.shader->enable();

    this->material.shader->setUniform("u_model", this->model);
    this->material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    if(this->material.diffuse) this->material.shader->setTexture("u_texture", this->material.diffuse, 0);
    
    /*
    if (this->is3D) {
        float max_dist = 5.f;
        float dist = clamp(camera->eye.distance(pos3d), 0, max_dist);
        this->mesh->createQuad(this->position.x, this->position.y, this->width, this->height, true);
    }
    */

    this->mesh->render(GL_TRIANGLES);

    this->material.shader->disable();
    //Restore flags
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void EntityUI::update(float seconds_elapsed)
{
    if (this->isHover()) this->material.diffuse = this->textureOnHover;
    else this->material.diffuse = this->regularTexture;
}

Inventory::Inventory(){
    for (int i = 0; i < INVENTORY_SIZE; i++)this->elements.push_back(0);
    //center hud
    Material mat;
    Vector2 p;
    float window_width = (float)Game::instance->window_width;

    p = Vector2(window_width / 2.f, 0) + Vector2(0.5 * background_size.x, 0.5 * background_size.y);
    mat.diffuse = Texture::Get("data/textures/inventory/hud.png");
    mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    this->background = new EntityUI(p, background_size, mat);
    this->background->setType(BACKGROUND);

    p = p + Vector2(20.0f, 16.0f);
    mat.diffuse = Texture::Get("data/textures/inventory/wood.png");
    this->imgs.push_back(new EntityUI(p, img_size, mat));

    p = p + Vector2(12.0f, 0.0f);
    mat.diffuse = Texture::Get("data/textures/inventory/meat.png");
    this->imgs.push_back(new EntityUI(p, img_size, mat));
}
