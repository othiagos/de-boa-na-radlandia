#include "Skatepark.hpp"

#include <iostream>
Skatepark::Skatepark(std::vector<Section*> sections, std::vector<Trick*> tricks) {
    this->m_sections = sections;
    this->m_tricks = tricks;

    for (Trick* t : m_tricks) {
        Trick* c(new Trick(*t));
        c->m_baseline_score = c->m_baseline_score / 2;
        m_tricks_penalized.push_back(c);
    }

    uint16_t max_combination = pow(2, m_tricks.size());
    for (uint16_t i = 1; i < max_combination; i++) {
        std::vector<Trick*> sub_trick;
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
}

Skatepark::~Skatepark() {
    for (Trick* t : m_tricks_penalized)
        delete t;
}

Trick* Skatepark::binary_search(const std::vector<Trick*> &tricks, uint8_t needle) {
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

uint64_t Skatepark::generated_penalized_tricks(const std::vector<Trick*> &tricks, const std::vector<Trick*> &use_tricks, std::vector<Trick*> &penalized_tricks) {

    uint16_t i = 1;
    uint64_t sum = 0;
    Trick* current_use_trick = (use_tricks.size()) ? use_tricks[0] : nullptr;
    for (Trick* t : tricks) {
        if (current_use_trick != nullptr && t->m_index == current_use_trick->m_index) {
            Trick *c = binary_search(m_tricks_penalized, t->m_index);
            penalized_tricks.push_back(c);
            sum += c->m_baseline_score;

            if (i < use_tricks.size()) {
                current_use_trick = use_tricks[i];
                i++;
            }
        } else {
            penalized_tricks.push_back(t);
            sum += t->m_baseline_score;
        }
    }

    return sum;
}

std::string Skatepark::radical_to_string(const std::vector<Trick*> &v, uint16_t n) {
    std::string s = "";
    for (const Trick *t : v)
        s += t->to_string();
    s += std::to_string(n);
    return s;
}

std::pair<int64_t, std::vector<std::vector<Trick*>>> Skatepark::more_radical_crossing(uint16_t n, std::vector<Trick*> used_tricks) {
    if (n >= m_sections.size()) {
        return std::pair<int64_t, std::vector<std::vector<Trick*>>>(0, {});
    }

    std::string rstr = radical_to_string(used_tricks, n);
    auto it = m_maps_radical.find(rstr);
    if (it != m_maps_radical.end()) {
        return it->second;
    }

    std::pair<int64_t, std::vector<std::vector<Trick *>>> max = more_radical_crossing(n + 1, {});
    max.second.push_back({});

    for (uint16_t i = 0; i < m_possible_tricks.size(); i++) {

        if (m_possible_tricks_time_trick[i] > m_sections[n]->m_crossing_time) continue;

        std::vector<Trick *> tricks;
        uint64_t sum = generated_penalized_tricks(m_possible_tricks[i], used_tricks, tricks);
        std::pair<int64_t, std::vector<Trick *>> curr;
        curr.first = sum;
        curr.second = tricks;

        curr.first *= curr.second.size() * m_sections[n]->m_bonus_factor;

        std::pair<int64_t, std::vector<std::vector<Trick *>>> ret = more_radical_crossing(n + 1, curr.second);
        ret.first += curr.first;
        ret.second.push_back(curr.second);

        if (max.first < ret.first)
            max = ret;
    }

    m_maps_radical.insert({rstr, max});
    return max;
}
