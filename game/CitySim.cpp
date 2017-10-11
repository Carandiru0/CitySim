#include <SFML/Graphics.hpp>
#include <memory>

#include "SpriteHandler.hpp"
#include "IsoEngine.hpp"
#include "GuiEnv.hpp"
#include "CityEngine.hpp"

using namespace sf;

int main() {
	/*if (argc < 3)
		return 1;

	if (argv[1] != "-sess")
		return 1;

	for (int i = 0; i < argc; i++)
		std::cout << argv[i] << std::endl;

	//std::string sessid = argv[2];

	//std::cout << sessid << std::endl;*/

	std::shared_ptr<RenderWindow> app = std::make_shared<RenderWindow>(VideoMode(1024, 768), "2D City Sim");
	app->setVerticalSyncEnabled(true);

	std::shared_ptr<SpriteHandler> spr	= std::make_shared<SpriteHandler>();
	std::shared_ptr<IsoEngine> iso		= std::make_shared<IsoEngine>(spr, app);
	std::shared_ptr<CityEngine> engine	= std::make_shared<CityEngine>(iso);
	std::shared_ptr<Gui> gui			= std::make_shared<Gui>(spr, app, engine);

	Clock timer;

	while (app->isOpen()) {
		Event e;

		while (app->pollEvent(e)) {
			if (e.type == Event::Closed)
				app->close();
			else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)
				app->close();

			gui->events(e);
		}

		engine->update((float)timer.getElapsedTime().asMilliseconds());

		app->clear(Color(119, 181, 254));
		
		iso->render();
		gui->render(0);

		app->display();
	}

	return 0;
}