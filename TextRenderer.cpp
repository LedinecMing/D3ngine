#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

sf::Text getText(std::string string, int x, int y, unsigned int size, sf::Font font, sf::Color color = sf::Color::Black, unsigned int style = sf::Text::Style::Regular)
{
	sf::Text text = sf::Text(string, font, size);
	text.setPosition(x, y);
	text.setFillColor(color);
	text.setStyle(style);
	return text;
}
sf::Font getFont(std::string path)
{
	sf::Font font;
	font.loadFromFile(path);
	return font;
}