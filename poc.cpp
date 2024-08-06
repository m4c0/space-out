#pragma leco app
import casein;
import quack;

struct init {
  init() {
    using namespace casein;
    using namespace quack::donald;

    window_title = "SpaceOut proof-of-concept";

    app_name("spaceout-poc");
    max_quads(1024);

    clear_colour({});
    push_constants({
        .grid_pos = {0, 0},
        .grid_size = {16, 16},
    });

    // atlas("??");
    // data(??);
  }
};
