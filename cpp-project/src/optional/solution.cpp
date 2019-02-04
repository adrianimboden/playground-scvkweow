#include <cstdint>
#include <iostream>
#include <optional>

struct QualityGoal {
    int32_t quality_reference;
    int32_t maximal_difference;
};

class ProductProducer {
public:
    explicit ProductProducer(std::optional<QualityGoal> quality_goal)
        : quality_goal_{quality_goal} {
    }

    bool produce_one(int32_t quality) {
        if (quality_goal_) {
            std::cerr << "checking quality..." << std::endl;
            const auto difference = quality - quality_goal_->quality_reference;
            return difference <= quality_goal_->maximal_difference;
        }
        std::cerr << "not checking quality..." << std::endl;
        return true;
    }

private:
    std::optional<QualityGoal> quality_goal_;
};

void do_one_production_cycle() {
    {
        // 1 parameters, 0 unused
        auto producer = ProductProducer{std::nullopt};
        producer.produce_one(5);
    }

    {
        // 1 parameters, 0 unused
        auto producer = ProductProducer{QualityGoal{1, 3}};
        producer.produce_one(5);
    }
}
