#pragma once

#include <cstdint>

struct Trick {
    int32_t m_baseline_score;
    uint32_t m_time_trick;

    Trick(int32_t baseline_score, uint32_t time_trick) :
       m_baseline_score(baseline_score), m_time_trick(time_trick) { }
};
