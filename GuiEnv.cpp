#include "GuiEnv.hpp"

#include <iostream>

using namespace sf;
using namespace std;

Gui::Gui(shared_ptr<SpriteHandler> spr, shared_ptr<RenderWindow> _app) {
	app = _app;
	sprHandler = spr;

	elements.push_back(createElement("btn_zone", IsoEngine::Coord<float>(42, 620), &Gui::action_btn_zone));
}

void Gui::render(float dt) {
	for (unsigned i = 0; i < elements.size(); i++)
		elements[i]->render(app);
}

void Gui::events(Event &e) {
	if (e.type == Event::MouseMoved) {
		Vector2i mouse = Mouse::getPosition(*app);

		for (unsigned i = 0; i < elements.size(); i++)
			elements[i]->update(0, mouse);
	} if (e.type == Event::MouseButtonReleased) {
		if (e.mouseButton.button == Mouse::Left) {
			for (unsigned i = 0; i < elements.size(); i++) {
				if (elements[i]->hover && elements[i]->action != nullptr)
					(this->*elements[i]->action)();
			}
		}
	}
}

shared_ptr<Gui::GuiElement> Gui::createElement(string name, IsoEngine::Coord<float> pos, Action action) {
	Sprite sn = sprHandler->create(name);
	Sprite sh = sprHandler->create(name + "_hov");
	
	return std::shared_ptr<GuiElement>(new GuiButton(action, pos, sn, sh));
}

void Gui::action_btn_zone() {
	cout << "Action called\n";
}

Gui::GuiButton::GuiButton(Action _action, IsoEngine::Coord<float> pos, sf::Sprite &sn, sf::Sprite &sh) : GuiElement(pos) {
	action = _action;
	spr = sn;
	spr_hover = sh;
	
	sf::IntRect size = sn.getTextureRect();

	spr.setPosition(pos.x, pos.y);
	spr.setOrigin((float)size.width / 2.0f, (float)size.height / 2.0f);
	spr_hover.setPosition(pos.x, pos.y);
	spr_hover.setOrigin((float)size.width / 2.0f, (float)size.height / 2.0f);
}

void Gui::GuiButton::render(shared_ptr<RenderWindow> app) {
	if (hover)
		app->draw(spr_hover);
	else
		app->draw(spr);
}

void Gui::GuiButton::update(float dt, sf::Vector2i mouse) {
	if (spr.getGlobalBounds().contains((float)mouse.x, (float)mouse.y))
		hover = true;
	else
		hover = false;
}
