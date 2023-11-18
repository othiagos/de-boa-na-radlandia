#pragma once

#include "Section.hpp"
#include "Trick.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#define MATRIX std::vector<std::shared_ptr<std::vector<std::pair<int64_t, std::vector<std::shared_ptr<Trick>>>>>>

class Skatepark {
public:
    std::vector<std::shared_ptr<Section>> m_sections;
    std::vector<std::shared_ptr<Trick>> m_tricks;
    std::vector<std::vector<std::shared_ptr<Trick>>> m_possible_tricks;
    std::map<std::string, std::shared_ptr<MATRIX>> m_maps;

    Skatepark(std::vector<std::shared_ptr<Section>> sections, std::vector<std::shared_ptr<Trick>> tricks);

    void generated_matrix(const std::vector<std::shared_ptr<Trick>> &tricks, std::shared_ptr<MATRIX> &out);

    void generated_penalized_tricks(const std::vector<std::shared_ptr<Trick>> &tricks, const std::vector<std::shared_ptr<Trick>> &use_tricks, std::vector<std::shared_ptr<Trick>> &out);

    std::string vector_to_string(const std::vector<std::shared_ptr<Trick>> &v);

    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> best_tricks(int16_t capacity, uint16_t index, std::shared_ptr<MATRIX> mat, std::vector<std::shared_ptr<Trick>> &tricks);

    std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> more_radical_crossing(uint16_t n, std::vector<std::shared_ptr<Trick>> used_tricks);
};
