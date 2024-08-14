#pragma leco app
#pragma leco add_resource "atlas.png"

import casein;
import dotz;
import hai;
import quack;
import silog;
import sitime;

enum buttons { B_UP = 0, B_DOWN, B_LEFT, B_RIGHT, B_COUNT };

static dotz::vec2 camera_pos {};
static dotz::vec2 player_pos {};
static bool button_state[B_COUNT] {};
static sitime::stopwatch last_frame {};

static hai::varray<dotz::vec2> dots { 1024 };

static float axis(buttons n, buttons p) {
  auto ns = button_state[n];
  auto ps = button_state[p];

  if (ns && !ps) return -1;
  if (ps && !ns) return +1;
  return 0;
}

static void blit(quack::instance *&is, quack::instance i) {
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
static void load_data() { quack::donald::data(::data); }

static void process_input(float dt) {
  dotz::vec2 d { axis(B_LEFT, B_RIGHT), axis(B_UP, B_DOWN) };
  if (dotz::length(d) > 0.001)
    player_pos = player_pos + d * dt * 4.0f;

}
static void process_camera(float dt) {
  auto delta = dotz::abs(camera_pos - player_pos);

  if (delta.x >= 5) camera_pos.x = dotz::mix(camera_pos.x, player_pos.x, dt);
  if (delta.y >= 5) camera_pos.y = dotz::mix(camera_pos.y, player_pos.y, dt);

  quack::donald::push_constants({
    .grid_pos = camera_pos,
    .grid_size = { 16, 16 },
  });
}

static void repaint() {
  float dt = last_frame.millis() / 1000.0f;
  last_frame = {};

  process_input(dt);
  process_camera(dt);

  load_data();
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

    clear_colour({ 0, 0, 0, 1 });
    push_constants({
        .grid_pos = { 0, 0 },
        .grid_size = { 16, 16 },
    });

    atlas("atlas.png");
    load_data();

    handle_btn(K_W, B_UP);
    handle_btn(K_S, B_DOWN);
    handle_btn(K_A, B_LEFT);
    handle_btn(K_D, B_RIGHT);

    handle(REPAINT, &repaint);

    handle(MOUSE_DOWN, M_LEFT, &ctor);
    handle(MOUSE_DOWN, M_RIGHT, &dtor);
  }
} i;
