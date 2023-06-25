#include "accumulating_beans.h"

using namespace std;

Bean::Bean() : Game() {}

void Bean::ShowInfo() const
{
    cout << "\033[0;32m";
    cout << "Now all players have beans: (id of each player is before the name)" << endl;
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i))
            cout << " " << i + 1 << "   " << players_[i]->GetName() << ": " << beans_[i] << endl;
    cout << "\033[0m";
}

void Bean::ShowOption() const
{
    cout << "\033[0;33m";
    cout << "  Options:" << endl;
    cout << "    0: accumulate    1: single_shot    2: double_shot    3: triple_shot" << endl;
    cout << "    4: small_defense    5: medium_defense    6: big_defense    7: super_defense" << endl;
    cout << "    8: break_super_defense    9: kill    10: rebound    11: double_rebound" << endl;
    cout << "    12: anti_rebound    13: disturb    14: taunt" << endl;
    cout << "\033[0m";
}

void Bean::Input(int p_id)
{
    int choice;
    int tar;
    while (true)
    {
        cin >> choice;
        if (choice >= 0 && choice <= 14)
        {
            if (beans_[p_id] >= consume[choice])
            {
                last_[p_id] = (Option)choice;
                beans_[p_id] -= consume[choice];
                switch (choice)
                {
                    case 1:
                        cout << "Enter the player_id of your shot." << endl;
                        cin >> tar;
                        damage_[p_id * num_p + (tar - 1)]++;
                        break;
                    case 2:
                        cout << "Enter the player_id of each shot." << endl;
                        for (int i = 0; i < 2; i++)
                        {
                            cin >> tar;
                            damage_[p_id * num_p + (tar - 1)]++;
                        }
                        break;
                    case 3:
                        cout << "Enter the player_id of each shot." << endl;
                        for (int i = 0; i < 3; i++)
                        {
                            cin >> tar;
                            damage_[p_id * num_p + (tar - 1)]++;
                        }
                        break;
                    case 8:
                        cout << "Enter the player_id of your target." << endl;
                        cin >> tar;
                        damage_[p_id * num_p + (tar - 1)] = 4;
                        break;
                    case 9:
                        cout << "Enter the player_id of your target." << endl;
                        cin >> tar;
                        damage_[p_id * num_p + (tar - 1)] = 5;
                        break;
                    case 12:
                        is_anti_rebound_ = true;
                        break;
                    case 13:
                        for (int i = 0; i < num_t; i++)
                        {
                            if (i != players_[p_id]->GetTeam())
                                is_disturbed_[i] = true;
                        }
                        break;
                    case 14:
                        num_taunt_[players_[p_id]->GetTeam()]++;
                        break;
                    default:
                        break;
                }
                break;
            }
            else
            {
                cout << "You don't have enough beans. Please input again." << endl;
                continue;
            }
        }
        cout << "Illegal input. Please input again." << endl;
        continue;
    }
}

void Bean::Process()
{
    // taunt
    for (int i = 0; i < num_t; i++)
        if (teams_[i]->GetState() == ingame && num_taunt_[i] == 1)
        {
            int t_id = -1;
            for (int j = 0; j < num_p; j++)
                if (IsLiving(j) && last_[j] == taunt && players_[j]->GetTeam() == i)
                    t_id = j;
            for (int j = 0; j < num_p; j++)
                if (IsLiving(j) && players_[j]->GetTeam() == i)
                    for (int k = 0; k < num_p; k++)
                    {
                        int d = damage_[k * num_p + j];
                        if (d >= 1 && d <= 3)
                        {
                            damage_[k * num_p + t_id] += d;
                            damage_[k * num_p + j] -= d;
                        }
                        if (d == 4)
                            damage_[k * num_p + j] = 0;
                    }
        }

    // rebound
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i) && last_[i] == rebound && !(is_disturbed_[players_[i]->GetTeam()]))
        {
            int damage = 0;
            for (int j = 0; j < num_p; j++)
            {
                int d = damage_[j * num_p + i];
                if (d >= 1 && d <= 3)
                    damage += d;
            }
            if (damage == 1)
                for (int j = 0; j < num_p; j++)
                {
                    int d = damage_[j * num_p + i];
                    if (d >= 1 && d <= 3)
                    {
                        damage_[i * num_p + j] = d;
                        damage_[j * num_p + i] = 0;
                    }
                }
        }

    // double rebound
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i) && last_[i] == double_rebound && !(is_disturbed_[players_[i]->GetTeam()]))
        {
            int damage = 0;
            for (int j = 0; j < num_p; j++)
            {
                int d = damage_[j * num_p + i];
                if (d >= 1 && d <= 3)
                    damage += d;
            }
            if (damage == 2)
                for (int j = 0; j < num_p; j++)
                {
                    int d = damage_[j * num_p + i];
                    if (d >= 1 && d <= 3)
                    {
                        damage_[i * num_p + j] = d;
                        damage_[j * num_p + i] = 0;
                    }
                }
        }

    // cancel the mutual shots
    for (int i = 0; i < num_p; i++)
        for (int j = 0; j < num_p; j++)
        {
            if (i != j && IsLiving(i) && IsLiving(j))
            {
                int d_1 = damage_[i * num_p + j];
                int d_2 = damage_[j * num_p + i];
                if (d_1 >= 1 && d_1 <= 3 && d_2 >= 1 && d_2 <= 3)
                {
                    int m = min(d_1, d_2);
                    damage_[i * num_p + j] -= m;
                    damage_[j * num_p + i] -= m;
                }
            }
        }
}

void Bean::Judge()
{
    // normal judge
    for (int i = 0; i < num_p; i++)
    {
        if (IsLiving(i) && last_[i] != kill)
        {
            if (last_[i] == super_defense)
            {
                for (int j = 0; j < num_p; j++)
                {
                    if (damage_[j * num_p + i] == 4)
                    {
                        players_[i]->SetState(dying);
                        beans_[j] += 2;
                    }
                    if (damage_[j * num_p + i] == 5)
                        players_[i]->SetState(dying);
                } 
            }
            else
            {
                int damage = 0;
                for (int j = 0; j < num_p; j++)
                {
                    int d = damage_[j * num_p + i];
                    if (d == 5)
                    {
                        players_[i]->SetState(dying);
                        break;
                    }
                    if (d >= 1 && d <= 3)
                        damage += d;
                }
                if (last_[i] == taunt)
                {
                    int enemy_teams = 0;
                    int enemy_players = 0;
                    for (int j = 0; j < num_p; j++)
                        if (IsLiving(j) && !BeTeammate(i, j))
                            enemy_players++;
                    for (int j = 0; j < num_t; j++)
                        if (teams_[j]->GetState() == ingame && j != players_[i]->GetTeam())
                            enemy_teams++;
                    int shield = enemy_players / enemy_teams;
                    if (damage > shield)
                        players_[i]->SetState(dying);
                }
                else
                {
                    if (damage == 1 && last_[i] != small_defense && last_[i] != break_super_defense)
                        players_[i]->SetState(dying);
                    if (damage == 2 && last_[i] != medium_defense)
                        players_[i]->SetState(dying);
                    if (damage >= 3 && last_[i] != big_defense)
                        players_[i]->SetState(dying);
                }
            }
        }
    }

    // anti rebound
    if (is_anti_rebound_)
        for (int i = 0; i < num_p; i++)
            if (IsLiving(i) && (last_[i] == rebound || last_[i] == double_rebound))
                players_[i]->SetState(dying);
}

void Bean::ComputerAct(int p_id, int round)
{
    cout << "\033[34;1m";
    if (round == 1)
    {
        last_[p_id] = accumulate;
        beans_[p_id]++;
        cout << players_[p_id]->GetName() << " uses accumulate." << endl;
    }
    else
    {
        while (true)
        {
            int choice = rand() % 15;
            if (beans_[p_id] < consume[choice])
                continue;
            last_[p_id] = (Option)choice;
            beans_[p_id] -= consume[choice];
            cout << players_[p_id]->GetName() << " uses " << option_name[choice] << ".";

            int tar;
            int times = 0;
            switch (choice)
            {
                case 1:
                    cout << " Target: ";
                    while (times < 1)
                    {
                        tar = rand() % num_p;
                        if (IsLiving(tar) && !(BeTeammate(tar, p_id)))
                        {
                            times++;
                            damage_[p_id * num_p + tar]++;
                            cout << players_[tar]->GetName();
                            if (times != 1)
                                cout << ", ";
                            else
                                cout << ".";
                        }
                    }
                    break;
                case 2:
                    cout << " Target: ";
                    while (times < 2)
                    {
                        tar = rand() % num_p;
                        if (IsLiving(tar) && !(BeTeammate(tar, p_id)))
                        {
                            times++;
                            damage_[p_id * num_p + tar]++;
                            cout << players_[tar]->GetName();
                            if (times != 2)
                                cout << ", ";
                            else
                                cout << ".";
                        }
                    }
                    break;
                case 3:
                    cout << " Target: ";
                    while (times < 3)
                    {
                        tar = rand() % num_p;
                        if (IsLiving(tar) && !(BeTeammate(tar, p_id)))
                        {
                            times++;
                            damage_[p_id * num_p + tar]++;
                            cout << players_[tar]->GetName();
                            if (times != 3)
                                cout << ", ";
                            else
                                cout << ".";
                        }
                    }
                    break;
                case 8:
                    cout << " Target: ";
                    while (times < 1)
                    {
                        tar = rand() % num_p;
                        if (IsLiving(tar) && !(BeTeammate(tar, p_id)))
                        {
                            times++;
                            damage_[p_id * num_p + tar] = 4;
                            cout << players_[tar]->GetName();
                            if (times != 1)
                                cout << ", ";
                            else
                                cout << ".";
                        }
                    }
                    break;
                case 9:
                    cout << " Target: ";
                    while (times < 1)
                    {
                        tar = rand() % num_p;
                        if (IsLiving(tar) && !(BeTeammate(tar, p_id)))
                        {
                            times++;
                            damage_[p_id * num_p + tar] = 5;
                            cout << players_[tar]->GetName();
                            if (times != 1)
                                cout << ", ";
                            else
                                cout << ".";
                        }
                    }
                    break;
                case 12:
                    is_anti_rebound_ = true;
                    break;
                case 13:
                    for (int i = 0; i < num_t; i++)
                    {
                        if (i != players_[p_id]->GetTeam())
                            is_disturbed_[i] = true;
                    }
                    break;
                case 14:
                    num_taunt_[players_[p_id]->GetTeam()]++;
                    break;
                default:
                    break;
            }
            cout << endl;
            break;
        }
    }
    cout << "\033[0m";
}

void Bean::Start()
{
    last_.resize(num_p);
    damage_.resize(num_p * num_p);
    beans_.resize(num_p);
    is_disturbed_.resize(num_t);
    num_taunt_.resize(num_t);
    for (int i = 0; i < num_p; i++)
        beans_[i] = 0;
    srand(time(NULL));

    int round = 1;
    while (CheckTeam())
    {
        while (CheckInGame())
        {
            cout << "\033[0;35m" << endl << "Round " << round << ":\033[0m" << endl << endl;
            for (int i = 0; i < num_p * num_p; i++)
                damage_[i] = 0;
            for (int i = 0; i < num_t; i++)
                is_disturbed_[i] = false;
            for (int i = 0; i < num_t; i++)
                num_taunt_[i] = 0;
            is_anti_rebound_ = false;

            ShowInfo();
            for (int i = 0; i < num_p; i++)
            {
                if (IsLiving(i))
                {
                    if (IsHuman(i))
                    {
                        cout << "It's " << players_[i]->GetName() << "'s turn." << endl;
                        ShowOption();
                        Input(i);
                    }
                    else
                        ComputerAct(i, round);
                }
            }
            Process();
            Judge();
            round++;
        }
        for (int i = 0; i < num_p; i++)
            if (players_[i]->GetState() == dying)
                cout << "\033[0;31m" << players_[i]->GetName() << " is out.\033[0m" << endl;
        UpdatePlayerState();
        UpdateTeamState();
    }

    End();
}