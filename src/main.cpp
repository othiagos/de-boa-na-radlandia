#include "Skatepark.hpp"

#include <cstdint>
#include <iostream>

int main(int argc, char const *argv[]) {

    uint16_t N, K;

    std::cin >> N;
    std::cin >> K;

    std::vector<std::shared_ptr<Section>> sections;
    std::vector<std::shared_ptr<Trick>> tricks;
    sections.reserve(N);
    tricks.reserve(K);

    for (uint8_t i = 0; i < N; i++) {
        uint16_t c;
        uint32_t l;

        std::cin >> c;
        std::cin >> l;

        sections.push_back(std::make_shared<Section>(c, l));
    }

    for (uint8_t j = 0; j < K; j++) {
        int32_t p;
        uint64_t t;

        std::cin >> p >> t;

        if (p > 0) tricks.push_back(std::make_shared<Trick>(p, t, j));
    }

    Skatepark park(sections, tricks);

    std::pair<int64_t, std::vector<std::vector<std::shared_ptr<Trick>>>> t = park.more_radical_crossing(0, {});
    std::cout << t.first << std::endl;

    for (auto it = t.second.rbegin(); it != t.second.rend(); it++) {
        std::cout << it->size() << ' ';
        for (const std::shared_ptr<Trick> &sp : *it) {
            std::cout << (uint16_t)sp->m_index + 1 << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
