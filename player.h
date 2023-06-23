#pragma once

#include <string>

using namespace std;

enum State
{
    ingame,
    dying,
    out
};

class Player
{
    public:
        Player(string name);
        Player(string name, int team_id);
        ~Player() {}

        void SetState(State state);
        State GetState() const;
        string GetName() const;
        int GetTeam() const;

    private: 
        string name_;
        State state_;
        int team_;
};