#include <SFML/Graphics.hpp>
#include <memory>

#include "SpriteHandler.hpp"
#include "IsoEngine.hpp"

using namespace sf;

int main() {
	std::shared_ptr<RenderWindow> app = std::make_shared<RenderWindow>(VideoMode(1024, 768), "2D City Sim");
	app->setVerticalSyncEnabled(true);

	std::shared_ptr<SpriteHandler> spr = std::make_shared<SpriteHandler>();
	std::shared_ptr<IsoEngine> iso = std::make_shared<IsoEngine>(spr, app);

	while (app->isOpen()) {
		Event e;

		while (app->pollEvent(e)) {
			if (e.type == Event::Closed)
				app->close();
			else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)
				app->close();
		}

		app->clear(Color(119, 181, 254));
		
		iso->render();

		app->display();
	}

	return 0;
}