
#include <src/entity/player.hpp>

Player::Player(){

}

Player::Player(Scene * _scene, Camera * _camera){
    m_scene = _scene;
    m_camera = _camera;
}

Player::~Player(){
    clear();
}