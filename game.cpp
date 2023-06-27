#include "game.h"

Game::Game()
{
    num_p = 0;
    num_t = 0;
}

Game::~Game()
{
    for (int i = 0; i < num_p; i++)
        delete players_[i];
    for (int i = 0; i < num_t; i++)
        delete teams_[i];
}

void Game::AddPlayer(string name, int team_id, Mode mode)
{
    players_.push_back(new Player(name, team_id, mode));
    num_p++;
}

void Game::AddTeam(string name)
{
    teams_.push_back(new Player(name));
    num_t++;
}

void Game::UpdateTeamState()
{
    for (int i = 0; i < num_t; i++)
        teams_[i]->SetState(out);
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i))
            teams_[players_[i]->GetTeam()]->SetState(ingame);
}

void Game::UpdatePlayerState()
{
    for (int i = 0; i < num_p; i++)
        if (players_[i]->GetState() == dying)
            players_[i]->SetState(out);
}

bool Game::CheckInGame() const
{
    for (int i = 0; i < num_p; i++)
        if (players_[i]->GetState() == dying)
            return false;
    return true;
}

bool Game::CheckTeam() const
{
    int live = 0;
    for (int i = 0; i < num_t; i++)
    {
        if (teams_[i]->GetState() == ingame)
            live++;
    }
    if (live > 1)
        return true;
    return false;
}

bool Game::IsLiving(int p_id) const
{
    return !(players_[p_id]->GetState() == out);
}

bool Game::IsHuman(int p_id) const
{
    return (players_[p_id]->GetMode() == human);
}

bool Game::BeTeammate(int id_1, int id_2) const
{
    return (players_[id_1]->GetTeam() == players_[id_2]->GetTeam());
}

void Game::End() const
{
    bool pvc = false;
    for (int i = 0; i < num_p; i++)
        if (!IsHuman(i))
            pvc = true;
    if (!pvc)
    {
        cout << "\033[1;35m";
        int win_id = -1;
        for (int i = 0; i < num_t; i++)
            if (teams_[i]->GetState() == ingame)
            {
                win_id = i;
                cout << teams_[i]->GetName() + " (";
            }
        if (win_id == -1)
            cout << "It's a tie!\033[0m" << endl;
        else
        {
            bool first = true;
            for (int i = 0; i < num_p; i++)
                if (players_[i]->GetTeam() == win_id)
                {
                    if (first)
                        first = false;
                    else
                        cout << ", ";
                    cout << players_[i]->GetName();
                }
            cout << ") wins!\033[0m" << endl;
        }
    }
    else
    {
        cout << "\033[1;35m";
        for (int i = 0; i < num_t; i++)
            if (teams_[i]->GetState() == ingame)
            {
                cout << teams_[i]->GetName() << " wins!\033[0m" << endl;
                return;
            }
        cout << "It's a tie!\033[0m" << endl;
    }
}