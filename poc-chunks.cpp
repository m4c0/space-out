#pragma leco app

import casein;
import dotz;
import hai;
import quack;

static hai::varray<dotz::vec2> dots { 1024 };

static unsigned data(quack::instance *is) {
  const auto b = is;

  *is++ = {
    .position = dotz::floor(quack::donald::mouse_pos()),
    .size = { 1 },
    .colour = { 1, 0, 0, 1 },
  };

  for (auto &d : dots) {
    *is++ = {
      .position = d + dotz::vec2 { 0.1f },
      .size = { 0.8f },
      .colour = { 0, 1, 0, 1 },
    };
  }

  return is - b;
}

static void redraw() { quack::donald::data(::data); }

static void ctor() {
  auto cursor = dotz::floor(quack::donald::mouse_pos());
  for (auto i = 0; i < dots.size(); i++) {
    if (dotz::length(dots[i] - cursor) < 0.001f) {
      return;
    }
  }

  dots.push_back(cursor);
}
static void dtor() {
  auto cursor = dotz::floor(quack::donald::mouse_pos());
  for (auto i = 0; i < dots.size(); i++) {
    if (dotz::length(dots[i] - cursor) < 0.001f) {
      dots[i] = dots.pop_back();
      return;
    }
  }
}

static struct init {
  init() {
    using namespace casein;
    using namespace quack::donald;

    max_quads(1024);
    clear_colour({ 0, 0, 0, 1 });

    push_constants({
      .grid_pos = { 0, 0 },
      .grid_size = { 32, 32 },
    });

    data(::data);

    handle(MOUSE_MOVE, &redraw);
    handle(MOUSE_DOWN, M_LEFT, &ctor);
    handle(MOUSE_DOWN, M_RIGHT, &dtor);
  }
} i;

