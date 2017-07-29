#include "IsoEngine.hpp"

using namespace sf;
using namespace std;

IsoEngine::IsoEngine(shared_ptr<SpriteHandler> _spr, std::shared_ptr<RenderWindow> _app) {
	app = _app;
	sprHandler = _spr;

	tiles["grass"] = { sprHandler->create("grass") };
	tiles["pavement"] = { sprHandler->create("pavement") };
	tiles["building1"] = { sprHandler->create("building1") };

	map_grnd	= make_shared<IsoMap>(9, 9);
	map_build	= make_shared<IsoMap>(9, 9);

	offset = origin(map_grnd);

	for (int y = 0; y < map_grnd->getH(); y++) {
		for (int x = 0; x < map_grnd->getW(); x++) {
			IsoMap::Tile t_grass;
			t_grass.iso = xy_iso(Coord<float>((float)x, (float)y));
			t_grass.screen = Coord<int>(x, y);
			t_grass.tile = "pavement";

			map_grnd->data[y][x] = make_shared<IsoMap::Tile>(t_grass);
		}
	}

	IsoMap::Tile t_building;
	t_building.iso = xy_iso(Coord<float>(4, 4));
	t_building.screen = Coord<int>(4, 4);
	t_building.tile = "building1";

	map_build->data[4][4] = make_shared<IsoMap::Tile>(t_building);
}

void IsoEngine::render() {
	for (int y = 0; y < map_grnd->getH(); y++) {
		for (int x = 0; x < map_grnd->getW(); x++) {
			shared_ptr<IsoMap::Tile> tile_g = map_grnd->data[y][x];
			shared_ptr<IsoMap::Tile> tile_b = map_build->data[y][x];

			tiles[tile_g->tile].spr.setPosition(tile_g->iso.x + offset.x, tile_g->iso.y + offset.y);
			app->draw(tiles[tile_g->tile].spr);

			if (tile_b != nullptr) {
				tiles[tile_b->tile].spr.setPosition(tile_b->iso.x + offset.x, tile_b->iso.y + offset.y);
				app->draw(tiles[tile_b->tile].spr);
			}
			
		}
	}
}

inline IsoEngine::Coord<float> IsoEngine::xy_iso(Coord<float> c) {
	return Coord<float>((c.x - c.y) * 32.f, (c.x + c.y) * 16.f);
}

inline IsoEngine::Coord<float> IsoEngine::iso_xy(Coord<float> iso) {
	return Coord<float>(iso.x / 16.f + iso.y / 32.f, iso.y / 16.f - iso.x / 32.f);
}

inline IsoEngine::Coord<float> IsoEngine::origin(std::shared_ptr<IsoMap> map) {
	return Coord<float>(512.f, 384.f - (float)(map->getH() * 16));
}

IsoEngine::IsoMap::IsoMap(int _w, int _h) {
	w = _w;
	h = _h;

	data.resize(h);
	
	for (int y = 0; y < h; y++)
		data[y].resize(w);
}