#pragma once
#include <SFML/Graphics.hpp>

namespace game {

enum origin {
    LEFT_TOP,
    RIGHT_TOP,
    MIDDLE
};

class Text : public sf::Text {
    sf::Font font;

   public:
    Text(const char* filePath, size_t size, sf::Color color);
    void setPosition(float x, float y, origin o);
    virtual ~Text();
};
}  // namespace game