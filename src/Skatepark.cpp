#include "Skatepark.hpp"

Skatepark::Skatepark(std::vector<std::shared_ptr<Section>> sections, std::vector<std::shared_ptr<Trick>> tricks) {
    this->m_sections = sections;
    this->m_tricks = tricks;

    for (std::shared_ptr<Trick> sp : m_tricks) {
        std::shared_ptr<Trick> c(new Trick(*sp));
        c->m_baseline_score = c->m_baseline_score / 2;
        m_tricks_penalized.push_back(c);
    }

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

std::shared_ptr<Trick> Skatepark::binary_search(const std::vector<std::shared_ptr<Trick>> &tricks, uint8_t needle) {
    uint16_t left = 0, right = tricks.size() - 1;

    while (left <= right) {
        uint16_t mid = left + (right - left) / 2;

        if (tricks[mid]->m_index == needle) {
            return tricks[mid];
        }
        else if (tricks[mid]->m_index < needle) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    throw std::invalid_argument("index " + std::to_string(needle) + " not found");
}

void Skatepark::generated_penalized_tricks(const std::vector<std::shared_ptr<Trick>> &tricks, const std::vector<std::shared_ptr<Trick>> &use_tricks, std::vector<std::shared_ptr<Trick>> &penalized_tricks) {

    uint16_t i = 1;
    std::shared_ptr<Trick> current_use_trick = (use_tricks.size()) ? use_tricks[0] : nullptr;
    for (std::shared_ptr<Trick> sp : tricks) {
        if (current_use_trick != nullptr && sp->m_index == current_use_trick->m_index) {
            penalized_tricks.push_back(binary_search(m_tricks_penalized, sp->m_index));

            if (i < use_tricks.size()) {
                current_use_trick = use_tricks[i];
                i++;
            }
        } else {
            penalized_tricks.push_back(sp);
        }
    }
}

std::string Skatepark::mult_matrix_to_string(const std::vector<std::shared_ptr<Trick>> &v, int16_t capacity, uint16_t index) {
    std::string s = "";
    for (const std::shared_ptr<Trick> &sp : v)
        s += sp->to_string();

    s += std::to_string(capacity);
    s += std::to_string(index);

    return s;
}

std::string Skatepark::radical_to_string(const std::vector<std::shared_ptr<Trick>> &v, uint16_t n) {
    std::string s = "";
    for (const std::shared_ptr<Trick> &sp : v)
        s += sp->to_string();
    s += std::to_string(n);
    return s;
}

std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> Skatepark::best_tricks(int32_t capacity, uint16_t index, std::vector<std::shared_ptr<Trick>> &tricks) {
    if (capacity <= 0 || (capacity >= 0 && index >= tricks.size()))
        return std::pair<int64_t, std::vector<std::shared_ptr<Trick>>>(0, {});

    std::string mmstr = mult_matrix_to_string(tricks, capacity, index);
    auto it = m_maps_mult_matrix.find(mmstr);

    if (it != m_maps_mult_matrix.end())
        return it->second;

    if (tricks[index]->m_time_trick > (uint32_t)capacity) {
        auto ret = best_tricks(capacity, index + 1, tricks);
        m_maps_mult_matrix.insert({mmstr, ret});
        return ret;
    }

    auto use = best_tricks(capacity - tricks[index]->m_time_trick, index + 1, tricks);
    auto not_use = best_tricks(capacity, index + 1, tricks);
    use.first += tricks[index]->m_baseline_score;

    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> ret;
    if (use.first * (use.second.size() + 1) >= not_use.first * not_use.second.size()) {
        ret.first = use.first;
        ret.second.push_back(tricks[index]);
        ret.second.insert(ret.second.end(), use.second.begin(), use.second.end());

    } else {
        ret.first = not_use.first;
        ret.second.insert(ret.second.end(), not_use.second.begin(), not_use.second.end());
    }

    m_maps_mult_matrix.insert({mmstr, ret});
    return ret;
}

std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> Skatepark::more_radical_crossing(uint16_t n, std::vector<std::shared_ptr<Trick>> used_tricks) {
    if (n >= m_sections.size()) {
        return std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>>(0, {});
    }

    std::string rstr = radical_to_string(used_tricks, n);
    auto it = m_maps_radical.find(rstr);
    if (it != m_maps_radical.end()) {
        return it->second;
    }

    std::vector<std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>>> values;
    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> out_1;
    std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> ret_1 = more_radical_crossing(n + 1, {});
    ret_1.second.push_back(out_1.second);
    values.push_back(ret_1);

    for (uint16_t i = 0; i < m_possible_tricks.size(); i++) {

        std::vector<std::shared_ptr<Trick>> tricks;
        generated_penalized_tricks(m_possible_tricks[i], used_tricks, tricks);

        std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> out_2 = best_tricks(m_sections[n]->m_crossing_time, 0, tricks);
        out_2.first *= out_2.second.size() * m_sections[n]->m_bonus_factor;

        std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> ret_2 = more_radical_crossing(n + 1, out_2.second);
        ret_2.first += out_2.first;
        ret_2.second.push_back(out_2.second);

        values.push_back(ret_2);
    }

    auto max_values = *std::max_element(values.begin(), values.end());
    m_maps_radical.insert({rstr, max_values});

    return max_values;
}
