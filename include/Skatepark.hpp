#pragma once

#include "Section.hpp"
#include "Trick.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

class Skatepark {
public:
    std::vector<Section *> m_sections;
    std::vector<Trick *> m_tricks;
    std::vector<std::vector<int64_t>> m_tricks_sum;
    std::vector<std::vector<Trick *>> m_possible_tricks;
    std::vector<uint64_t> m_possible_tricks_time_trick;
    std::vector<std::vector<int64_t>> m_radical;
    std::vector<std::vector<uint16_t>> m_radical_used;

    Skatepark(std::vector<Section *> sections, std::vector<Trick *> tricks);

    bool contains(const std::vector<Trick *> &vecObj, const Trick *t);

    int64_t sum_penalized_tricks(uint16_t tricks, uint16_t use_tricks);
    
    std::pair<int64_t, std::vector<std::vector<Trick *>>> more_radical_crossing();

};
