#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <functional>

#include "TextRenderer.cpp"
#include "Camera.cpp"

int main()
{
    int windowWidth = 1600;
    int windowHeight = 1200;
    int wd2 = windowWidth/2;
    int hd2 = windowHeight/2;

    bool keysInfo[10] = {false};

    Camera camera = Camera(sf::Vector3f(-5.0, 0.0, 2.0), sf::Vector3f(0, -PI, 0), sf::Vector3f(0.5, 0.5, 20), 0.5);
    sf::RenderTexture emptyTexture;
    emptyTexture.create(windowWidth, windowHeight);

    sf::RenderTexture dataTransferTexture;
    dataTransferTexture.create(1, 1);

    sf::Clock clock;
    float lastFrameRenderTime = clock.getElapsedTime().asSeconds();
    float diferrenceBeetwenLastFrames = 0.0;

    sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
    sf::Sprite sprite1x1 = sf::Sprite();

    sf::Shader shader;
    shader.loadFromFile("shader.frag", sf::Shader::Fragment);

    float objects[] =
    {
       // type, x, y, z, xw, yw, zw, xa, ya, za, r, g, b
         PLANE, 0, 0, 1,   1,   1,   1, 1, 0, 0, 1, 1, 1,
          CUBE, 0, 0, 2,   1,   1,   1, 0.9, 0, 0, 1, 1, 1,
          SPHERE, 0, 3, 2, 0.5,   1,   1, 0.9, 0, 0, 1, 1, 1

    };
    int objectsLength = sizeof objects / sizeof objects[0];
    std::cout<<objectsLength;

    sf::Vector2f baseDistation = sf::Vector2f(1000.0, 1000.0);
    float lightPower = 2;

    shader.setUniform("lightPower", lightPower);
    shader.setUniform("baseDistation", baseDistation);

    shader.setUniform("uni_resolution", sf::Vector2f(windowWidth, windowHeight));
    shader.setUniformArray("objects", objects, objectsLength);

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "D3ngine", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        shader.setUniform("getInfo", true);

        dataTransferTexture.draw(sprite1x1, &shader);

        bool result = bool((dataTransferTexture.getTexture()).copyToImage().getPixel(0,0).r);
        if ( result )
        {
            camera.fall();
        }

        shader.setUniform("getInfo", false);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }

                if (event.key.code == sf::Keyboard::W) keysInfo[0] = true;
                else if (event.key.code == sf::Keyboard::A) keysInfo[1] = true;
                else if (event.key.code == sf::Keyboard::S) keysInfo[2] = true;
                else if (event.key.code == sf::Keyboard::D) keysInfo[3] = true;
                else if (event.key.code == sf::Keyboard::Space) keysInfo[4] = true;
                else if (event.key.code == sf::Keyboard::LShift) keysInfo[5] = true;
                else if (event.key.code == sf::Keyboard::Up) keysInfo[6] = true;
                else if (event.key.code == sf::Keyboard::Down) keysInfo[7] = true;
                else if (event.key.code == sf::Keyboard::Left) keysInfo[8] = true;              
                else if (event.key.code == sf::Keyboard::Right) keysInfo[9] = true;
            }
        
            else if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W) keysInfo[0] = false;
                else if (event.key.code == sf::Keyboard::A) keysInfo[1] = false;
                else if (event.key.code == sf::Keyboard::S) keysInfo[2] = false;
                else if (event.key.code == sf::Keyboard::D) keysInfo[3] = false;
                else if (event.key.code == sf::Keyboard::Space) keysInfo[4] = false;
                else if (event.key.code == sf::Keyboard::LShift) keysInfo[5] = false;
                else if (event.key.code == sf::Keyboard::Up) keysInfo[6] = false;
                else if (event.key.code == sf::Keyboard::Down) keysInfo[7] = false;
                else if (event.key.code == sf::Keyboard::Left) keysInfo[8] = false;              
                else if (event.key.code == sf::Keyboard::Right) keysInfo[9] = false;
            }
        }
        if (keysInfo[0])
        {
            camera.move(FORWARD);
        }
        if (keysInfo[1])
        {
            camera.move(LEFT);
        }
        if (keysInfo[2])
        {
            camera.move(BACKWARD);
        }
        if (keysInfo[3])
        {
            camera.move(RIGHT);
        }
        if (keysInfo[4])
        {
            camera.move(UP);
        }
        if (keysInfo[5])
        {
            camera.move(DOWN);
        }
        if (keysInfo[6])
        {
            camera.addAngle(sf::Vector3f(0, GRADUS, 0));
        }
        if (keysInfo[7])
        {
            camera.addAngle(sf::Vector3f(0, -GRADUS, 0));
        } 
        if (keysInfo[8])
        {
            camera.addAngle(sf::Vector3f(GRADUS, 0, 0));
        } 
        if (keysInfo[9])
        {
            camera.addAngle(sf::Vector3f(-GRADUS, 0, 0));
        } 
        shader.setUniformArray("objects", objects, objectsLength);
        window.clear();

        diferrenceBeetwenLastFrames = float(clock.getElapsedTime().asSeconds())-lastFrameRenderTime;
        lastFrameRenderTime = clock.getElapsedTime().asSeconds();

        shader.setUniform("uni_time", lastFrameRenderTime);
        shader.setUniform("cameraPosition", camera.getPosition());
        shader.setUniform("cameraAngle", camera.getAngle());
        shader.setUniform("cameraSize", camera.getSize());
 
        window.draw(emptySprite, &shader);
        window.draw(getText("x"+std::to_string(camera.getPosition().x)+"y"+std::to_string(camera.getPosition().y)+"z"+std::to_string(camera.getPosition().z), 0, 24, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
        window.draw(getText(std::to_string(1/diferrenceBeetwenLastFrames)+" fps", 0, 48, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
        window.draw(getText("x"+std::to_string(camera.getAngle().x)+"y"+std::to_string(camera.getAngle().y)+"z"+std::to_string(camera.getAngle().z), 0, 72, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));

        window.display();
    }

    return 0;
}
