#pragma once

#include <cstdint>

class Section {
public:
    uint16_t m_bonus_factor;
    uint32_t m_crossing_time;

    Section(uint16_t bonus_factor, uint32_t crossing_time);
};
