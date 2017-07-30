#include "IsoEngine.hpp"

using namespace sf;
using namespace std;

IsoEngine::IsoEngine(shared_ptr<SpriteHandler> _spr, std::shared_ptr<RenderWindow> _app) {
	app = _app;
	sprHandler = _spr;

	tiles["grass"] = { sprHandler->create("grass") };
	tiles["pavement"] = { sprHandler->create("pavement") };
	tiles["building1"] = { sprHandler->create("building1") };

	map_grnd	= make_shared<IsoMap>(13, 13);
	map_build	= make_shared<IsoMap>(13, 13);

	offset = origin(map_grnd);

	for (int y = 0; y < map_grnd->getH(); y++)
		for (int x = 0; x < map_grnd->getW(); x++)
			setTile(Coord<int>(x, y), "grass");

	setTile(Coord<int>(4, 4), "pavement");
	setTile(Coord<int>(4, 4), "building1", 1);
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

void IsoEngine::setTile(Coord<int> position, string tile, unsigned layer) {
	IsoMap::Tile t;
	t.iso = xy_iso(Coord<float>((float)position.x, (float)position.y));
	t.screen = Coord<int>(position.x, position.y);
	t.tile = tile;

	switch (layer) {
		case 0: map_grnd->data[position.y][position.x] = make_shared<IsoMap::Tile>(t); break;
		case 1: map_build->data[position.y][position.x] = make_shared<IsoMap::Tile>(t); break;
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