#pragma once

#include "player.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class Game
{
    public:
        Game();
        ~Game();

        void AddPlayer(string name, int team_id, Mode mode = human);
        void AddTeam(string name);

        virtual void Start() = 0;
    
    protected:
        vector<Player*> players_;
        vector<Player*> teams_;
        int num_p; // number of players
        int num_t; // number of teams

        bool CheckInGame() const; // check whether all players are ingame 
        bool CheckTeam() const; // check whether there are at least 2 living teams
        bool IsLiving(int p_id) const;
        bool IsHuman(int p_id) const;
        bool BeTeammate(int id_1, int id_2) const; // check whether they are teammates
        void UpdateTeamState();
        void UpdatePlayerState();
        void End() const;
        
        virtual void ShowOption() const = 0;
        virtual void ShowInfo() const = 0; // show other players current state
        virtual void Judge() = 0;
};