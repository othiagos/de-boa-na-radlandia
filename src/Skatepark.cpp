#include "Skatepark.hpp"

Skatepark::Skatepark(std::vector<Section *> sections, std::vector<Trick *> tricks) {
    this->m_sections = sections;
    this->m_tricks = tricks;

    m_matrix_tricks = std::vector<std::vector<std::pair<int64_t, std::vector<Trick *>>>>(100);

    for (uint16_t i = 0; i < 100; i++)
        m_matrix_tricks[i] = std::vector<std::pair<int64_t, std::vector<Trick *>>>(m_tricks.size());

    for (uint16_t i = 0; i < 100; i++) {
        for (uint16_t j = 0; j < m_tricks.size(); j++) 
            m_matrix_tricks[i][j].first = INT64_MIN;
    }
}

std::pair<int64_t, std::vector<Trick *>> Skatepark::best_tricks(int16_t capacity, uint16_t index) {
    if (capacity <= 0 || (capacity >= 0 && index >= m_tricks.size()))
        return std::pair<int64_t, std::vector<Trick *>>(0, {});

    if (m_matrix_tricks[capacity][index].first != INT64_MIN)
        return m_matrix_tricks[capacity][index];

    if (m_tricks[index]->m_time_trick > (uint16_t)capacity) {
        m_matrix_tricks[capacity][index] = best_tricks(capacity, index + 1);
        return m_matrix_tricks[capacity][index];
    }

    std::pair<int64_t, std::vector<Trick *>> use = best_tricks(capacity - m_tricks[index]->m_time_trick, index + 1);
    std::pair<int64_t, std::vector<Trick *>> not_use = best_tricks(capacity, index + 1);
    use.first += m_tricks[index]->m_baseline_score;

    if (use >= not_use) {
        m_matrix_tricks[capacity][index].first = use.first;
        m_matrix_tricks[capacity][index].second.push_back(m_tricks[index]);
        m_matrix_tricks[capacity][index].second.insert(
            m_matrix_tricks[capacity][index].second.end(), use.second.begin(), use.second.end());
    } else {
        m_matrix_tricks[capacity][index].first = not_use.first;
        m_matrix_tricks[capacity][index].second.insert(
            m_matrix_tricks[capacity][index].second.end(), not_use.second.begin(), not_use.second.end());
    }

    // m_matrix_tricks[capacity][index] = std::max(use, not_use);
    return m_matrix_tricks[capacity][index];
}

// uint64_t Skatepark::trick_sequence(int16_t n, std::vector<const Trick *> used_tricks) {
//     if (n < 0)
//         return 0;

//     std::set<uint16_t> new_use_tricks;

//     uint16_t i = calc_index(used_tricks);

//     if()

//     int64_t sum;
//     for (const Trick *s : m_tricks) {

//         if (m_sections.at(m_sections.size() - 1 - n)->m_crossing_time >= s->m_time_trick)
//             if ()
//             sum += s->m_baseline_score;
//     }

//     sum *= used_tricks.size() * m_sections.at(m_sections.size() - 1 - n)->m_bonus_factor;

//     return sum + std::max({trick_sequence(n - 2, {}),
//                            trick_sequence(n - 1, m_tricks),
//                            trick_sequence(n - 1, m_tricks)});
// }

uint64_t Skatepark::more_radical_crossing() {
    std::pair<int64_t, std::vector<Trick *>> a = best_tricks(m_sections[0]->m_crossing_time, 0);
    return a.first;
}
