#pragma once
#include "entityui.h"
#include "game/game.h"
Camera* MiniMap::minimapCamera = nullptr;

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
    
    if (this->is3D) {
        Vector3 pos3d;
        float max_dist = 5.f;
        float dist = clamp(camera->eye.distance(pos3d), 0, max_dist);
        this->mesh->createQuad(this->position.x, this->position.y, this->width, this->height, true);
    }

    this->mesh->render(GL_TRIANGLES);

    this->material.shader->disable();
    //Restore flags
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void EntityUI::update(float seconds_elapsed) {
    if (this->isHover()) this->material.diffuse = this->textureOnHover;
    else this->material.diffuse = this->regularTexture;
}

Inventory::Inventory(){
    for (int i = 0; i < INVENTORY_SIZE; i++) this->elements.push_back(0);
    //center hud
    Material mat;
    Vector2 p;
    float window_center = (float)Game::instance->window_width / 2.f;

    //center div
    p = Vector2(window_center, this->background_size.y / 2.f + 20.f);
    mat.diffuse = Texture::Get("data/textures/inventory/hud.png");
    mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    this->background = new EntityUI(p, background_size, mat);
    this->background->setType(BACKGROUND);
    
    // add margin of box
    p.y += 16.f;
    p.x += 20.0f;

    mat.diffuse = Texture::Get("data/textures/inventory/wood.png");
    this->imgs.push_back(new EntityUI(p, img_size, mat));

    // add size of each element
    p.x += 50.f;
    mat.diffuse = Texture::Get("data/textures/inventory/meat.png");
    this->imgs.push_back(new EntityUI(p, img_size, mat));
}

PointCross::PointCross(){
    const Vector2& screen_center = Vector2(0.5f * (float)Game::instance->window_width, 0.5f * (float)Game::instance->window_height);
    Material cross_mat;
    cross_mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    cross_mat.diffuse = Texture::Get("data/textures/pointcross/cross.png");
    this->cross = new EntityUI(screen_center, this->cross_size, cross_mat);
}

MiniMap::MiniMap(){
    if (!MiniMap::minimapCamera) {
        MiniMap::minimapCamera = new Camera();
        MiniMap::minimapCamera->setPerspective(60.f, 1.f, 0.1f, 100.f);
    }
    Material mat;
    float window_width = (float)Game::instance->window_width;

    const Vector2& p = Vector2(window_width - this->minimap_size.x/2.f - this->margin, this->minimap_size.y/2.f + this->margin);
    mat.diffuse = Texture::Get("data/textures/minimap_frame.png");
    mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
    this->background = new EntityUI(p, this->minimap_size, mat);
}
// Code seen in theory class
void MiniMap::render(){
    PlayStage* ps = dynamic_cast<PlayStage*>(Game::instance->manager->current);
    if (!ps) return;
    this->background->render(PlayStage::player->player_camera2D);

    const Vector2& size = Vector2((float)Game::instance->window_width, (float)Game::instance->window_height);
    glViewport((GLint)(size.x - this->minimap_size.x - this->margin/2.f), (GLint)(3.f/2.f*this->margin), (GLsizei)(this->minimap_size.x - this->margin), (GLsizei)(this->minimap_size.y - this->margin));
    glDisable(GL_DEPTH_TEST);

    // Rotate to look as player
    Matrix44 myaw;
    myaw.setRotation(ps->camera_yaw, Vector3::UP);
    const Vector3& new_up = myaw*Vector3(0.f, 0.f, 1.f);
    Vector3 eye = PlayStage::player->position();
    eye.y = 0.0f;
    
    const Vector3& center = eye;
    MiniMap::minimapCamera->lookAt(eye + cameraheight, center, new_up);
    
    ps->scene->root->render(MiniMap::minimapCamera);
    
    // reset viewport
    glEnable(GL_DEPTH_TEST); 
    glViewport(0,0, (GLsizei)size.x, (GLsizei)size.y);
}
