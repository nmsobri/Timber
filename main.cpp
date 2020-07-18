#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <string>
#include "Game.h"
#include "Sound.h"
#include "Sprite.h"

int main() {
    /* Create video mode */
    sf::VideoMode vm(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* Create window object and open a window */
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

    game::Sprite spriteBackground("asset/graphic/background.png", 0, 0);
    game::Sprite spriteTree("asset/graphic/tree.png", 810, 0);
    game::Sprite spriteBee("asset/graphic/bee.png", -500, 0);
    game::Sprite spriteCloud1("asset/graphic/cloud.png", -500, 0);
    game::Sprite spriteCloud2("asset/graphic/cloud.png", -500, 0);
    game::Sprite spriteCloud3("asset/graphic/cloud.png", -500, 0);

    std::array<game::Sprite, NUM_BRANCHES> spriteBranches = {
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0),
        game::Sprite("asset/graphic/branch.png", -500, 0)};

    for (int i = 0; i < NUM_BRANCHES; i++) {
        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        spriteBranches[i].setOrigin(220, 20);
    }

    game::Sprite spritePlayer("asset/graphic/player.png", -500, 0);
    game::Sprite spriteAxe("asset/graphic/axe.png", -500, 0);
    game::Sprite spriteLog("asset/graphic/log.png", -500, 0);
    game::Sprite spriteRip("asset/graphic/rip.png", -500, 0);

    game::Sound soundChop("asset/sound/chop.wav");
    game::Sound soundDeath("asset/sound/death.wav");
    game::Sound soundOutOfTime("asset/sound/out_of_time.wav");

    sf::Font fontFile;
    sf::Text textMessage;
    sf::Text textScore;

    int score = 0;
    std::string scoreString = std::string("Score = ") + std::to_string(score);
    std::array<Side, NUM_BRANCHES> branchPositions;
    Side playerSide = Side::LEFT;
    Side prevPlayerSide = Side::LEFT;

    std::array<bool, sf::Keyboard::KeyCount> previousKeyState;
    previousKeyState.fill(false);

    fontFile.loadFromFile("asset/font/komikap.ttf");

    textMessage.setFont(fontFile);
    textScore.setFont(fontFile);

    textScore.setString(scoreString);
    textMessage.setString("Press enter to start");

    textMessage.setCharacterSize(100);
    textScore.setCharacterSize(75);

    textMessage.setFillColor(sf::Color::White);
    textScore.setFillColor(sf::Color::White);

    /* Get bounding box*/
    sf::FloatRect textMessageBounding = textMessage.getLocalBounds();

    /* Set text point origin to the center of the text */
    textMessage.setOrigin(
        (textMessageBounding.left + textMessageBounding.width) / 2.0f,
        (textMessageBounding.top + textMessageBounding.height) / 2.0f);

    textScore.setPosition(20, 20);
    textMessage.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

    sf::RectangleShape rectTimeBar;
    rectTimeBar.setSize(sf::Vector2f(TIMEBAR_WIDTH, TIMEBAR_HEIGHT));
    rectTimeBar.setFillColor(sf::Color::Red);
    rectTimeBar.setPosition(SCREEN_WIDTH - 30, 100);
    rectTimeBar.setRotation(180);

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

    bool logActive = false;
    float logSpeed = 0.0f;

    sf::Clock clock;
    bool isPaused = true;
    bool isGameOver = false;

    float remainingTime = GAME_TIME;
    float timeBarWidthPerSecond = TIMEBAR_WIDTH / remainingTime;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        /*
        *******************************************
        * Handle player input
        * *****************************************
        */

        /* Check all the window's events that were triggered since the last iteration of the loop */
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }

                    if (event.key.code == sf::Keyboard::Left && !previousKeyState[sf::Keyboard::Left]) {
                        score++;
                        logActive = true;
                        playerSide = Side::LEFT;

                        if (!isPaused && !isGameOver) {
                            soundChop.play();
                        }

                        updateBranches(score, branchPositions);
                        previousKeyState[sf::Keyboard::Left] = true;
                    }

                    if (event.key.code == sf::Keyboard::Right && !previousKeyState[sf::Keyboard::Right]) {
                        score++;
                        logActive = true;
                        playerSide = Side::RIGHT;

                        if (!isPaused && !isGameOver) {
                            soundChop.play();
                        }

                        updateBranches(score, branchPositions);
                        previousKeyState[sf::Keyboard::Right] = true;
                    }

                    if (event.key.code == sf::Keyboard::Enter && !previousKeyState[sf::Keyboard::Enter]) {
                        isPaused = !isPaused;

                        if (isGameOver) {
                            score = 0;
                            isGameOver = false;
                            remainingTime = GAME_TIME;

                            textMessage.setString("Press enter to start");

                            /* Get bounding box*/
                            textMessageBounding = textMessage.getLocalBounds();

                            /* Set text point origin to the center of the text */
                            textMessage.setOrigin(
                                (textMessageBounding.left + textMessageBounding.width) / 2.0f,
                                (textMessageBounding.top + textMessageBounding.height) / 2.0f);
                        }

                        spriteRip.setPosition(-500, 0);
                        branchPositions.fill(Side::NONE);

                        previousKeyState[sf::Keyboard::Enter] = true;
                    }
                    break;

                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Left && previousKeyState[sf::Keyboard::Left]) {
                        previousKeyState[sf::Keyboard::Left] = false;
                    }

                    if (event.key.code == sf::Keyboard::Right && previousKeyState[sf::Keyboard::Right]) {
                        previousKeyState[sf::Keyboard::Right] = false;
                    }

                    if (event.key.code == sf::Keyboard::Enter && previousKeyState[sf::Keyboard::Enter]) {
                        previousKeyState[sf::Keyboard::Enter] = false;
                    }
                    break;

                default:
                    break;
            }
        }

        /*
        *******************************************
        * Update scene
        * *****************************************
        */
        if (!isPaused && !isGameOver) {
            scoreString = std::string("Score = ") + std::to_string(score);
            textScore.setString(scoreString);

            remainingTime -= deltaTime.asSeconds();
            rectTimeBar.setSize(sf::Vector2f(remainingTime * timeBarWidthPerSecond, TIMEBAR_HEIGHT));

            if (remainingTime <= 0.0f) {
                textMessage.setString("Game Over");

                /* Get bounding box*/
                textMessageBounding = textMessage.getLocalBounds();

                /* Set text point origin to the center of the text */
                textMessage.setOrigin(
                    (textMessageBounding.left + textMessageBounding.width) / 2.0f,
                    (textMessageBounding.top + textMessageBounding.height) / 2.0f);

                isPaused = true;
                isGameOver = true;
                soundOutOfTime.play();
            }

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

            if (!logActive) {
                logSpeed = 4000;
                spriteLog.setPosition(810, 750);
            } else if (prevPlayerSide != playerSide) {
                logSpeed = 4000;
                spriteLog.setPosition(810, 750);
            } else {
                float logXMove = deltaTime.asSeconds() * logSpeed;
                float logX = (playerSide == Side::LEFT) ? spriteLog.getPosition().x - logXMove : spriteLog.getPosition().x + logXMove;
                spriteLog.setPosition(logX, 750);

                if (spriteLog.getPosition().x > SCREEN_WIDTH || spriteLog.getPosition().x + 100 < 0) {
                    logActive = false;
                }
            }

            prevPlayerSide = playerSide;

            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;

                if (branchPositions[i] == Side::LEFT) {
                    spriteBranches[i].setPosition(600, height);
                    // Flip the sprite round the other way
                    spriteBranches[i].setRotation(180);
                } else if (branchPositions[i] == Side::RIGHT) {
                    // Move the sprite to the right side
                    spriteBranches[i].setPosition(1330, height);
                    // Set the sprite rotation to normal
                    spriteBranches[i].setRotation(0);

                } else {
                    // Hide the branch
                    spriteBranches[i].setPosition(3000, height);
                }
            }

            if (playerSide == Side::LEFT) {
                spritePlayer.setScale(-1, 1);
                spritePlayer.setPosition(710, 700);
                spriteAxe.setScale(-1, 1);
                spriteAxe.setPosition(840, 810);
            } else if (playerSide == Side::RIGHT) {
                spritePlayer.setScale(1, 1);
                spritePlayer.setPosition(1200, 700);
                spriteAxe.setScale(1, 1);
                spriteAxe.setPosition(1080, 810);
            } else {
                spritePlayer.setPosition(-500, 0);
                spriteAxe.setPosition(-500, 0);
            }

            /* Check for collision */
            if (branchPositions[NUM_BRANCHES - 1] == playerSide) {
                isGameOver = true;
                soundDeath.play();
                textMessage.setString("Game Over");

                textMessageBounding = textMessage.getLocalBounds();

                textMessage.setOrigin(
                    (textMessageBounding.left + textMessageBounding.width) / 2.0f,
                    (textMessageBounding.top + textMessageBounding.height) / 2.0f);

                if (playerSide == Side::LEFT) {
                    spriteRip.setPosition(550, 745);

                } else {
                    spriteRip.setPosition(1186, 748);
                }

                spritePlayer.setPosition(-500, 0);
                spriteAxe.setPosition(-500, 0);
                spriteLog.setPosition(-500, 0);
                logActive = false;
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
        window.draw(spriteLog);
        window.draw(spriteBee);
        window.draw(textScore);
        window.draw(spriteAxe);
        window.draw(spritePlayer);

        /* Draw the branches */
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(spriteBranches[i]);
        }

        window.draw(spriteRip);
        window.draw(rectTimeBar);

        if (isPaused || isGameOver) {
            window.draw(textMessage);
        }

        /* Render what we just drew*/
        window.display();
    }
    return 0;
}