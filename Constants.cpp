#include <SFML/System/Vector3.hpp>

const float GRAVITY = 9.8;

const sf::Vector3f DOWN = sf::Vector3f(0, 0, -1);
const sf::Vector3f UP = sf::Vector3f(0, 0, 1);
const sf::Vector3f LEFT = sf::Vector3f(-1, 0, 0);
const sf::Vector3f RIGHT = sf::Vector3f(1, 0, 0);
const sf::Vector3f FORWARD = sf::Vector3f(0, 1, 0);
const sf::Vector3f BACKWARD = sf::Vector3f(0, -1, 0);

const float PI = 3.14;
const float GRADUS = 0.0174;

const float SPHERE  = 0;
const float CUBE    = 1;
const float PLAIN   = 2;
const float POLYGON = 3;