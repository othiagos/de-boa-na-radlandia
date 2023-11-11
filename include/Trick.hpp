#pragma once

#include <cstdint>

class Trick {
public:
    int32_t m_baseline_score;
    uint32_t m_time_trick;

    Trick(int32_t baseline_score, uint32_t time_trick);
};
