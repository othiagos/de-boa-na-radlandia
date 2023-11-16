#include "Skatepark.hpp"

Skatepark::Skatepark(std::vector<Section *> sections, std::vector<Trick *> tricks) {
    this->m_sections = sections;
    this->m_tricks = tricks;

    m_matrix_tricks = MATRIX(100);

    for (uint16_t i = 0; i < 100; i++)
        m_matrix_tricks[i] = std::vector<std::pair<int64_t, std::vector<Trick *>>>(m_tricks.size());

    for (uint16_t i = 0; i < 100; i++) {
        for (uint16_t j = 0; j < m_tricks.size(); j++)
            m_matrix_tricks[i][j].first = INT64_MIN;
    }
}

std::vector<Trick *> Skatepark::generated_penalized_tricks(std::vector<Trick *> use_tricks) {
    std::vector<Trick *> penalized_tricks;

    for (const Trick *t : m_tricks) {
        penalized_tricks.push_back(new Trick(*t));
    }

    for (const Trick *t : use_tricks) {
        penalized_tricks[t->m_index]->m_baseline_score = penalized_tricks[t->m_index]->m_baseline_score / 2;
    }

    return penalized_tricks;
}

std::vector<Trick *> Skatepark::generated_tricks_without_use_tricks(std::vector<Trick *> use_tricks) {
    std::vector<Trick *> not_use_tricks;

    uint16_t i = 1;
    Trick *current_use_trick = use_tricks[0];
    for (const Trick *t : m_tricks) {
        if (t->m_index < current_use_trick->m_index || i >= use_tricks.size()) {
            not_use_tricks.push_back(new Trick(*t));
        } else {
            if (i < use_tricks.size()) {
                current_use_trick = use_tricks[i];
                i++;
            }
        }
    }
    return not_use_tricks;
}

std::pair<int64_t, std::vector<Trick *>> Skatepark::best_tricks(int16_t capacity, uint16_t index, MATRIX &mat, std::vector<Trick *> &tricks) {
    if (capacity <= 0 || (capacity >= 0 && index >= tricks.size()))
        return std::pair<int64_t, std::vector<Trick *>>(0, {});

    if (mat[capacity][index].first != INT64_MIN)
        return mat[capacity][index];

    if (tricks[index]->m_time_trick > (uint16_t)capacity) {
        mat[capacity][index] = best_tricks(capacity, index + 1, mat, tricks);
        return mat[capacity][index];
    }

    std::pair<int64_t, std::vector<Trick *>> use = best_tricks(capacity - tricks[index]->m_time_trick, index + 1, mat, tricks);
    std::pair<int64_t, std::vector<Trick *>> not_use = best_tricks(capacity, index + 1, mat, tricks);
    use.first += tricks[index]->m_baseline_score;

    if (use.first >= not_use.first) {
        mat[capacity][index].first = use.first;
        mat[capacity][index].second.push_back(tricks[index]);
        mat[capacity][index].second.insert(
            mat[capacity][index].second.end(), use.second.begin(), use.second.end());
    } else {
        mat[capacity][index].first = not_use.first;
        mat[capacity][index].second.insert(
            mat[capacity][index].second.end(), not_use.second.begin(), not_use.second.end());
    }

    return mat[capacity][index];
}

int64_t Skatepark::tricks_sequence(uint16_t n, std::vector<const Trick *> used_tricks) {
    if (n >= m_sections.size()) {
        return 0;
    }

    return 0;
}

uint64_t Skatepark::more_radical_crossing() {
    std::pair<int64_t, std::vector<Trick *>> a = best_tricks(m_sections[2]->m_crossing_time, 0, m_matrix_tricks, m_tricks);

    for (Trick *t : a.second) {
        std::cout << '(' << (int16_t)t->m_index + 1 << ',' << t->m_baseline_score << ',' << t->m_time_trick << ")\n";
    }
    
    return a.first * a.second.size() * m_sections[2]->m_bonus_factor;
}
