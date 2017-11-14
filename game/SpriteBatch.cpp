#include "SpriteBatch.hpp"

#include <math.h>

const float Pi = 3.14159265f;
const int MaxCapacity = 400000;
const int LookupSize = 512;

float getSin[LookupSize];
float getCos[LookupSize];
bool initialized = false;

void create_lookup() {
	for (int i = 0; i < LookupSize; i++) {
		getSin[i] = sin(i * Pi / LookupSize * 2);
		getCos[i] = cos(i * Pi / LookupSize * 2);
	}

	initialized = true;
}

SpriteBatch::SpriteBatch() {
	count = 0;
	capacity = 20000;
	vertices = new Vertex[capacity];

	if (!initialized)
		create_lookup();
}


SpriteBatch::~SpriteBatch() {
	delete[] vertices;
	vertices = NULL;
}

void SpriteBatch::draw(Sprite &sprite) {
	draw(sprite.getTexture(), sprite.getPosition(), sprite.getTextureRect(), sprite.getColor(), sprite.getScale(), sprite.getOrigin(), sprite.getRotation());
}

void SpriteBatch::display(bool reset) {
	rt->draw(vertices, count * 4, PrimitiveType::Quads, state);
	count = 0;

	if (reset) state = RenderStates();
}

void SpriteBatch::draw(const Texture *texture, Vector2f position, IntRect rec, Color color, Vector2f scale, Vector2f origin, float rotation) {
	if (texture != state.texture) {
		display(false);
		state.texture = texture;
	}

	if (count * 4 >= capacity) {
		display(false);

		if (capacity < MaxCapacity) {
			delete[] vertices;
			capacity *= 2;
			if (capacity > MaxCapacity) capacity = MaxCapacity;
			vertices = new Vertex[capacity];
		}
	}

	int rot = (int)(rotation / 360 * LookupSize) & (LookupSize - 1);
	float _sin = getSin[rot];
	float _cos = getCos[rot];

	origin.x *= scale.x;
	origin.y *= scale.y;
	scale.x *= rec.width;
	scale.y *= rec.height;

	Vertex v;

	v.color = color;

	float pX, pY;

	pX = -origin.x;
	pY = -origin.y;
	v.position.x = pX * _cos - pY * _sin + position.x;
	v.position.y = pX * _sin + pY * _cos + position.y;
	v.texCoords.x = (float)rec.left;
	v.texCoords.y = (float)rec.top;
	vertices[count * 4 + 0] = v;

	pX += scale.x;
	v.position.x = pX * _cos - pY * _sin + position.x;
	v.position.y = pX * _sin + pY * _cos + position.y;
	v.texCoords.x += rec.width;
	vertices[count * 4 + 1] = v;

	pY += scale.y;
	v.position.x = pX * _cos - pY * _sin + position.x;
	v.position.y = pX * _sin + pY * _cos + position.y;
	v.texCoords.y += rec.height;
	vertices[count * 4 + 2] = v;

	pX -= scale.x;
	v.position.x = pX * _cos - pY * _sin + position.x;
	v.position.y = pX * _sin + pY * _cos + position.y;
	v.texCoords.x -= rec.width;
	vertices[count * 4 + 3] = v;

	count++;
}

void SpriteBatch::draw(const Texture *texture, FloatRect dest, IntRect rec, Color color) {
	draw(texture, Vector2f(dest.left, dest.top), rec, color, Vector2f(1, 1), Vector2f(0, 0), 0);
}