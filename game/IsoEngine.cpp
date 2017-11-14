#include "IsoEngine.hpp"

#include <iostream>

using namespace sf;
using namespace std;
using namespace City;

IsoEngine::IsoEngine(const SpriteHandler &_spr, std::shared_ptr<RenderWindow> _app)
	: sprHandler(_spr), dimensions(13, 13)
{
	app = _app;
	batch.setRenderTarget(*app);

	tiles["grass"]		= { sprHandler.create("grass") };
	tiles["pavement"]	= { sprHandler.create("pavement") };
	tiles["building1"]	= { sprHandler.create("building1") };
	tiles["building2"]  = { sprHandler.create("building2") };
	tiles["building3"]  = { sprHandler.create("building3") };
	tiles["zone_res"]	= { sprHandler.create("zone_res") };
	tiles["road_c"]		= { sprHandler.create("road_c") };
	tiles["road_v"]		= { sprHandler.create("road_v") };
	tiles["road_h"]		= { sprHandler.create("road_h") };
	tiles["road_t"]		= { sprHandler.create("road_t") };
	tiles["road_b"]		= { sprHandler.create("road_b") };

	map_layers.push_back(make_shared<IsoMap>(dimensions.x, dimensions.y));
	map_layers.push_back(make_shared<IsoMap>(dimensions.x, dimensions.y));
	map_layers.push_back(make_shared<IsoMap>(dimensions.x, dimensions.y, false));

	offset = origin(map_layers[Ground]);

	hoverTile = sprHandler.create("zone_res");
}

void IsoEngine::render() {
	City::Coord<int> coord = getIsoFromMouseXY(Mouse::getPosition(*app).x, Mouse::getPosition(*app).y);

	auto pos = map_layers[Ground]->data[coord.y][coord.x]->iso;
	hoverTile.setPosition(pos.x + offset.x, pos.y + offset.y);

	for (int y = 0; y < map_layers[Ground]->getH(); y++) {
		for (int x = 0; x < map_layers[Ground]->getW(); x++) {
			for (unsigned z = 0; z < map_layers.size(); z++) {
				if (map_layers[z]->isActive()) {
					shared_ptr<IsoMap::Tile> tile = map_layers[z]->data[y][x];

					if (tile != nullptr) {
						float sx = tile->iso.x + offset.x, sy = tile->iso.y + offset.y;

						if (onScreen((int)sx, (int)sy)) {
							tiles[tile->tile].spr.setPosition(sx, sy);
							batch.draw(tiles[tile->tile].spr);
						}
					}
				}
			}
		}
	}

	batch.draw(hoverTile);
	batch.display();
}

City::Coord<int> IsoEngine::getIsoFromMouseXY(int mx, int my) {
	auto hoverPos = City::Coord<float>((float)mx, (float)my);
	hoverPos.x -= offset.x - 0;
	hoverPos.y -= offset.y - 16.f;
	hoverPos = iso_xy(hoverPos);

	int x = (int)hoverPos.x, y = (int)hoverPos.y;

	x = (x < 0) ? 0 : x;
	y = (y < 0) ? 0 : y;
	x = (x >= dimensions.x) ? dimensions.x - 1 : x;
	y = (y >= dimensions.y) ? dimensions.y - 1 : y;

	return City::Coord<int>(x, y);
}

void IsoEngine::setTile(Coord<int> position, string tile, int layer) {
	IsoMap::Tile t;
	t.iso = xy_iso(Coord<float>((float)position.x, (float)position.y));
	t.screen = Coord<int>(position.x, position.y);
	t.tile = tile;

	map_layers[layer]->data[position.y][position.x] = make_shared<IsoMap::Tile>(t);
}

inline bool IsoEngine::onScreen(int x, int y) {
	if (x >= 0 && y >= 0 && x < 1024 + 64 && y < 768 + 32)
		return true;

	return false;
}

inline Coord<float> IsoEngine::xy_iso(Coord<float> c) {
	return Coord<float>((c.x - c.y) * 32.f, (c.x + c.y) * 16.f);
}

inline Coord<float> IsoEngine::iso_xy(Coord<float> iso) {
	return Coord<float>((iso.x / 32.f + iso.y / 16.f) / 2.f, (iso.y / 16.f - iso.x / 32.f) / 2.f);
}

inline Coord<float> IsoEngine::origin(std::shared_ptr<IsoMap> map) {
	return Coord<float>(512.f, 384.f - (float)(map->getH() * 16));
}

IsoEngine::IsoMap::IsoMap(int _w, int _h, bool a) : active(a) {
	w = _w;
	h = _h;

	data.resize(h);
	
	for (int y = 0; y < h; y++)
		data[y].resize(w);
}