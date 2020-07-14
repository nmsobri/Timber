#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    /* Create video mode */
    sf::VideoMode vm(1920, 1080);

    /* Create window object and open a window */
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

    sf::Texture textureBackground;
    sf::Texture textureTree;
    sf::Texture textureCloud;
    sf::Texture textureBee;

    textureBackground.loadFromFile("asset/graphic/background.png");
    textureTree.loadFromFile("asset/graphic/tree.png");
    textureCloud.loadFromFile("asset/graphic/cloud.png");
    textureBee.loadFromFile("asset/graphic/bee.png");

    sf::Sprite spriteBackground;
    sf::Sprite spriteTree;
    sf::Sprite spriteBee;
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;

    spriteBackground.setTexture(textureBackground);
    spriteTree.setTexture(textureTree);
    spriteBee.setTexture(textureBee);
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteBackground.setPosition(0, 0);
    spriteTree.setPosition(810, 0);
    spriteBee.setPosition(0, 800);

    bool isBeeActive = false;
    float beeSpeed = 0.0f;
    float beeHigh = 0.0f;

    bool isCloud1Active = false;
    float cloud1Speed = 0.0f;
    float cloud1High = 0.0f;

    bool isCloud2Active = false;
    float cloud2Speed = 0.0f;
    float cloud2High = 0.0f;

    bool isCloud3Active = false;
    float cloud3Speed = 0.0f;
    float cloud3High = 0.0f;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        /*
        *******************************************
        * Handle player input
        * *****************************************
        */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        /*
        *******************************************
        * Update scene
        * *****************************************
        */
        if (!isBeeActive) {
            srand((int)time(0) * 10);
            beeSpeed = (rand() % 200) + 200;

            srand((int)time(0) * 100);
            beeHigh = (rand() % 500) + 500;

            spriteBee.setPosition(-500, beeHigh);
            isBeeActive = true;
        } else {
            auto beeXPos = deltaTime.asSeconds() * beeSpeed;
            spriteBee.setPosition(spriteBee.getPosition().x + beeXPos, spriteBee.getPosition().y);

            if (spriteBee.getPosition().x > 2000) {
                isBeeActive = false;
            }
        }

        if (!isCloud1Active) {
            srand(time(0) * 10);
            cloud1Speed = (rand() % 100) * 2;
            cloud1High = (rand() % 200);
            spriteCloud1.setPosition(-500, 0);
            isCloud1Active = true;
        } else {
            auto cloud1XPos = deltaTime.asSeconds() * cloud1Speed;
            spriteCloud1.setPosition(spriteCloud1.getPosition().x + cloud1XPos, cloud1High);

            if (spriteCloud1.getPosition().x > 2000) {
                isCloud1Active = false;
            }
        }

        if (!isCloud2Active) {
            srand(time(0) * 20);
            cloud2Speed = (rand() % 100) * 2;
            cloud2High = (rand() % 200);
            spriteCloud2.setPosition(-500, 250);
            isCloud2Active = true;
        } else {
            auto cloud2XPos = deltaTime.asSeconds() * cloud2Speed;
            spriteCloud2.setPosition(spriteCloud2.getPosition().x + cloud2XPos, cloud2High);

            if (spriteCloud2.getPosition().x > 2000) {
                isCloud2Active = false;
            }
        }

        if (!isCloud3Active) {
            srand(time(0) * 30);
            cloud3Speed = (rand() % 100) * 2;
            cloud3High = (rand() % 200);
            spriteCloud3.setPosition(-500, -500);
            isCloud3Active = true;
        } else {
            auto cloud3XPos = deltaTime.asSeconds() * cloud3Speed;
            spriteCloud3.setPosition(spriteCloud3.getPosition().x + cloud3XPos, cloud3High);

            if (spriteCloud3.getPosition().x > 2000) {
                isCloud3Active = false;
            }
        }

        /*
        *******************************************
        * Draw scene
        * *****************************************
        */

        /* Clear everything from last frame*/
        window.clear();

        /* Draw our scene here*/
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteBee);

        /* Render what we just drew*/
        window.display();
    }
    return 0;
}