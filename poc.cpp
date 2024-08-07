#pragma leco app
#pragma leco add_resource "atlas.png"

import casein;
import dotz;
import quack;

enum buttons { B_UP = 0, B_DOWN, B_LEFT, B_RIGHT, B_COUNT };

static dotz::vec2 player_pos {};
static bool button_state[B_COUNT] {};

static void redraw();

static float axis(buttons n, buttons p) {
  auto ns = button_state[n];
  auto ps = button_state[p];

  if (ns && !ps) return -1;
  if (ps && !ns) return +1;
  return 0;
}

static unsigned data(quack::instance *is) {
  *is = quack::instance {
    .position = player_pos,
    .size = { 1, 1 },
    .uv0 = { 0.0f / 16.0f, 1.0f / 16.0f },
    .uv1 = { 1.0f / 16.0f, 2.0f / 16.0f },
    .multiplier = { 1, 1, 1, 1 },
  };
  return 1;
}

static void redraw() { quack::donald::data(::data); }

static void process_input() {
  dotz::vec2 d { axis(B_LEFT, B_RIGHT), axis(B_UP, B_DOWN) };
  if (dotz::length(d) < 0.001) return;

  player_pos = player_pos + d * 0.25f;
  redraw();
}

static void handle_btn(casein::keys k, buttons b) {
  casein::handle(casein::KEY_DOWN, k, [=] { button_state[b] = true; });
  casein::handle(casein::KEY_UP, k, [=] { button_state[b] = false; });
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
        .grid_pos = { 0, 0 },
        .grid_size = { 16, 16 },
    });

    atlas("atlas.png");
    data(::data);

    handle_btn(K_W, B_UP);
    handle_btn(K_S, B_DOWN);
    handle_btn(K_A, B_LEFT);
    handle_btn(K_D, B_RIGHT);

    handle(TIMER, &process_input);
  }
} i;
