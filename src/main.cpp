#include "Skatepark.hpp"

#include <cstdint>
#include <iostream>

int main(int argc, char const *argv[]) {

    uint16_t N, K;

    std::cin >> N;
    std::cin >> K;

    std::vector<Section *> sections;
    std::vector<Trick *> tricks;
    sections.reserve(N);
    tricks.reserve(K);

    for (uint16_t i = 0; i < N; i++) {
        uint16_t c;
        uint32_t l;

        std::cin >> c;
        std::cin >> l;

        sections.push_back(new Section(c, l));
    }

    for (uint16_t j = 0; j < K; j++) {
        int32_t p;
        uint64_t t;

        std::cin >> p >> t;

        tricks.push_back(new Trick(p, t, j));
    }

    Skatepark park(sections, tricks);

    std::pair<int64_t, std::vector<std::vector<Trick *>>> t = park.more_radical_crossing();
    std::cout << t.first << std::endl;

    for (auto it = t.second.begin(); it != t.second.end(); it++) {
        std::cout << it->size() << ' ';
        for (const Trick *t : *it)
            std::cout << (uint16_t)t->m_index + 1 << ' ';
        std::cout << '\n';
    }

    for (Section *s : sections)
        delete s;

    for (Trick *t : tricks)
        delete t;

    return 0;
}
