#include "Trick.hpp"

Trick::Trick(int32_t baseline_score, uint32_t time_trick, uint16_t index)
    : m_baseline_score(baseline_score), m_time_trick(time_trick), m_index(index) {}

Trick::Trick(const Trick &t) {
    this->m_baseline_score = t.m_baseline_score;
    this->m_index = t.m_index;
    this->m_time_trick = t.m_time_trick;
}

std::string Trick::to_string() const {
    return std::to_string(m_index);
}

bool Trick::operator==(const Trick& o) {
    return this->m_index == o.m_index;
}
