#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

PlayMode::PlayMode() {
	//TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.

	//Also, *don't* use these tiles in your game:

	{ //use tiles 0-16 as some weird dot pattern thing:
		std::array< uint8_t, 8*8 > distance;
		for (uint32_t y = 0; y < 8; ++y) {
			for (uint32_t x = 0; x < 8; ++x) {
				float d = glm::length(glm::vec2((x + 0.5f) - 4.0f, (y + 0.5f) - 4.0f));
				d /= glm::length(glm::vec2(4.0f, 4.0f));
				distance[x+8*y] = uint8_t(std::max(0,std::min(255,int32_t( 255.0f * d ))));
			}
		}
		for (uint32_t index = 0; index < 16; ++index) {
			PPU466::Tile tile;
			uint8_t t = uint8_t((255 * index) / 16);
			for (uint32_t y = 0; y < 8; ++y) {
				uint8_t bit0 = 0;
				uint8_t bit1 = 0;
				for (uint32_t x = 0; x < 8; ++x) {
					uint8_t d = distance[x+8*y];
					if (d > t) {
						bit0 |= (1 << x);
					} else {
						bit1 |= (1 << x);
					}
				}
				tile.bit0[y] = bit0;
				tile.bit1[y] = bit1;
			}
			ppu.tile_table[index] = tile;
		}
	}

	// ignore everything above
	load_tiles("tiles.asset");

	// set up darkness
	tile_table[3].bit0 = { 0,0,0,0,0,0,0,0 };
	tile_table[3].bit1 = { 0,0,0,0,0,0,0,0 };
	palette_table[3] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff)
	};

	//ppu.palette_table[7] = palette_table[0];
	//ppu.tile_table[32] = tile_table[0];
	ppu.palette_table = palette_table;
	ppu.tile_table = tile_table;

	// map editor
	std::array<bool, PPU466::BackgroundWidth * PPU466::BackgroundHeight> map = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	uint16_t wall_tile = 0x0202;
	uint16_t floor_tile = 0x0101;
	uint16_t darkness = 0x0303;
	std::cout << std::bitset<16>(darkness) << std::endl;
	uint32_t height = PPU466::BackgroundHeight;
	uint32_t width = PPU466::BackgroundWidth;
	for (uint32_t y = 0; y < height; ++y) {
		for (uint32_t x = 0; x < width; ++x) {
			if (y >= height / 2 ||
				x >= width / 2) {
				
				ppu.background[x + width * y] = darkness;
			}
			else {
				ppu.background[x + width * y] =
					map[x + (width / 2) * (height / 2 - 1 - y)] ?
						wall_tile : floor_tile;
			}
		}
	}
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	constexpr float PlayerSpeed = 30.0f;
	if (left.pressed) player_at.x -= PlayerSpeed * elapsed;
	if (right.pressed) player_at.x += PlayerSpeed * elapsed;
	if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	// just regular all white background:
	ppu.background_color = glm::u8vec4(0xff, 0xff, 0xff, 0xff);

	//tilemap gets recomputed every frame as some weird plasma thing:
	//NOTE: don't do this in your game! actually make a map or something :-)
	//for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
	//	for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
	//		//TODO: load the map
	//		ppu.background[x + PPU466::BackgroundWidth * y] = ((x + y) % 16);
	//	}
	//}

	//background scroll:
	//ppu.background_position.x = int32_t(-0.5f * player_at.x);
	//ppu.background_position.y = int32_t(-0.5f * player_at.y);
	glm::ivec2 background_position = glm::ivec2(0, 0);

	//player sprite:
	ppu.sprites[0].x = int8_t(player_at.x);
	ppu.sprites[0].y = int8_t(player_at.y);
	//ppu.sprites[0].index = 32;
	//ppu.sprites[0].attributes = 7;
	ppu.sprites[0].index = 0;
	ppu.sprites[0].attributes = 0;

	//--- actually draw ---
	ppu.draw(drawable_size);
}

void PlayMode::load_tiles(std::string tile_path) {
	std::ifstream path(tile_path);

	/* read the tiles */
	unsigned int tile_count;
	path >> tile_count;
	// 8 * 8 is the tile size
	for (uint8_t tile_idx = 0; tile_idx < tile_count; tile_idx++) {
		PPU466::Tile new_tile{
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0}
		};

		for (uint8_t i = 0; i < 8 * 8; i++) {
			unsigned int color_idx_32;
			path >> color_idx_32;
			uint8_t color_idx = static_cast<uint8_t> (color_idx_32);
			assert(color_idx < 4);
			uint8_t bit0 = color_idx & 1;
			uint8_t bit1 = (color_idx >> 1) & 1;
			// std::cout << color_idx_32 << " bit1:" << std::bitset<8>(bit1) << " bit0:" << std::bitset<8>(bit0) << std::endl;
			unsigned int x = i % 8, y = i / 8;

			new_tile.bit0[y] |= bit0 << (7 - x);
			new_tile.bit1[y] |= bit1 << (7 - x);
		}

		tile_table[tile_idx] = new_tile;
	}

	/* read the tile-pelatte mapping */
	std::unordered_map <unsigned int, unsigned int> tp_map = {};
	for (uint8_t tp_map_idx = 0; tp_map_idx < tile_count; tp_map_idx++) {
		unsigned int tile_idx, palette_idx;
		path >> tile_idx >> palette_idx;
		assert(tile_idx < 16 * 16 && palette_idx < 8);

		tp_map[tile_idx] = palette_idx;
	}

	/* read the palette */
	unsigned int palette_count;
	path >> palette_count;
	for (uint8_t palette_idx = 0; palette_idx < palette_count; palette_idx++) {
		PPU466::Palette new_palette;
		for (int i = 0; i < 4; i++) {
			unsigned int R_32, G_32, B_32, A_32;
			path >> R_32 >> G_32 >> B_32 >> A_32;
			// std::cout << R_32 << G_32 << B_32 << A_32 << std::endl;
			uint8_t R = static_cast<uint8_t> (R_32);
			uint8_t G = static_cast<uint8_t> (G_32);
			uint8_t B = static_cast<uint8_t> (B_32);
			uint8_t A = static_cast<uint8_t> (A_32);
			glm::u8vec4 RGBA(R, G, B, A);

			new_palette[i] = RGBA;
		}
		palette_table[palette_idx] = new_palette;
	}
}

