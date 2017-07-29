#include "AnimatedSprite.h"

using namespace std;

AnimatedSprite::AnimatedSprite() : sf::Sprite() {
	animated = false;
}

AnimatedSprite::AnimatedSprite(sf::Texture &tex) : sf::Sprite(tex) {
	animated = false;
}

void AnimatedSprite::initAnimation(unsigned type, int _frames, float _duration) {
	animated	= true;
	play_once	= false;
	
	if(type == 1)
		play_once = true;
	
	frame 	= 0;
	frames 	= _frames;
	milli	= _duration;
	box		= sf::IntRect(0, 0, 128, 64);
	delta_y = box.height;
	
	setTextureRect(box);
}

void AnimatedSprite::update() {	
	if(animated) {
		if(timer.getElapsedTime().asMilliseconds() > milli) {
			frame += delta_y;
			timer.restart();
		}
		
		if(frame >= frames * delta_y) {
			if(play_once) {
				animated = false;
			} else
				frame = 0;
		}

		setTextureRect(sf::IntRect(0, 0 + frame, 128, 64));
	}
}

void AnimatedSprite::setHover(bool hover) {
	
}
