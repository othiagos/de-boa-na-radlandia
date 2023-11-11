#pragma once

#include <cstdint>

struct Section {
    uint16_t m_bonus_factor;
    uint32_t m_crossing_time;

    Section(uint16_t bonus_factor, uint32_t crossing_time) : 
        m_bonus_factor(bonus_factor), m_crossing_time(crossing_time) { }
};
