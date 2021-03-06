#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include "SpriteHandler.hpp"
#include "CityEngine.hpp"
#include "EngineInterface.hpp"

class Gui {
	private:
		typedef void (Gui::*Action)();

		class GuiElement {
			public:
				virtual void render(std::shared_ptr<sf::RenderWindow> app) = 0;
				virtual void update(float dt, sf::Vector2i mouse) = 0;

				Action action;
				bool hover;

			protected:
				City::Coord<float> position;
				GuiElement(City::Coord<float> pos, Action a = nullptr) : position(pos), action(a), hover(false) {}
			};

		class GuiButton : public GuiElement {
			public:
				GuiButton(Action action, City::Coord<float> coord, sf::Sprite &sn, sf::Sprite &sh);

				void render(std::shared_ptr<sf::RenderWindow> app);
				void update(float dt, sf::Vector2i mouse);

			private:
				sf::Sprite spr, spr_hover;
		};

		sf::Font font;
		sf::Text hudTxt;

		void attachValues();
		std::map<std::string, std::shared_ptr<long>> hudValues;

	public:
		Gui(SpriteHandler &sprHandler, std::shared_ptr<sf::RenderWindow> app, CityEngine &engine);

		void render(float dt);
		void events(sf::Event &e);

	private:
		CityEngine &engine;
		SpriteHandler &sprHandler;

		std::shared_ptr<sf::RenderWindow> app;
		std::vector<std::shared_ptr<GuiElement>> elements;

		std::shared_ptr<GuiElement> createElement(std::string name, City::Coord<float> pos, Action action = nullptr);
};
