#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
int main()
{
    int w = 1600;
    int h = 1200;
    int wd2 = w/2;
    int hd2 = h/2;
    float PI = 3.14;
    float gradus = 0.0174;
    bool keysInfo[10] = {false, false, false, false, false, false};
    sf::Vector3f camera = sf::Vector3f(-5.0, 0.0, 0.0);
    sf::Vector3f angle = sf::Vector3f(0.0, 0.0, 0.0);
    sf::RenderTexture emptyTexture;
    emptyTexture.create(w, h);
    sf::Clock clock;
    sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
    sf::Shader shader;
    shader.loadFromFile("shader.frag", sf::Shader::Fragment);
    shader.setUniform("uni_resolution", sf::Vector2f(w,h));
    sf::RenderWindow window(sf::VideoMode(1600, 1200), "D3ngine", sf::Style::Fullscreen);
    window.setFramerateLimit(30);
    while (window.isOpen())
    {
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
            camera.x=camera.x+(0.1*cos( angle.x));
            camera.y=camera.y+(0.1*sin( angle.x));
        }
        if (keysInfo[1])
        {
            camera.x=camera.x+(-0.1*sin( angle.x));
            camera.y=camera.y+(-0.1*cos( angle.x));
        }
        if (keysInfo[2])
        {
            camera.x=camera.x+(-0.1*cos( angle.x));
            camera.y=camera.y+(-0.1*sin( angle.x));
        }
        if (keysInfo[3])
        {
            camera.x=camera.x+(0.1*sin( angle.x));
            camera.y=camera.y+(0.1*cos( angle.x));
        }
        if (keysInfo[4])
        {
            camera.z+=0.1;
        }
        if (keysInfo[5])
        {
            camera.z-=0.1;
        }
        if (keysInfo[6])
        {
            angle.y+=gradus;
        }
        if (keysInfo[7])
        {
            angle.y-=gradus;
        } 
        if (keysInfo[8])
        {
            angle.x+=gradus;
        } 
        if (keysInfo[9])
        {
            angle.x-=gradus;
        } 
        window.clear();
        shader.setUniform("uni_time", clock.getElapsedTime().asSeconds());
        shader.setUniform("camera", camera);
        //shader.setUniform("uni_angle", sf::Vector3f(( angle.x* 180 ) / PI, ( angle.y * 180 ) / PI, ( angle.z * 180 ) / PI));
        shader.setUniform("uni_angle", angle);
        window.draw(emptySprite, &shader);
        window.display();
    }

    return 0;
}
