#pragma once

#include "Section.hpp"
#include "Trick.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <exception>

class Skatepark {
public:
    std::vector<Section*> m_sections;
    std::vector<Trick *> m_tricks;
    std::vector<Trick *> m_tricks_penalized;
    std::vector<std::vector<Trick *>> m_possible_tricks;
    std::map<std::string, std::pair<int64_t, std::vector<std::vector<Trick *>>>> m_maps_radical;

    Skatepark(std::vector<Section*> sections, std::vector<Trick *> tricks);

    ~Skatepark();

    Trick * binary_search(const std::vector<Trick *> &tricks, uint8_t needle);

    void generated_penalized_tricks(const std::vector<Trick *> &tricks, const std::vector<Trick *> &use_tricks, std::vector<Trick *> &out);

    std::string radical_to_string(const std::vector<Trick *> &v, uint16_t n);

    std::pair<int64_t, std::vector<Trick *>> best_tricks(int32_t capacity, uint16_t index, std::vector<Trick *> &tricks);

    std::pair<int64_t, std::vector<std::vector<Trick *>>> more_radical_crossing(uint16_t n, std::vector<Trick *> used_tricks);
};
