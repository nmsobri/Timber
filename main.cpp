#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

int main() {
    /* Create video mode */

    sf::VideoMode vm(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* Create window object and open a window */
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

    sf::Texture textureBackground;
    sf::Texture textureTree;
    sf::Texture textureCloud;
    sf::Texture textureBee;

    sf::Sprite spriteBackground;
    sf::Sprite spriteTree;
    sf::Sprite spriteBee;
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;

    sf::Font fontFile;
    sf::Text textMessage;
    sf::Text textScore;

    int score = 0;
    std::string scoreString = std::string("Score = ") + std::to_string(score);

    textureBackground.loadFromFile("asset/graphic/background.png");
    textureTree.loadFromFile("asset/graphic/tree.png");
    textureCloud.loadFromFile("asset/graphic/cloud.png");
    textureBee.loadFromFile("asset/graphic/bee.png");
    fontFile.loadFromFile("asset/font/komikap.ttf");

    textMessage.setFont(fontFile);
    textScore.setFont(fontFile);

    textScore.setString(scoreString);
    textMessage.setString("Press enter to start");

    textMessage.setCharacterSize(100);
    textScore.setCharacterSize(75);

    textMessage.setColor(sf::Color::White);
    textScore.setColor(sf::Color::White);

    /* Get bounding box*/
    sf::FloatRect textMessageBounding = textMessage.getLocalBounds();

    /* Set text point origin to the center of the text */
    textMessage.setOrigin(
        (textMessageBounding.left + textMessageBounding.width) / 2.0f,
        (textMessageBounding.top + textMessageBounding.height) / 2.0f);

    textScore.setPosition(20, 20);
    textMessage.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

    spriteBackground.setTexture(textureBackground);
    spriteTree.setTexture(textureTree);
    spriteBee.setTexture(textureBee);
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteBackground.setPosition(0, 0);
    spriteTree.setPosition(810, 0);

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
    bool isPaused = true;

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            isPaused = !isPaused;
        }

        /*
        *******************************************
        * Update scene
        * *****************************************
        */
        if (!isPaused) {
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

                if (spriteBee.getPosition().x > SCREEN_WIDTH) {
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

                if (spriteCloud1.getPosition().x > SCREEN_WIDTH) {
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

                if (spriteCloud2.getPosition().x > SCREEN_WIDTH) {
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

                if (spriteCloud3.getPosition().x > SCREEN_WIDTH) {
                    isCloud3Active = false;
                }
            }

            /*
            *******************************************
            * Draw scene
            *******************************************
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
            window.draw(textScore);

            if (isPaused) {
                window.draw(textMessage);
            }

            /* Render what we just drew*/
            window.display();
        }
    }
    return 0;
}