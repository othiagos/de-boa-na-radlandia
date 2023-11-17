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

    uint16_t max_combination = pow(2, m_tricks.size());
    for (uint16_t i = 1; i < max_combination; i++) {
        std::vector<Trick *> sub_trick;

        for (uint16_t j = 0; j < m_tricks.size(); j++) {
            if (i >> j & 1)
                sub_trick.push_back(m_tricks[j]);
        }
        m_possible_tricks.push_back(sub_trick);
    }
}

void Skatepark::generated_matrix(const std::vector<Trick *> &tricks, MATRIX &out) {
    out = MATRIX(100);

    for (uint16_t i = 0; i < 100; i++)
        out[i] = std::vector<std::pair<int64_t, std::vector<Trick *>>>(m_tricks.size());

    for (uint16_t i = 0; i < 100; i++) {
        for (uint16_t j = 0; j < m_tricks.size(); j++)
            out[i][j].first = INT64_MIN;
    }
}

void Skatepark::generated_penalized_tricks(const std::vector<Trick *> &tricks, const std::vector<Trick *> &use_tricks, std::vector<Trick *> &penalized_tricks) {

    uint16_t i = 1;
    Trick *current_use_trick = (use_tricks.size()) ? use_tricks[0] : nullptr;
    for (Trick *t : tricks) {
        Trick *c = new Trick(*t);
        if (current_use_trick != nullptr && t->m_index == current_use_trick->m_index) {
            c->m_baseline_score = c->m_baseline_score / 2;

            if (i < use_tricks.size()) {
                current_use_trick = use_tricks[i];
                i++;
            }
        }
        penalized_tricks.push_back(c);
    }
}

void Skatepark::otimize_reference(std::vector<Trick *> &tricks, std::vector<Trick *> &tricks_out) {

    std::vector<Trick*> v;

    for (Trick *t : tricks_out) {
        v.push_back(m_tricks[t->m_index]);
    }
    tricks_out = v;

    for (Trick* t : tricks)
        delete t;
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

    if (use.first * (use.second.size() + 1) >= not_use.first * not_use.second.size()) {
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

std::pair<int64_t, std::vector<std::vector<Trick *>>> Skatepark::more_radical_crossing(uint16_t n, std::vector<Trick *> used_tricks) {
    if (n >= m_sections.size()) {
        return std::pair<int64_t, std::vector<std::vector<Trick *>>>(0, {});
    }

    std::vector<std::pair<int64_t, std::vector<std::vector<Trick *>>>> values;
    std::pair<int64_t, std::vector<Trick *>> out;
    std::pair<int64_t, std::vector<std::vector<Trick *>>> ret = more_radical_crossing(n + 1, {});
    ret.second.push_back(out.second);
    values.push_back(ret);

    for (uint16_t i = 0; i < m_possible_tricks.size(); i++) {
        MATRIX m;
        generated_matrix(m_possible_tricks[i], m);

        std::vector<Trick *> tricks;
        generated_penalized_tricks(m_possible_tricks[i], used_tricks, tricks);

        std::pair<int64_t, std::vector<Trick *>> out = best_tricks(m_sections[n]->m_crossing_time, 0, m, tricks);
        otimize_reference(tricks, out.second);

        out.first *= out.second.size() * m_sections[n]->m_bonus_factor;

        std::pair<int64_t, std::vector<std::vector<Trick *>>> ret = more_radical_crossing(n + 1, out.second);
        ret.first += out.first;
        ret.second.push_back(out.second);

        values.push_back(ret);
    }

    return *std::max_element(values.begin(), values.end());
}
