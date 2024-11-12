#include "action.hpp"
#include "game.hpp"

int ItemAction::accuracy() const {
    return accuracy_;
}

int ItemAction::hp_effect() const {
    return hp_effect_;
}

const coordinates<size_t>& Action::target() const {
    return target_;
}

void ItemAction::execute(Game &game) const {
    std::cout << "Dealing " << hp_effect() << " damage to enemy at " << target().toString() 
              << " with accuracy " << accuracy() << " %" << std::endl;
}
