#pragma leco app
#pragma leco add_resource "atlas.png"

import casein;
import dotz;
import hai;
import quack;
import silog;

enum buttons { B_UP = 0, B_DOWN, B_LEFT, B_RIGHT, B_COUNT };

static dotz::vec2 camera_pos {};
static dotz::vec2 player_pos {};
static bool button_state[B_COUNT] {};

static hai::varray<dotz::vec2> dots { 1024 };

static void redraw();

static float axis(buttons n, buttons p) {
  auto ns = button_state[n];
  auto ps = button_state[p];

  if (ns && !ps) return -1;
  if (ps && !ns) return +1;
  return 0;
}

static void blit(quack::instance *&is, quack::instance i) {
  i.position = i.position - camera_pos;
  i.size = { 1 };
  i.uv0 = i.uv0 / 16.0f;
  i.uv1 = i.uv1 / 16.0f;
  i.multiplier = { 1 };
  *is++ = i;
}

static unsigned data(quack::instance * is) {
  const auto b = is;

  blit(is, quack::instance {
    .position = player_pos,
    .uv0 = {0, 1},
    .uv1 = {1, 2},
  });

  for (auto p : dots) {
    blit(is, quack::instance {
      .position = p,
      .uv0 = {0, 3},
      .uv1 = {1, 4},
    });
  }

  auto cursor = dotz::floor(quack::donald::mouse_pos());
  if (dotz::length(cursor - player_pos) <= 3) {
    blit(is, quack::instance {
      .position = cursor,
      .uv0 = {1, 0},
      .uv1 = {2, 1},
    });
  }

  return is - b;
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

static void ctor() {
  auto cursor = dotz::floor(quack::donald::mouse_pos());
  if (dotz::length(cursor - player_pos) <= 3) {
    dots.push_back(cursor);
  }
}
static void dtor() {
  auto cursor = dotz::floor(quack::donald::mouse_pos());
  if (dotz::length(cursor - player_pos) <= 3) {
    for (auto i = 0; i < dots.size(); i++) {
      if (dotz::length(dots[i] - cursor) < 0.001f) {
        dots[i] = dots.pop_back();
        return;
      }
    }
  }
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
    handle(MOUSE_MOVE, &redraw);

    handle(MOUSE_DOWN, M_LEFT, &ctor);
    handle(MOUSE_DOWN, M_RIGHT, &dtor);
  }
} i;
