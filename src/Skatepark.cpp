#include "Skatepark.hpp"

Skatepark::Skatepark(std::vector<std::shared_ptr<Section>> sections, std::vector<std::shared_ptr<Trick>> tricks) {
    this->m_sections = sections;
    this->m_tricks = tricks;

    uint16_t max_combination = pow(2, m_tricks.size());
    for (uint16_t i = 1; i < max_combination; i++) {
        std::vector<std::shared_ptr<Trick>> sub_trick;

        for (uint16_t j = 0; j < m_tricks.size(); j++) {
            if (i >> j & 1)
                sub_trick.push_back(m_tricks[j]);
        }
        m_possible_tricks.push_back(sub_trick);
    }
}

void Skatepark::generated_matrix(const std::vector<std::shared_ptr<Trick>> &tricks, std::shared_ptr<MATRIX> &out) {
    out = std::make_shared<MATRIX>(100);

    for (uint16_t i = 0; i < 100; i++)
        (*out)[i] = std::make_shared<std::vector<std::pair<int64_t, std::vector<std::shared_ptr<Trick>>>>>(m_tricks.size());

    for (uint16_t i = 0; i < 100; i++) {
        for (uint16_t j = 0; j < m_tricks.size(); j++)
            (*(*out)[i])[j].first = INT64_MIN;
    }
}

void Skatepark::generated_penalized_tricks(const std::vector<std::shared_ptr<Trick>> &tricks, const std::vector<std::shared_ptr<Trick>> &use_tricks, std::vector<std::shared_ptr<Trick>> &penalized_tricks) {

    uint16_t i = 1;
    std::shared_ptr<Trick> current_use_trick = (use_tricks.size()) ? use_tricks[0] : nullptr;
    for (std::shared_ptr<Trick> sp : tricks) {
        std::shared_ptr<Trick> c(new Trick(*sp));
        if (current_use_trick != nullptr && sp->m_index == current_use_trick->m_index) {
            c->m_baseline_score = c->m_baseline_score / 2;

            if (i < use_tricks.size()) {
                current_use_trick = use_tricks[i];
                i++;
            }
        }
        penalized_tricks.push_back(c);
    }
}

std::string Skatepark::vector_to_string(const std::vector<std::shared_ptr<Trick>> &v) {
    std::string s = "";
    for (const std::shared_ptr<Trick> &sp : v)
        s += sp->to_string();
    return s;
}

std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> Skatepark::best_tricks(int16_t capacity, uint16_t index, std::shared_ptr<MATRIX> mat, std::vector<std::shared_ptr<Trick>> &tricks) {
    if (capacity <= 0 || (capacity >= 0 && index >= tricks.size()))
        return std::pair<int64_t, std::vector<std::shared_ptr<Trick>>>(0, {});
    
    if ((*(*mat)[capacity])[index].first != INT64_MIN)
        return (*(*mat)[capacity])[index];

    if (tricks[index]->m_time_trick > (uint16_t)capacity) {
        (*(*mat)[capacity])[index] = best_tricks(capacity, index + 1, mat, tricks);
        return (*(*mat)[capacity])[index];
    }

    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> use = best_tricks(capacity - tricks[index]->m_time_trick, index + 1, mat, tricks);
    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> not_use = best_tricks(capacity, index + 1, mat, tricks);
    use.first += tricks[index]->m_baseline_score;

    if (use.first * (use.second.size() + 1) >= not_use.first * not_use.second.size()) {
        (*(*mat)[capacity])[index].first = use.first;
        (*(*mat)[capacity])[index].second.push_back(tricks[index]);
        (*(*mat)[capacity])[index].second.insert(
            (*(*mat)[capacity])[index].second.end(), use.second.begin(), use.second.end());
    } else {
        (*(*mat)[capacity])[index].first = not_use.first;
        (*(*mat)[capacity])[index].second.insert(
            (*(*mat)[capacity])[index].second.end(), not_use.second.begin(), not_use.second.end());
    }

    return (*(*mat)[capacity])[index];
}

std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> Skatepark::more_radical_crossing(uint16_t n, std::vector<std::shared_ptr<Trick>> used_tricks) {
    if (n >= m_sections.size()) {
        return std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>>(0, {});
    }

    std::vector<std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>>> values;
    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> out;
    std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> ret = more_radical_crossing(n + 1, {});
    ret.second.push_back(out.second);
    values.push_back(ret);

    for (uint16_t i = 0; i < m_possible_tricks.size(); i++) {

        std::vector<std::shared_ptr<Trick>> tricks;
        generated_penalized_tricks(m_possible_tricks[i], used_tricks, tricks);

        std::string vstr = vector_to_string(tricks);

        std::shared_ptr<MATRIX> m;
        auto it = m_maps.find(vstr);
        if (it != m_maps.end()) {
            m = it->second;
        } else {
            generated_matrix(m_possible_tricks[i], m);
            m_maps.insert({vstr, m});
        }

        std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> out = best_tricks(m_sections[n]->m_crossing_time, 0, m, tricks);

        out.first *= out.second.size() * m_sections[n]->m_bonus_factor;

        std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> ret = more_radical_crossing(n + 1, out.second);
        ret.first += out.first;
        ret.second.push_back(out.second);

        values.push_back(ret);
    }

    return *std::max_element(values.begin(), values.end());
}
