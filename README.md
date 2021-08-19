## D3ngine
## Трёхмерный движок основанный на рейкастинге шейдером.
![Пример работы текущей версии](https://github.com/LedinecMing/D3ngine/raw/main/Screenshots/снимок.png)
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
#include "TextRenderer.cpp"
```
SFML/Graphics.hpp - для создания окна и его управления.  
math.h - для тригонометрических вычислений  
TextRenderer.cpp - для однострочной инициализации текстов и шрифтов  
  
Далее устанавливаются константы, необходимые для корректной работы
```
const float PI = 3.14;
const float GRADUS = 0.0174;
const float SPHERE = 0.0;
const float CUBE = 1.0;
const float PLAIN = 2.0;
```
GRADUS - значение 1 градуса в радианах, перевод в радианы необходим для корректной работы тригонометрических функций glsl  
SPHERE, CUBE, PLANE - константы обозначений примитивов
```
int w = 1600;
int h = 1200;
int wd2 = w/2;
int hd2 = h/2;
bool keysInfo[10] = {false};
```
w, h - значения размера экрана в int  
wd2, hd2 - размер экрана разделенный на 2, в int.  
keysInfo - массив булевых значений нажатых клавиш  

```
sf::Vector3f camera = sf::Vector3f(-5.0, 0.0, -1.0);
sf::Vector3f angle = sf::Vector3f(0.0, 0.0, 0.0);
```
camera(Vec3) - позиция камеры  
angle(Vec3) - угол направления камеры  
  
```
sf::RenderTexture emptyTexture;
emptyTexture.create(w, h);
sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
```
Создание пустого спрайта под размер экрана  
  
Переменные для расчёта FPS
```
sf::Clock clock;
float time = clock.getElapsedTime().asSeconds();
float diff = 0.0;
```
clock - отслеживает время  
time - время на момент отрисовки последнего кадра  
diff - разница между текущим временем и time  
  
Загрузка шейдера
```
sf::Shader shader;
shader.loadFromFile("shader.frag", sf::Shader::Fragment);
```

Скорость камеры ```float speed = 0.05;```

Объекты на сцене
```
float objects[] =
{
    // type, x, y, z, xw, yw, zw, r, g, b
    PLAIN,  0, 0, -1,   0, 1, 1, 0, 1, 0,
    CUBE,   0, 0, -1,   1, 1, 1.5, 1, 0, 0,
    SPHERE, 2, 2, -1, 0.5, 0, 0, 0, 1, 1
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

shader.setUniform("uni_resolution", sf::Vector2f(w,h));
shader.setUniformArray("objects", objects, objectsLength);
```

Таблица синусов, косинусов для ускорения вычислений
```
float sinTable[180];
float cosTable[180]; 
for (int i = 0; i < 180; i++)
{
    sinTable[i] = sin(i);
    cosTable[i] = cos(i);
}
```
Передаем в шейдер
```
shader.setUniformArray("uni_sin", sinTable, 180);
shader.setUniformArray("uni_cos", cosTable, 180);
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
  camera.x=camera.x+(speed*cos(angle.x));
  camera.y=camera.y+(speed*sin( angle.x));
}
...
```

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
diff = float(clock.getElapsedTime().asSeconds())-time;
time = clock.getElapsedTime().asSeconds();
```

Отрисовка текста ( FPS и POS )
```
window.draw(getText("x"+std::to_string(camera.x)+"y"+std::to_string(camera.y)+"z"+std::to_string(camera.z), 0, 24, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
window.draw(getText(std::to_string(1/diff)+" fps", 0, 48, 24, getFont("Fonts/DejaVuSans.ttf"), sf::Color::Red));
```

Обновление содержимого экрана ```window.display();```

Больше документации нет. Пока что.
