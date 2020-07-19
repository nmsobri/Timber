#include "include/Sprite.h"

game::Sprite::Sprite(const char* filePath) {
    texture.loadFromFile(filePath);
    this->setTexture(texture);
}

game::Sprite::Sprite(const char* filePath, float x, float y, float speed, bool active) {
    texture.loadFromFile(filePath);
    this->setTexture(texture);
    this->setPosition(x, y);
    this->m_speed = speed;
    this->m_active = active;
}

game::Sprite::~Sprite() {
}

bool game::Sprite::active() {
    return this->m_active;
}

void game::Sprite::active(bool active) {
    this->m_active = active;
}

float game::Sprite::speed() {
    return this->m_speed;
}

void game::Sprite::speed(float speed) {
    this->m_speed = speed;
}

void game::Sprite::setPosition(float x, float y) {
    this->m_x = x;
    this->m_y = y;

    sf::Sprite::setPosition(this->m_x, this->m_y);
}