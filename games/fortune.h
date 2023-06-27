#pragma once

#include "game.h"

enum GameMode
{
    no_computer,
    with_computer
};

enum Option
{
    fortune,
    defend,
    spear,
    double_spear,
    trace,
    gold
};

class Fortune : public Game
{
    public:
        Fortune(GameMode mode = no_computer);

        void Start() override;

    private:
        const int consume[6]={-1,0,1,2,3,0};
        const int compare[6][6]={{0,0,2,2,2,0},{0,0,0,0,2,0},{1,0,0,2,2,1},
        {1,0,1,0,2,0},{1,1,1,1,0,0},{0,0,2,0,0,0}};
        // 0 means tie, 1 means the first one wins, 2 means the second one wins
        const string option_name[6]={"fortune", "defend", "spear", "double spear", "trace", "gold"};

        vector<int> fortunes_;
        vector<Option> last_;
        GameMode mode_;

        void ShowOption() const override;
        void ShowInfo() const override;
        void Judge() override;
        
        void Input(int p_id, int round);
        void ComputerAct(int p_id, int round);
        // pure random except always use fortune at the first round
};