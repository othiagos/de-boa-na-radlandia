#pragma once

#include "Section.hpp"
#include "Trick.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

#define MATRIX std::vector<std::vector<std::pair<int64_t, std::vector<Trick *>>>>

class Skatepark {
public:
    std::vector<Section *> m_sections;
    std::vector<Trick *> m_tricks;
    MATRIX m_matrix_tricks;
    std::vector<std::vector<Trick *>> m_possible_tricks;

    Skatepark(std::vector<Section *> sections, std::vector<Trick *> tricks);

    void generated_matrix(const std::vector<Trick *> &tricks ,MATRIX &out);

    void generated_penalized_tricks(const std::vector<Trick *> &tricks, const std::vector<Trick *> &use_tricks, std::vector<Trick *> &out);

    void otimize_reference(std::vector<Trick *> &tricks, std::vector<Trick *> &tricks_out);

    std::pair<int64_t, std::vector<Trick *>> best_tricks(int16_t capacity, uint16_t index, MATRIX &mat, std::vector<Trick *> &tricks);

    std::pair<int64_t, std::vector<std::vector<Trick *>>> more_radical_crossing(uint16_t n, std::vector<Trick *> used_tricks);
};
