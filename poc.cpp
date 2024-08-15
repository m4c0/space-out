#pragma leco app
#pragma leco add_resource "atlas.png"

import casein;
import dotz;
import hai;
import input;
import quack;
import silog;
import sitime;

static dotz::vec2 camera_pos {};
static dotz::vec2 player_pos {};
static sitime::stopwatch last_frame {};

static hai::varray<dotz::vec2> dots { 1024 };

static void blit(quack::instance *& is, quack::instance i) {
  i.size = { 1 };
  i.uv0 = i.uv0 / 16.0f;
  i.uv1 = i.uv1 / 16.0f;
  i.multiplier = { 1 };
  *is++ = i;
}

static void data(quack::instance *& is) {
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
}
static void load_data() { quack::donald::data(::data); }

static void process_input(float dt) {
  auto d = input::left_stick();
  if (dotz::length(d) > 0.001) player_pos = player_pos + d * dt * 4.0f;
}

static void process_camera(float dt) {
  auto delta = dotz::abs(camera_pos - player_pos);
  dt *= 0.8;

  if (delta.x >= 3) camera_pos.x = dotz::mix(camera_pos.x, player_pos.x, dt);
  if (delta.y >= 3) camera_pos.y = dotz::mix(camera_pos.y, player_pos.y, dt);

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

    input::setup_defaults();

    handle(REPAINT, &repaint);

    handle(MOUSE_DOWN, M_LEFT, &ctor);
    handle(MOUSE_DOWN, M_RIGHT, &dtor);
  }
} i;
