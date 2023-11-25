#pragma once

#include <cstdint>
#include <string>

class Trick {
public:
    int32_t m_baseline_score;
    uint32_t m_time_trick;
    uint16_t m_index;

    Trick(int32_t baseline_score, uint32_t time_trick, uint16_t index);

    Trick(const Trick &t);

    std::string to_string() const;
};
