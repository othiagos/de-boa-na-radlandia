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
#include <exception>

class Skatepark {
public:
    std::vector<std::shared_ptr<Section>> m_sections;
    std::vector<std::shared_ptr<Trick>> m_tricks;
    std::vector<std::shared_ptr<Trick>> m_tricks_penalized;
    std::vector<std::vector<std::shared_ptr<Trick>>> m_possible_tricks;
    std::map<std::string, std::pair<int64_t, std::vector<std::shared_ptr<Trick>>>> m_maps_mult_matrix;
    std::map<std::string, std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>>> m_maps_radical;

    Skatepark(std::vector<std::shared_ptr<Section>> sections, std::vector<std::shared_ptr<Trick>> tricks);

    std::shared_ptr<Trick> binary_search(const std::vector<std::shared_ptr<Trick>> &tricks, uint8_t needle);

    void generated_penalized_tricks(const std::vector<std::shared_ptr<Trick>> &tricks, const std::vector<std::shared_ptr<Trick>> &use_tricks, std::vector<std::shared_ptr<Trick>> &out);

    std::string mult_matrix_to_string(const std::vector<std::shared_ptr<Trick>> &v, int16_t capacity, uint16_t index);

    std::string radical_to_string(const std::vector<std::shared_ptr<Trick>> &v, uint16_t n);

    std::pair<int64_t, std::vector<std::shared_ptr<Trick>>> best_tricks(int32_t capacity, uint16_t index, std::vector<std::shared_ptr<Trick>> &tricks);

    std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> more_radical_crossing(uint16_t n, std::vector<std::shared_ptr<Trick>> used_tricks);
};
