#include <SFML/System/Vector3.hpp>
#include "Constants.cpp"

class Camera
{
	public:
		Camera(sf::Vector3f position, sf::Vector3f angle, sf::Vector3f size, float speed)
		{
			_position = sf::Vector3f(position);
			_angle = sf::Vector3f(angle);
			_size = sf::Vector3f(size);
			this->_speed = speed;
		};
		~Camera(){};

		void fall()
		{
			_position+=DOWN*GRAVITY*0.01f;
		}

		void move(sf::Vector3f directionToMove)
		{
			_position+=directionToMove*_speed;
			// _position.x+=
		}

		void addAngle(sf::Vector3f angle)
		{
			_angle+=angle;
		}

		sf::Vector3f getPosition(){ return _position; }
		sf::Vector3f    getAngle(){ return _angle;    }
		sf::Vector3f     getSize(){ return _size;     }

	private:
		sf::Vector3f _position, _angle, _size;
		float _speed;
};