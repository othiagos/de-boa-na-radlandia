#pragma once

#include <cstdint>

class Trick {
public:
    int32_t m_baseline_score;
    uint32_t m_time_trick;
    uint8_t m_index;

    Trick(int32_t baseline_score, uint32_t time_trick, uint8_t index);

    Trick(const Trick &t);
};
