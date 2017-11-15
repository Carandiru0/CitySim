#pragma once

#include <SFML\Graphics.hpp>

using namespace sf;

class SpriteBatch {
	public:
		SpriteBatch();
		~SpriteBatch();

		void setRenderTarget(RenderTarget &rt) {
			this->rt = &rt;
		}

		void display(bool reset = true);
		void draw(Sprite &sprite);
		void draw(const Texture *texture, Vector2f position, IntRect rec, Color color, Vector2f scale, Vector2f origin, float rotation = 0);
		void draw(const Texture *texture, FloatRect dest, IntRect rec, Color color);
	private:
		RenderTarget *rt;
		RenderStates state;
		Vertex *vertices;

		int count;
		int capacity;
};

class sfLine : public sf::Drawable {
	public:
		sfLine() {}

		sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2) :
			color(sf::Color::Red), thickness(5.f)
		{
			set(point1, point2);
		}

		void set(const sf::Vector2f &point1, const sf::Vector2f &point2) {
			sf::Vector2f direction = point2 - point1;
			sf::Vector2f unitDirection = direction / std::sqrt(direction.x*direction.x + direction.y*direction.y);
			sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

			sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

			vertices[0].position = point1 + offset;
			vertices[1].position = point2 + offset;
			vertices[2].position = point2 - offset;
			vertices[3].position = point1 - offset;

			for (int i = 0; i < 4; ++i)
				vertices[i].color = color;
		}

		void draw(sf::RenderTarget &target, sf::RenderStates states) const {
			target.draw(vertices, 4, sf::Quads);
		}


	private:
		sf::Vertex vertices[4];
		float thickness;
		sf::Color color;
};