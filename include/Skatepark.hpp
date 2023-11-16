#pragma once

#include "Section.hpp"
#include "Trick.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

class Skatepark {
public:
    std::vector<Section *> m_sections;
    std::vector<Trick *> m_tricks;
    std::vector<std::vector<std::pair<int64_t, std::vector<Trick *>>>> m_matrix_tricks;

    Skatepark(std::vector<Section *> sections, std::vector<Trick *> tricks);

    std::pair<int64_t, std::vector<Trick *>> best_tricks(int16_t capacity, uint16_t index);

    // uint64_t tricks_sequence(int16_t n, std::vector<const Trick *> used_tricks);

    uint64_t more_radical_crossing();
};
