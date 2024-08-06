#pragma leco app
#pragma leco add_resource "atlas.png"

import casein;
import quack;

static unsigned data(quack::instance *is) {
  *is = quack::instance{
      .position{0, 0},
      .size{1, 1},
      .uv0{0.0f / 16.0f, 1.0f / 16.0f},
      .uv1{1.0f / 16.0f, 2.0f / 16.0f},
      .multiplier{1, 1, 1, 1},
  };
  return 1;
}

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

    atlas("atlas.png");
    data(::data);
  }
} i;
