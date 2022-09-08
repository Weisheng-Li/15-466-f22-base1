#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <deque>
#include <cassert>
#include <bitset>
#include <unordered_map>
#include <fstream>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	// asset loading functions
	void load_tiles(std::string tile_path);

	// helper function
	bool is_collide(glm::vec2 player_pos);

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	//----- drawing handled by PPU466 -----
	// local storage for ppu assets
	std::array< uint16_t, PPU466::BackgroundWidth* PPU466::BackgroundHeight > background;
	std::array< PPU466::Tile, 16 * 16 > tile_table;
	std::array< PPU466::Palette, 8 > palette_table;

	PPU466 ppu;
};
