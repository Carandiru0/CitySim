#include <SFML/Graphics.hpp>

using namespace sf;

int main() {
	RenderWindow app(VideoMode(1024, 768), "2D City Sim");

	while (app.isOpen()) {
		Event e;

		while (app.pollEvent(e)) {
			if (e.type == Event::Closed)
				app.close();
		}

		app.clear();
		app.display();
	}

	return 0;
}