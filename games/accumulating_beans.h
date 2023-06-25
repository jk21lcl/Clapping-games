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
    disturb
};

class Bean : public Game
{
    public:
        Bean();

        void Start() override;

    protected:
        const int consume[14] = {-1,1,2,3,0,0,0,1,1,5,0,0,0,1};
        const string option_name[14] = {"accumulate", "single shot", "double shot", "triple shot",
        "small defense", "medium defense", "big defense", "super defense", "break super defense",
        "kill", "rebound", "double rebound", "anti rebound", "disturb"};

        vector<Option> last_;
        vector<int> damage_; // damage_matrix, (i,j) means i damages j
        // 4 means break_super_defense, 5 means kill
        vector<int> beans_;
        bool is_anti_rebound_;
        vector<bool> is_disturbed_;

        void ShowOption() const override;
        void ShowInfo() const override;
        void Judge() override;

        void Input(int p_id);
        void Process();
        void ComputerAct(int p_id, int round);
};