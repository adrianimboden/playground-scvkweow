#include <cstdint>
#include <iostream>

class ProductProducer {
public:
    explicit ProductProducer(bool quality_check_enabled, int32_t quality_reference, int32_t maximal_difference)
        : quality_check_enabled_{quality_check_enabled}
        , quality_reference_{quality_reference}
        , maximal_difference_{maximal_difference} {
    }

    bool produce_one(int32_t quality) {
        if (quality_check_enabled_) {
            std::cout << "checking quality..." << std::endl;
            const auto difference = quality - quality_reference_;
            return difference <= maximal_difference_;
        }
        std::cout << "not checking quality..." << std::endl;
        return true;
    }

private:
    bool    quality_check_enabled_;
    int32_t quality_reference_;
    int32_t maximal_difference_;
};

void do_one_production_cycle() {
    {
        // 3 parameters, 2 unused
        auto producer = ProductProducer{false, 1, 3};
        producer.produce_one(5);
    }

    {
        // 3 parameters, 0 unused
        auto producer = ProductProducer{true, 1, 3};
        producer.produce_one(5);
    }
}
