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
			if ( directionToMove==FORWARD )
			{
				this->_position.x=_position.x+(-_speed*cos( _angle.x));
				this->_position.y=_position.y+(-_speed*sin( _angle.x));
			}
			else if ( directionToMove==BACKWARD )
			{
				this->_position.x=_position.x+(_speed*cos( _angle.x));
				this->_position.y=_position.y+(_speed*sin( _angle.x));
			}
			else if ( directionToMove==LEFT )
			{
				this->_position.x=_position.x+(_speed*sin( _angle.x));
				this->_position.y=_position.y+(-_speed*cos( _angle.x));	
			}
			else if ( directionToMove==RIGHT )
			{
				this->_position.x=_position.x+(-_speed*sin( _angle.x));
				this->_position.y=_position.y+(_speed*cos( _angle.x));
			}
			else if ( directionToMove==UP )
			{
				this->_position.z+=_speed;
			}
			else if ( directionToMove==DOWN )
			{
				this->_position.z-=_speed;
			}
		}

		void addAngle(sf::Vector3f angle)
		{
			_angle+=angle;
		}

		sf::Vector3f getPosition(){ return _position; }
		sf::Vector3f getAngle(){ return _angle;}
		sf::Vector3f getSize(){ return _size; }

	private:
		sf::Vector3f _position, _angle, _size;
		float _speed;
};