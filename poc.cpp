#pragma leco app
#pragma leco add_resource "atlas.png"

import casein;
import dotz;
import quack;

static dotz::vec2 player_pos{};

static void redraw();

static void move(float dx, float dy) {
  player_pos = player_pos + dotz::vec2{dx, dy} * 0.5f;
  redraw();
}

static unsigned data(quack::instance *is) {
  *is = quack::instance{
      .position = player_pos,
      .size{1, 1},
      .uv0{0.0f / 16.0f, 1.0f / 16.0f},
      .uv1{1.0f / 16.0f, 2.0f / 16.0f},
      .multiplier{1, 1, 1, 1},
  };
  return 1;
}

static void redraw() { quack::donald::data(::data); }

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

    handle(KEY_DOWN, K_W, [] { move(0, -1); });
    handle(KEY_DOWN, K_S, [] { move(0, +1); });
    handle(KEY_DOWN, K_A, [] { move(-1, 0); });
    handle(KEY_DOWN, K_D, [] { move(+1, 0); });
  }
} i;
