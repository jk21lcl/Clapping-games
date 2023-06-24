#include "accumulating_beans.h"

using namespace std;

void Bean::ShowInfo() const
{
    cout << "\033[0;32mNow players have beans:" << endl;
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i))
            cout << "  " << players_[i]->GetName() << ": " << beans_[i] << endl;
    cout << "\033[0m";
}

void Bean::ShowOption() const
{
    cout << "\033[0;33m  Options:" << endl;
    cout << "    0: accumulate    1: single_shot    2: double_shot    3: triple_shot" << endl;
    cout << "    4: small_defense    5: medium_defense    6: big_defense    7: super_defense" << endl;
    cout << "    8: break_super_defense    9: kill\033[0m" << endl;
}

void Bean::Input(int p_id)
{
    int choice;
    int tar;
    while (true)
    {
        cin >> choice;
        if (choice >= 0 && choice <= 9)
        {
            if (beans_[p_id] >= consume[choice])
            {
                last_[p_id] = (Option)choice;
                beans_[p_id] -= consume[choice];
                if (choice == 1)
                {
                    cout << "Enter the player_id of your shot." << endl;
                    cin >> tar;
                    damage_[p_id * num_p + (tar - 1)]++;
                }
                if (choice == 2)
                {
                    cout << "Enter the player_id of each shot." << endl;
                    for (int i = 0; i < 2; i++)
                    {
                        cin >> tar;
                        damage_[p_id * num_p + (tar - 1)]++;
                    }
                }
                if (choice == 3)
                {
                    cout << "Enter the player_id of each shot." << endl;
                    for (int i = 0; i < 3; i++)
                    {
                        cin >> tar;
                        damage_[p_id * num_p + (tar - 1)]++;
                    }
                }
                if (choice == 8)
                {
                    cout << "Enter the player_id of your target." << endl;
                    cin >> tar;
                    damage_[p_id * num_p + (tar - 1)] = 4;
                }
                if (choice == 9)
                {
                    cout << "Enter the player_id of your target." << endl;
                    cin >> tar;
                    damage_[p_id * num_p + (tar - 1)] = 5;
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
    // process the shot to oneself
    for (int i = 0; i < num_p; i++)
    {
        if (IsLiving(i))
            damage_[i * num_p + i] = 0;
    }

    // cancel the mutual shots
    for (int i = 0; i < num_p; i++)
        for (int j = 0; j < num_p; j++)
        {
            if (IsLiving(i) && IsLiving(j))
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

void Bean::Start()
{
    last_.resize(num_p);
    damage_.resize(num_p * num_p);
    beans_.resize(num_p);
    for (int i = 0; i < num_p; i++)
        beans_[i] = 0;

    int round = 1;
    while (CheckTeam())
    {
        while (CheckInGame())
        {
            cout << "\033[0;35m" << endl << "Round " << round << ":\033[0m" << endl << endl;
            for (int i = 0; i < num_p * num_p; i++)
                damage_[i] = 0;

            ShowInfo();
            for (int i = 0; i < num_p; i++)
            {
                if (IsLiving(i))
                {
                    cout << "It's " << players_[i]->GetName() << "'s turn." << endl;
                    ShowOption();
                    Input(i);
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