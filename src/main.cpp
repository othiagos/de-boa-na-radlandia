#include "Section.hpp"
#include "Trick.hpp"

#include <iostream>
#include <vector>

int main(int argc, char const *argv[]) {
    
    uint16_t N, K;

    std::cin >> N;
    std::cin >> K;
    
    std::vector<Section*> sections;
    std::vector<Trick*> tricks;
    sections.reserve(N);
    tricks.reserve(K);


    for (uint8_t i = 0; i < N; i++) {
        uint16_t c;
        uint32_t l;

        std::cin >> c;
        std::cin >> l;

        sections.push_back(new Section(c, l));
    }

    for (uint8_t j = 0; j < K; j++) {
        int32_t p;
        uint64_t t;

        std::cin >> p >> t;

        tricks.push_back(new Trick(p, t));
    }

    
    for (Section *s : sections) {
        std::cout << s->m_bonus_factor << ' ' << s->m_crossing_time << '\n';
    }

    for (Trick *t : tricks) {
        std::cout << t->m_baseline_score << ' ' << t->m_time_trick << '\n';
    }
    
    for (Section *s : sections) {
        delete s;
    }

    for (Trick *t : tricks) {
        delete t;
    }

    return 0;
}
