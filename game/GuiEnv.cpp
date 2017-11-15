#include "GuiEnv.hpp"

#include <iostream>

using namespace sf;
using namespace std;

Gui::Gui(SpriteHandler &spr, shared_ptr<RenderWindow> _app, CityEngine &_engine)
	: engine(_engine), sprHandler(spr)
{
	app = _app;
	attachValues();
	//elements.push_back(createElement("btn_zone", City::Coord<float>(42, 620), &Gui::action_btn_zone));

	font.loadFromFile("data/fonts/arial.ttf");
	
	hudTxt.setFont(font);
	hudTxt.setCharacterSize(28U);
	hudTxt.setFillColor(sf::Color::White);
}

void Gui::render(float dt) {
	for (unsigned i = 0; i < elements.size(); i++)
		elements[i]->render(app);

	int i = 0;

	for (auto &el : hudValues) {
		hudTxt.setString(el.first + ": " + to_string(*el.second));
		hudTxt.setPosition(10.f, 50.f + (float)i * 40.f);
		app->draw(hudTxt);
		++i;
	}
}

void Gui::events(Event &e) {
	if (e.type == Event::MouseMoved) {
		Vector2i mouse = Mouse::getPosition(*app);

		for (unsigned i = 0; i < elements.size(); i++)
			elements[i]->update(0, mouse);
	} else if (e.type == Event::MouseButtonReleased) {
		if (e.mouseButton.button == Mouse::Left) {
			for (unsigned i = 0; i < elements.size(); i++) {
				if (elements[i]->hover && elements[i]->action != nullptr)
					(this->*elements[i]->action)();
			}

			sf::Vector2f mp = app->mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));
			engine.clickTile((int)mp.x, (int)mp.y);
		}	
	} else if (e.type == Event::KeyReleased) {
		switch (e.key.code) {
			case Keyboard::E: engine.expandRoads(); break;
			case Keyboard::C: engine.newBuilding(City::Building::Com); break;
			case Keyboard::R: engine.newBuilding(City::Building::Res); break;
		}
	}
}

void Gui::attachValues() {
	hudValues = engine.getValues();
}

shared_ptr<Gui::GuiElement> Gui::createElement(string name, City::Coord<float> pos, Action action) {
	Sprite sn = sprHandler.create(name);
	Sprite sh = sprHandler.create(name + "_hov");
	
	return std::shared_ptr<GuiElement>(new GuiButton(action, pos, sn, sh));
}

Gui::GuiButton::GuiButton(Action _action, City::Coord<float> pos, sf::Sprite &sn, sf::Sprite &sh) : GuiElement(pos) {
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
