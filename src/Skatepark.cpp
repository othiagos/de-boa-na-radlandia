#include "Skatepark.hpp"

Skatepark::Skatepark(std::vector<Section *> &sections, std::vector<Trick *> &tricks) : m_sections(sections), m_tricks(tricks) {}

uint64_t Skatepark::T_zero(int16_t n, uint8_t last_stage) {
    if (n < 0)
        return 0;

    std::set<uint16_t> use_tricks;

    if (last_stage == STAGE_ZERO || last_stage == STAGE_PLUS)
        return std::max(T_zero(n - 1, STAGE_ZERO), T_plus(n - 1, STAGE_ZERO, use_tricks));

    return std::max({T_zero(n - 1, STAGE_ZERO), T_plus(n - 1, STAGE_ZERO, use_tricks), T_star(n - 1, STAGE_ZERO, use_tricks)});
}

uint64_t Skatepark::T_plus(int16_t n, uint8_t last_stage, std::set<uint16_t> &use_tricks) {
    if (n < 0)
        return 0;

    uint64_t sum = 0;
    uint64_t count = 0;
    std::set<uint16_t> new_use_tricks;

    for (Trick *s : m_tricks) {
        if (s->m_baseline_score > 0 && m_sections.at(m_sections.size() - 1 - n)->m_crossing_time >= s->m_time_trick) {

            auto it = use_tricks.find(count);

            if (it != use_tricks.end()) 
                sum += s->m_baseline_score / 2;
            else 
                sum += s->m_baseline_score;

            new_use_tricks.insert(count);
            count++;
        }
    }


    sum *= (count * m_sections.at(m_sections.size() - 1 - n)->m_bonus_factor);

    if (last_stage == STAGE_ZERO || last_stage == STAGE_PLUS)
        return sum + std::max(T_zero(n - 1, STAGE_ZERO), T_plus(n - 1, STAGE_ZERO, new_use_tricks));

    return sum + std::max({T_zero(n - 1, STAGE_ZERO), T_plus(n - 1, STAGE_ZERO, new_use_tricks), T_star(n - 1, STAGE_ZERO, new_use_tricks)});
}

uint64_t Skatepark::T_star(int16_t n, uint8_t last_stage, std::set<uint16_t> &use_tricks) {

    if (n < 0)
        return 0;

    uint64_t sum = 0;
    uint64_t count = 0;
    std::set<uint16_t> new_use_tricks;

    for (Trick *s : m_tricks) {
        auto it = use_tricks.find(count);
        if (it != use_tricks.end())
            continue;

        if (s->m_baseline_score > 0 && m_sections.at(m_sections.size() - 1 - n)->m_crossing_time >= s->m_time_trick) {
            sum += s->m_baseline_score;

            new_use_tricks.insert(count);
            count++;
        }
    }

    sum *= (count * m_sections.at(m_sections.size() - 1 - n)->m_bonus_factor);

    if (last_stage == STAGE_ZERO || last_stage == STAGE_PLUS)
        return sum + std::max(T_zero(n - 1, STAGE_ZERO), T_plus(n - 1, STAGE_ZERO, new_use_tricks));

    return sum + std::max({T_zero(n - 1, STAGE_ZERO), T_plus(n - 1, STAGE_ZERO, new_use_tricks), T_star(n - 1, STAGE_ZERO, new_use_tricks)});
}

uint64_t Skatepark::findMaxTrickSequence() {

    std::set<uint16_t> use_tricks;
    return std::max({T_zero(m_sections.size() - 1, STAGE_ZERO), T_plus(m_sections.size() - 1, STAGE_ZERO, use_tricks), T_star(m_sections.size() - 1, STAGE_ZERO, use_tricks)});
}