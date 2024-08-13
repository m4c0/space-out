#pragma leco app

import casein;
import dotz;
import quack;

static unsigned data(quack::instance *is) {
  const auto b = is;

  return is - b;
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
  }
} i;

