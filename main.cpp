#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

const float PI = 3.14;
const float GRADUS = 0.0174;
const float SPHERE = 0.0;
const float CUBE = 1.0;
const float PLANE = 2.0;

int main()
{
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    int w = 1600;
    int h = 1200;
    int wd2 = w/2;
    int hd2 = h/2;
    bool keysInfo[10] = {false, false, false, false, false, false};
    sf::Vector3f camera = sf::Vector3f(-5.0, 0.0, 1.0);
    sf::Vector3f angle = sf::Vector3f(0.0, 0.0, 0.0);
    sf::RenderTexture emptyTexture;
    emptyTexture.create(w, h);
    sf::Clock clock;
    float time = clock.getElapsedTime().asSeconds();
    float diff = 0.0;
    sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
    sf::Shader shader;
    shader.loadFromFile("shader.frag", sf::Shader::Fragment);
    float speed = 0.1;

    float objects[] =
    {
        // type, x, y, z, xw, yw, zw, r, g, b
        PLANE, 0, 0, -1,  0, 1, 1, 0, 0, 0,
        CUBE,  0, 1,  1, 10, 60, 1, 1, 1, 1,
        CUBE, 20, 1,  1, 10, 60, 1, 0, 0, 1,
        CUBE, 40, 1,  1, 10, 60, 1, 1, 0, 0
    };
    int objectsLength = sizeof objects / sizeof objects[0];

    sf::Vector2f baseDist = sf::Vector2f(1000.0, 1000.0);
    float lightPower = 8;

    shader.setUniform("lightPower", lightPower);
    shader.setUniform("baseDist", baseDist);

    shader.setUniform("uni_resolution", sf::Vector2f(w,h));
    shader.setUniformArray("objects", objects, objectsLength);

    float sinTable[180];
    float cosTable[180]; 
    for (int i = 0; i < 180; i++)
    {
        sinTable[i] = sin(i);
        cosTable[i] = cos(i);
    }
    shader.setUniformArray("uni_sin", sinTable, 180);
    shader.setUniformArray("uni_cos", cosTable, 180);

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "D3ngine", sf::Style::Fullscreen);
    window.setFramerateLimit(120);
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
            camera.x=camera.x+(speed*cos(angle.x));
            camera.y=camera.y+(speed*sin( angle.x));
        }
        if (keysInfo[1])
        {
            camera.x=camera.x+(-speed*sin( angle.x));
            camera.y=camera.y+(-speed*cos( angle.x));
        }
        if (keysInfo[2])
        {
            camera.x=camera.x+(-speed*cos( angle.x));
            camera.y=camera.y+(-speed*sin( angle.x));
        }
        if (keysInfo[3])
        {
            camera.x=camera.x+(speed*sin( angle.x));
            camera.y=camera.y+(speed*cos( angle.x));
        }
        if (keysInfo[4])
        {
            camera.z+=speed;
        }
        if (keysInfo[5])
        {
            camera.z-=speed;
        }
        if (keysInfo[6])
        {
            angle.y+=GRADUS;
        }
        if (keysInfo[7])
        {
            angle.y-=GRADUS;
        } 
        if (keysInfo[8])
        {
            angle.x+=GRADUS;
        } 
        if (keysInfo[9])
        {
            angle.x-=GRADUS;
        } 
        window.clear();
        diff = float(clock.getElapsedTime().asSeconds())-time;
        time = clock.getElapsedTime().asSeconds();
        sf::Text text;
        text.setString(std::to_string(1/diff)+" fps");
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(24);
        text.setPosition(0, 0);
        text.setFont(font); 
        shader.setUniform("uni_time", time);
        shader.setUniform("camera", camera);
        shader.setUniform("uni_angle", angle);
        window.draw(emptySprite, &shader);
        window.draw(text);
        text.setString("x"+std::to_string(camera.x)+"y"+std::to_string(camera.y)+"z"+std::to_string(camera.z));
        text.setPosition(0, 24);
        window.draw(text);
        window.display();
    }

    return 0;
}
