#include "Skatepark.hpp"

Skatepark::Skatepark(std::vector<Section *> sections, std::vector<Trick *> tricks) {
    this->m_sections = sections;
    this->m_tricks = tricks;

    uint16_t max_combination = pow(2, m_tricks.size());

    m_possible_tricks.reserve(max_combination);
    m_possible_tricks_time_trick.reserve(max_combination);

    m_possible_tricks.push_back({});
    m_possible_tricks_time_trick.push_back(0);
    for (uint16_t i = 1; i < max_combination; i++) {
        std::vector<Trick *> sub_trick;
        uint64_t sum = 0;
        
        for (uint16_t j = 0; j < m_tricks.size(); j++) {
            if (i >> j & 1) {
                sub_trick.push_back(m_tricks[j]);
                sum += m_tricks[j]->m_time_trick;
            }
        }

        m_possible_tricks.push_back(sub_trick);
        m_possible_tricks_time_trick.push_back(sum);
    }

    m_radical.reserve(max_combination);
    for (uint16_t i = 0; i < max_combination; i++) {
        m_radical.push_back(std::vector<int64_t>(sections.size()));

        for (uint16_t j = 0; j < sections.size(); j++) {
            m_radical[i][j] = INT64_MIN;
        }
    }

    m_radical_used.reserve(max_combination);
    for (uint16_t i = 0; i < max_combination; i++) {
        m_radical_used.push_back(std::vector<uint16_t>(sections.size()));
    }

    m_tricks_sum.reserve(max_combination);
    for (uint16_t i = 0; i < max_combination; i++) {
        m_tricks_sum.push_back(std::vector<int64_t>(max_combination));

        for (uint16_t j = 0; j < max_combination; j++) {
            m_tricks_sum[i][j] = INT64_MIN;
        }
    }
}

int64_t Skatepark::sum_penalized_tricks(uint16_t used_m, uint16_t m) {
    int64_t sum = 0;
    
    uint16_t trick_size = m_tricks.size();
    for (uint16_t i = 0; i < trick_size; i++) {
        if ((used_m >> i & 1) & (m >> i & 1)) {
            sum += m_tricks[i]->m_baseline_score / 2;
        } else if (m >> i & 1) {
            sum += m_tricks[i]->m_baseline_score;
        }
    }
    return sum;
}

std::pair<int64_t, std::vector<std::vector<Trick *>>> Skatepark::more_radical_crossing() {

    int64_t max_section = 0;

    for (uint16_t used_m = 0; used_m < m_possible_tricks.size(); used_m++) {
        max_section = 0;
        uint16_t max_m = 0;

        for (uint16_t m = 0; m < m_possible_tricks.size(); m++) {

            if (m_possible_tricks_time_trick[m] > m_sections[m_sections.size() - 1]->m_crossing_time)
                continue;

            int64_t sum = sum_penalized_tricks(used_m, m);
            m_tricks_sum[used_m][m] = sum;

            sum *= m_possible_tricks[m].size() * m_sections[m_sections.size() - 1]->m_bonus_factor;

            if (max_section < sum) {
                max_section = sum;
                max_m = m;
            }
        }
        m_radical[used_m][m_sections.size() - 1] = max_section;
        m_radical_used[used_m][m_sections.size() - 1] = max_m;
    }

    for (int16_t i = m_sections.size() - 2; i >= 0; i--) {
        for (uint16_t used_m = 0; used_m < m_possible_tricks.size(); used_m++) {
            max_section = 0;
            uint16_t max_m = 0;
            for (uint16_t m = 0; m < m_possible_tricks.size(); m++) {

                if (m_possible_tricks_time_trick[m] > m_sections[i]->m_crossing_time)
                    continue;

                int64_t sum;
                if (m_tricks_sum[used_m][m] == INT64_MIN) {
                    sum = sum_penalized_tricks(used_m, m);
                    m_tricks_sum[used_m][m] = sum;
                } else {
                    sum = m_tricks_sum[used_m][m];
                }

                sum *= m_possible_tricks[m].size() * m_sections[i]->m_bonus_factor;
                sum += m_radical[m][i + 1];

                if (max_section < sum) {
                    max_section = sum;
                    max_m = m;
                }
            }
            m_radical[used_m][i] = max_section;
            m_radical_used[used_m][i] = max_m;
        }
    }

    std::vector<std::vector<Trick *>> v;
    uint16_t j = 0;
    for (uint16_t i = 0; i < m_sections.size(); i++) {
        j = m_radical_used[j][i];
        v.push_back(m_possible_tricks[j]);
    }

    return {m_radical[0][0], v};
}
