#pragma once

#include "game.h"

enum Option
{
    accumulate,
    single_shot,
    double_shot,
    triple_shot,
    small_defense,
    medium_defense,
    big_defense,
    super_defense
};

class Bean : public Game
{
    public:
        Bean() : Game() {}

        void Start() override;

    private:
        const int consume[8] = {-1,1,2,3,0,0,0,1};

        vector<Option> last_;
        vector<int> damage_; // damage_matrix, (i,j) means i damages j
        vector<int> beans_;

        void ShowOption() const override;
        void ShowInfo() const override;
        void Judge() override;

        void Input(int p_id);
        void Process();
};