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
    super_defense,
    break_super_defense,
    kill,
    rebound,
    double_rebound,
    anti_rebound,
    disturb,
    taunt,
    purify
};

class Bean : public Game
{
    public:
        Bean();

        void Start() override;

    protected:
        const int consume[16] = {-1,1,2,3,0,0,0,1,1,5,0,0,0,1,2,2};
        const string option_name[16] = {"accumulate", "single shot", "double shot", "triple shot",
        "small defense", "medium defense", "big defense", "super defense", "break super defense",
        "kill", "rebound", "double rebound", "anti rebound", "disturb", "taunt", "purify"};

        vector<Option> last_;
        vector<int> damage_; // damage_matrix, (i,j) means i damages j
        // 4 means break_super_defense, 5 means kill
        vector<int> beans_;
        bool is_anti_rebound_;
        vector<bool> has_disturbed_;
        vector<bool> is_disturbed_;
        vector<int> num_taunt_;
        vector<bool> is_purified_;

        void ShowOption() const override;
        void ShowInfo() const override;
        void Judge() override;

        void Input(int p_id);
        void Process();
        void ComputerAct(int p_id, int round);
};