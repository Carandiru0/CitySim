#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class AnimatedSprite : public sf::Sprite {
	public:
		AnimatedSprite();
		AnimatedSprite(sf::Texture &tex);
		
		void initAnimation(unsigned type, int frames, float duration);
		void update();
		void setHover(bool hover = false);
		
	private:
		int playing;
		int frames;
		int frame;
		int delta_y;
		
		float milli;
		bool animated;
		bool play_once;
		
		sf::Clock timer;
		sf::IntRect box;
};