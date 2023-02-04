#pragma once

#include <core.h>

enum tbstd_world {
    TBCORE = 1 << 0,
    TBMATH = 1 << 1
};

void tbstd_init(struct dict *dict, enum tbstd_world world);
