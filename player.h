#pragma once

#include <string>

using namespace std;

enum State
{
    ingame,
    dying,
    out
};

enum Mode
{
    human,
    computer
};

class Player
{
    public:
        Player(string name);
        Player(string name, int team_id);
        Player(string name, int team_id, Mode mode);
        ~Player() {}

        void SetState(State state);
        State GetState() const;
        string GetName() const;
        int GetTeam() const;
        Mode GetMode() const;

    private: 
        string name_;
        State state_; // initialize ingame
        int team_; // default 0
        Mode mode_; // default human
};