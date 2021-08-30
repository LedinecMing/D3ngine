## D3ngine
## Трёхмерный движок основанный на рейкастинге шейдером.
MAX_REFLECT = 16
![Пример работы текущей версии](https://github.com/LedinecMing/D3ngine/raw/shader/Screenshots/снимок.png)
MAX_REFLECT = 256
![Пример работы текущей версии 2](https://github.com/LedinecMing/D3ngine/raw/shader/Screenshots/cнимок1.png)
# Управление
`WASD` для передвижения  
`Left Right Up Down` для поворота камеры  
`Shift` `Space` для движения по высоте  
`ESCAPE` для закрытия программы
## Разбор файлов
# main.cpp ( Основной файл проекта )
В начале подключаются необходимые библиотеки и файлы
```
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <functional>

#include "TextRenderer.cpp"
#include "Camera.cpp"
```
SFML/Graphics.hpp - для создания окна и его управления.  
math.h - для тригонометрических вычислений  
TextRenderer.cpp - для однострочной инициализации текстов и шрифтов  
Camera.cpp - для работы с камерой
  
Константы подгружаются вместе с Camera.cpp из Constants.cpp


```
int windowWidth = 1600;
int windowHeight = 1200;
int wd2 = windowWidth/2;
int hd2 = windowHeight/2;

bool keysInfo[10] = {false};
```
windowWidth, windowHeight значения размера экрана в int  
wd2, hd2 - размер экрана разделенный на 2, в int.  
keysInfo - массив булевых значений нажатых клавиш  

Объект камеры ```Camera camera = Camera(sf::Vector3f(-5.0, 0.0, 2.0), sf::Vector3f(0, -PI, 0), sf::Vector3f(0.5, 0.5, 20), 0.5);```
 
```
sf::RenderTexture emptyTexture;
emptyTexture.create(windowWidth, windowHeight);
```
Создание пустого спрайта под размер экрана  

Создание текстуры 1 на 1, для передачи сообщений из шейдера в программу  

Переменные для расчёта FPS
```
sf::Clock clock;
float lastFrameRenderTime = clock.getElapsedTime().asSeconds();
float diferrenceBeetwenLastFrames = 0.0;
```
clock - отслеживает время  
lastFrameRenderTime - время на момент отрисовки последнего кадра  
differenceBeetwenLastFrames - разница между текущим временем и lastFrameRenderTime  

```
sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
sf::Sprite sprite1x1 = sf::Sprite();
```
Спрайты для отрисовки  

Загрузка шейдера
```
sf::Shader shader;
shader.loadFromFile("shader.frag", sf::Shader::Fragment);
```

Объекты на сцене
```
float objects[] =
{
    // type, x, y, z, xw, yw, zw, xa, ya, za, r, g, b
};
int objectsLength = sizeof objects / sizeof objects[0];
```
objectsLength - размер массива objects

Прорисовка и сила света
```
sf::Vector2f baseDist = sf::Vector2f(1000.0, 1000.0);
float lightPower = 2;
```

Передаём информацию в шейдер
```
shader.setUniform("lightPower", lightPower);
shader.setUniform("baseDist", baseDist);

shader.setUniform("uni_resolution", sf::Vector2f(windowWidth,windowHeight));
shader.setUniformArray("objects", objects, objectsLength);
```

Инициализация окна и включение Vsync
```
sf::RenderWindow window(sf::VideoMode(1600, 1200), "D3ngine", sf::Style::Fullscreen);
window.setVerticalSyncEnabled(true);
```

Цикл, работающий до закрытия окна
```
while (window.isOpen()){...}
```

Закрыть окно по прибытию ивента Closed
```
if (event.type == sf::Event::Closed)
  window.close();
```

Общаемся с шейдером
```
shader.setUniform("getInfo", true);

dataTransferTexture.draw(sprite1x1, &shader);

bool result = bool((dataTransferTexture.getTexture()).copyToImage().getPixel(0,0).r);
if ( result )
{
  camera.fall();
}
```

Обработка нажатий клавиш
```
else if (event.type == sf::Event::KeyPressed)
{
    if (event.key.code == sf::Keyboard::Escape)
    {
        window.close();
    }
    if (event.key.code == sf::Keyboard::W) keysInfo[0] = true;
    ...
}
else if (event.type == sf::Event::KeyReleased)
{
    if (event.key.code == sf::Keyboard::W) keysInfo[0] = false;
    ...
}
```
Изменения при keysInfo[n] = true
```
if (keysInfo[0])
{
  camera.move(DIRECTION)
}
...
```
Обновление объектов в шейдере ```shader.setUniformArray("objects", objects, objectsLength);```
Очистка окна ```window.clear();```

Обновление uniform переменных шейдера
```
shader.setUniform("uni_time", time);
shader.setUniform("camera", camera);
shader.setUniform("uni_angle", angle);
```

Отрисовка пустого спрайта с наложенной на него работой шейдера ```window.draw(emptySprite, &shader);```

Обновление fps переменных
```
diferrenceBeetwenLastFrames = float(clock.getElapsedTime().asSeconds())-lastFrameRenderTime;
lastFrameRenderTime = clock.getElapsedTime().asSeconds();
```

```
shader.setUniform("cameraPosition", camera.getPosition());
shader.setUniform("cameraAngle", camera.getAngle());
shader.setUniform("cameraSize", camera.getSize());
```
Обновляем информацию о камере в шейдере

Отрисовка текста
```
window.draw(getText("x"+std::to_string(camera.getPosition().x)+"y"+std::to_string(camera.getPosition().y)+"z"+std::to_string(camera.getPosition().z), 0, 24, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
window.draw(getText(std::to_string(1/diferrenceBeetwenLastFrames)+" fps", 0, 48, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
window.draw(getText("x"+std::to_string(camera.getAngle().x)+"y"+std::to_string(camera.getAngle().y)+"z"+std::to_string(camera.getAngle().z), 0, 72, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
```

Обновление содержимого экрана ```window.display();```

Больше документации нет. Пока что.
