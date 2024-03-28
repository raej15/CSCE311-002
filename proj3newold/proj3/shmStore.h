// Copyright 2024 Rae Jones
#ifndef PROJ3_SHMSTORE_H_
#define PROJ3_SHMSTORE_H_

#include <cstddef> //size_t

namespace shared_mem_struct {
    const std::size_t kRows = 1 << 2;
    const std::size_t kCols = 1 << 22;
    struct Store {
        char buf[kRows][kCols];
        std::size_t lens[kRows];
    };
} // namespace shared_mem_struct

#endif  // PROJ3_SHMSTORE_H_