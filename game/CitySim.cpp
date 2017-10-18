#include <SFML/Graphics.hpp>
#include <memory>
#include <thread>

#include "SpriteHandler.hpp"
#include "IsoEngine.hpp"
#include "GuiEnv.hpp"
#include "Net.hpp"
#include "CityEngine.hpp"

using namespace sf;

const short port = 15817;

void thread_network(std::shared_ptr<RenderWindow> app, Net *net, CityEngine &engine) {
	while (app->isOpen())
		net->update();
}

int main(int argc, char *argv[]) {
	/*if (argc < 2)
		return 1;

	if (strcmp(argv[0], "-sess"))
		return 1;

	std::string sessid = argv[1];*/

	std::shared_ptr<RenderWindow> app = std::make_shared<RenderWindow>(VideoMode(1024, 768), "2D City Sim");
	app->setVerticalSyncEnabled(true);

	SpriteHandler spr;
	IsoEngine    *iso = new IsoEngine(spr, app);
	CityEngine    engine(iso);
	Gui			  gui(spr, app, engine);
	Net			 *net = new Net(port);

	std::thread network(thread_network, app, net, engine);

	Clock timer;

	while (app->isOpen()) {
		Event e;

		while (app->pollEvent(e)) {
			if (e.type == Event::Closed)
				app->close();
			else if (e.type == Event::KeyReleased && e.key.code == Keyboard::Escape)
				app->close();

			gui.events(e);
		}
		
		engine.update((float)timer.getElapsedTime().asMilliseconds());

		app->clear(Color(119, 181, 254));
		
		iso->render();
		gui.render(0);

		app->display();
	}

	network.join();

	delete iso;
	delete net;

	return 0;
}