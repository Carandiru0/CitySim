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