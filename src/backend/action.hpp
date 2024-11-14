#pragma once

#include "coordinates.hpp"

class Game; //forward declaration

class Action {
public:
    Action(const coordinates<size_t>& target):
        target_(target){}

    virtual ~Action() = default;

    [[nodiscard]]
    virtual const coordinates<size_t>& target() const;

    virtual void execute(Game& game) const = 0;

protected:
    const coordinates<size_t>& target_;
};

class ItemAction : public Action {
public:
    ItemAction(int acc, int hp_eff, int area_of_effect, const coordinates<size_t>& target):
        Action(target), accuracy_(acc), hp_effect_(hp_eff), area_of_effect_(area_of_effect) {}

    [[nodiscard]]
    int accuracy() const;

    [[nodiscard]]
    int hp_effect() const;

    virtual void execute(Game& game) const;

private:
    const int accuracy_;
    const int hp_effect_;
    const int area_of_effect_;
};
