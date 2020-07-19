#include <SFML/Graphics.hpp>

namespace game {

class Sprite : public sf::Sprite {
    sf::Texture texture;
    bool m_active;
    float m_x;
    float m_y;
    float m_speed;

   public:
    Sprite() = delete;
    Sprite(const char* filePath);
    Sprite(const char* filePath, float x, float y, float speed = 0.0f, bool active = false);
    virtual ~Sprite();
    bool active();
    void active(bool active);
    float speed();
    void speed(float active);
    void setPosition(float x, float y);
};

}  // namespace game
