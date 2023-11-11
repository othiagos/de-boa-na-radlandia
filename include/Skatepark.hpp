#pragma once

#include "Section.hpp"
#include "Trick.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>
#include <set>

#define STAGE_ZERO 0
#define STAGE_PLUS 1
#define STAGE_STAR 2

class Skatepark {
public:
    std::vector<Section *> &m_sections;
    std::vector<Trick *> &m_tricks;

    Skatepark(std::vector<Section *> &sections, std::vector<Trick *> &tricks);

    uint64_t T_zero(int16_t n, uint8_t last_stage);

    uint64_t T_plus(int16_t n, uint8_t last_stage, std::set<uint16_t> &use_tricks);

    uint64_t T_star(int16_t n, uint8_t last_stage, std::set<uint16_t> &use_tricks);

    uint64_t findMaxTrickSequence();
};
