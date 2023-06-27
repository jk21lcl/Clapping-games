#pragma once

#include "game.h"

enum GameMode
{
    no_computer,
    full_easy,
    full_hard,
    partial_easy,
    partial_hard
};

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
        Bean(GameMode mode = no_computer);

        void Start() override;

    protected:
        const int consume[16] = {-1,1,2,3,0,0,0,1,1,5,0,0,0,1,2,2};
        const string option_name[16] = {"accumulate", "single shot", "double shot", "triple shot",
        "small defense", "medium defense", "big defense", "super defense", "break super defense",
        "kill", "rebound", "double rebound", "anti rebound", "disturb", "taunt", "purify"};

        vector<Option> last_;
        vector<int> damage_; // damage_matrix, (i,j) means i damages j
        // 4 means break_super_defense, 5 means kill
        vector<int> ori_beans_; // the number of beans at the beginning of this round
        vector<int> beans_;
        GameMode mode_;

        bool is_anti_rebound_; // whether there exists anti rebound
        vector<bool> has_disturb_; // whether a team uses disturb
        vector<bool> is_disturbed_; // whether a team is disturbed
        vector<int> num_taunt_; // the number of taunt in a team
        vector<bool> is_purified_; // whether a team is purified

        bool comp_use_attack; // whether computer uses normal attack
        bool comp_use_anti_rebound; // whether computer uses anti rebound
        bool comp_use_disturb; // whether computer uses disturb
        bool comp_use_taunt; // whether computer uses taunt
        bool comp_use_purify; // whether computer uses purify

        void ShowOption() const override;
        void ShowInfo() const override;
        void Judge() override;

        void Input(int p_id);
        void Process();
        void ComputerAct(int p_id, int round);

        // pure random except always accumulate at the first round
        void FullEasy(int p_id, int round);
        // use some strategy
        void FullHard(int p_id);
        // pure random except always accumulate at the first round
        void PartialEasy(int p_id, int round);
        // use some strategy
        void PartialHard(int p_id);

        int AttackPlayer(); // attack an living player, returns the player_id
        void RandomAttack(int p_id, Option option); // randomly attack a living enemy
};