#include "accumulating_beans.h"

using namespace std;

Bean::Bean(GameMode mode) : Game()
{
    mode_ = mode;
}

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
    switch (mode_)
    {
        case partial_easy: case partial_hard:
            cout << "  Options:" << endl;
            cout << "    0: accumulate    1: single_shot    2: double_shot    4: small_defense    5: medium_defense" << endl;
            break;
        default:
            cout << "  Options:" << endl;
            cout << "    0: accumulate             1: single_shot       2: double_shot    3: triple_shot" << endl;
            cout << "    4: small_defense          5: medium_defense    6: big_defense    7: super_defense" << endl;
            cout << "    8: break_super_defense    9: kill              10: rebound       11: double_rebound" << endl;
            cout << "    12: anti_rebound          13: disturb          14: taunt         15: purify" << endl;
            break;
    }
    cout << "\033[0m";
}

int Bean::AttackPlayer()
{
    int tar;
    while (true)
    {
        cin >> tar;
        if (tar <= 0 || tar > num_p)
        {
            cout << "Illegal input. Please input again." << endl;
            continue;
        }
        if (!IsLiving(tar - 1))
        {
            cout << "You cannot attack player that has been out. Please input again." << endl;
            continue;
        }
        break;
    }
    return tar;
}

void Bean::RandomAttack(int p_id, Option option)
{
    int tar;
    int times;
    switch (option)
    {
        case single_shot: case break_super_defense: case kill:
            times = 1;
            break;
        case double_shot:
            times = 2;
            break;
        case triple_shot:
            times = 3;
            break;
        default:
            times = 0;
            break;
    }

    int n = 0;
    cout << " Target: ";
    while (n < times)
    {
        tar = rand() % num_p;
        if (IsLiving(tar) && !(BeTeammate(tar, p_id)))
        {
            n++;
            switch (option)
            {
                case single_shot: case double_shot: case triple_shot:
                    damage_[p_id * num_p + tar]++;
                    break;
                case break_super_defense:
                    damage_[p_id * num_p + tar] = 4;
                    break;
                case kill:
                    damage_[p_id * num_p + tar] = 5;
                    break;
                default:
                    break;
            }
            cout << players_[tar]->GetName();
            if (n != times)
                cout << ", ";
            else
                cout << ".";
        }
    }
}

void Bean::Input(int p_id)
{
    int choice;
    int tar;
    switch (mode_)
    {
        case partial_easy: case partial_hard:
            while (true)
            {
                cin >> choice;
                if (choice >= 0 && choice <= 5 && choice != 3)
                {
                    if (beans_[p_id] >= consume[choice])
                    {
                        last_[p_id] = (Option)choice;
                        beans_[p_id] -= consume[choice];
                        switch (choice)
                        {
                            case 1:
                                cout << "Enter the player_id of your shot." << endl;
                                tar = AttackPlayer();
                                damage_[p_id * num_p + (tar - 1)]++;
                                break;
                            case 2:
                                cout << "Enter the player_id of each shot." << endl;
                                for (int i = 0; i < 2; i++)
                                {
                                    tar = AttackPlayer();
                                    damage_[p_id * num_p + (tar - 1)]++;
                                }
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
            break;
        default:
            while (true)
            {
                cin >> choice;
                if (choice >= 0 && choice <= 15)
                {
                    if (beans_[p_id] >= consume[choice])
                    {
                        last_[p_id] = (Option)choice;
                        beans_[p_id] -= consume[choice];
                        switch (choice)
                        {
                            case 1:
                                cout << "Enter the player_id of your shot." << endl;
                                tar = AttackPlayer();
                                damage_[p_id * num_p + (tar - 1)]++;
                                break;
                            case 2:
                                cout << "Enter the player_id of each shot." << endl;
                                for (int i = 0; i < 2; i++)
                                {
                                    tar = AttackPlayer();
                                    damage_[p_id * num_p + (tar - 1)]++;
                                }
                                break;
                            case 3:
                                cout << "Enter the player_id of each shot." << endl;
                                for (int i = 0; i < 3; i++)
                                {
                                    tar = AttackPlayer();
                                    damage_[p_id * num_p + (tar - 1)]++;
                                }
                                break;
                            case 8:
                                cout << "Enter the player_id of your target." << endl;
                                tar = AttackPlayer();
                                damage_[p_id * num_p + (tar - 1)] = 4;
                                break;
                            case 9:
                                cout << "Enter the player_id of your target." << endl;
                                tar = AttackPlayer();
                                damage_[p_id * num_p + (tar - 1)] = 5;
                                break;
                            case 12:
                                is_anti_rebound_ = true;
                                break;
                            case 13:
                                has_disturb_[players_[p_id]->GetTeam()] = true;
                                break;
                            case 14:
                                num_taunt_[players_[p_id]->GetTeam()]++;
                                break;
                            case 15:
                                for (int i = 0; i < num_t; i++)
                                {
                                    if (i != players_[p_id]->GetTeam())
                                        is_purified_[i] = true;
                                }
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
            break;
    }
}

void Bean::Process()
{
    // disturb
    for (int i = 0; i < num_t; i++)
    {
        if (teams_[i]->GetState() == ingame && has_disturb_[i] && !is_purified_[i])
            for (int j = 0; j < num_t; j++)
                if (teams_[j]->GetState() == ingame && j != i)
                    is_disturbed_[j] = true;
    }

    // taunt
    for (int i = 0; i < num_t; i++)
        if (teams_[i]->GetState() == ingame && num_taunt_[i] == 1 && !is_purified_[i])
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
        if (IsLiving(i) && last_[i] != kill && last_[i] != purify)
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
                if (last_[i] == taunt && !is_purified_[players_[i]->GetTeam()])
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
    switch (mode_)
    {
        case full_easy:
            FullEasy(p_id, round);
            break;
        case full_hard:
            FullHard(p_id);
            break;
        case partial_easy:
            PartialEasy(p_id, round);
            break;
        case partial_hard:
            PartialHard(p_id);
            break;
        default:
            break;
    }
}

void Bean::Start()
{
    last_.resize(num_p);
    damage_.resize(num_p * num_p);
    ori_beans_.resize(num_p);
    beans_.resize(num_p);
    has_disturb_.resize(num_t);
    is_disturbed_.resize(num_t);
    num_taunt_.resize(num_t);
    is_purified_.resize(num_t);
    for (int i = 0; i < num_p; i++)
        ori_beans_[i] = 0;
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
                has_disturb_[i] = false;
            for (int i = 0; i < num_t; i++)
                is_disturbed_[i] = false;
            for (int i = 0; i < num_t; i++)
                num_taunt_[i] = 0;
            for (int i = 0; i < num_t; i++)
                is_purified_[i] = false;
            is_anti_rebound_ = false;

            comp_use_attack = false;
            comp_use_anti_rebound = false;
            comp_use_disturb = false;
            comp_use_taunt = false;
            comp_use_purify = false;

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
            for (int i = 0; i < num_p; i++)
                ori_beans_[i] = beans_[i];
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

void Bean::FullEasy(int p_id, int round)
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
            int choice = rand() % 16;
            if (beans_[p_id] < consume[choice])
                continue;
            last_[p_id] = (Option)choice;
            beans_[p_id] -= consume[choice];
            cout << players_[p_id]->GetName() << " uses " << option_name[choice] << ".";

            switch (choice)
            {
                case 1:
                    RandomAttack(p_id, single_shot);
                    break;
                case 2:
                    RandomAttack(p_id, double_shot);
                    break;
                case 3:
                    RandomAttack(p_id, triple_shot);
                    break;
                case 8:
                    RandomAttack(p_id, break_super_defense);
                    break;
                case 9:
                    RandomAttack(p_id, kill);
                    break;
                case 12:
                    is_anti_rebound_ = true;
                    break;
                case 13:
                    has_disturb_[players_[p_id]->GetTeam()] = true;
                    break;
                case 14:
                    num_taunt_[players_[p_id]->GetTeam()]++;
                    break;
                case 15:
                    for (int i = 0; i < num_t; i++)
                    {
                        if (i != players_[p_id]->GetTeam())
                            is_purified_[i] = true;
                    }
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

void Bean::FullHard(int p_id)
{
    cout << "\033[34;1m";
    int prob[16] = {20,15,10,10,15,4,1,2,3,0,2,1,2,6,10,10};

    int max_enemy_bean = 0;
    int max_ally_bean = 0;
    int num_enemy = 0;
    int num_ally = 0;

    for (int i = 0; i < num_p; i++)
    {
        if (IsLiving(i))
        {
            int b = ori_beans_[i];
            if (!BeTeammate(p_id, i))
            {
                num_enemy++;
                if (b > max_enemy_bean)
                    max_enemy_bean = b;
            }
            else
            {
                num_ally++;
                if (b > max_ally_bean)
                    max_ally_bean = b;
            }
        }
    }

    // process the probability distribution

    //// if enemy is few, more probability to use rebound and anti rebound, 
    //// don't use disturb, taunt, purify
    if (num_enemy <= 2)
    {
        prob[10] = 10;
        prob[11] = 4;
        prob[12] = 5;
        prob[13] = 0;
        prob[14] = 0;
        prob[15] = 0;
    }
    //// if computer has used disturb or purify, or has more than 2 beans,
    //// then more probability to attack
    if (comp_use_disturb || comp_use_purify || beans_[p_id] > 2)
    {
        prob[1] = 20;
        prob[2] = 15;
        prob[3] = 15;
    }
    
    //// if ally is much, don't use anti rebound 
    if (num_ally >= 4)
    {
        prob[12] = 0;
    }
    //// if ally is few, don't use taunt
    if (num_ally <= 2)
    {
        prob[14] = 0;
    }
    //// at most one computer use disturb, taunt, purify
    if (comp_use_anti_rebound)
        prob[12] = 0;
    if (comp_use_disturb)
        prob[13] = 0;
    if (comp_use_taunt)
        prob[14] = 0;
    if (comp_use_purify)
        prob[15] = 0;
    //// if all enemy has at most 2 beans, don't use big defense
    if (max_enemy_bean <= 2)
    {
        prob[6] = 0;
    }
    //// if all enemy has at most 1 bean, don't use medium defense, super defense, 
    //// break super defense, double rebound
    if (max_enemy_bean <= 1)
    {
        prob[5] = 0;
        prob[7] = 0;
        prob[8] = 0;
        prob[11] = 0;
    }
    //// if no enemy has bean, don't use small defense, rebound, taunt, purify
    if (max_enemy_bean == 0)
    {
        prob[4] = 0;
        prob[10] = 0;
        prob[14] = 0;
        prob[15] = 0;
    }
    //// if all ally has at most 1 bean, don't use break super defense
    if (max_ally_bean <= 1)
    {
        prob[8] = 0;
    }
    //// if no ally has been, don't use anti rebound
    if (max_ally_bean == 0)
    {
        prob[12] = 0;
    }
    //// if no computer attacks, don't use disturb, taunt, purify
    if (!comp_use_attack)
    {
        prob[13] = 0;
        prob[15] = 0;
    }

    int sum[16];
    sum[0] = prob[0];
    for (int i = 1; i < 16; i++)
        sum[i] = sum[i - 1] + prob[i];

    // determine the choice

    int choice = 0;
    //// when itself has at least 5 beans, always kill
    if (beans_[p_id] >= 5)
        choice = 9;
    else
    {
        while (true)
        {
            int r = rand() % sum[15];
            for (int i = 0; i < 16; i++)
                if (r < sum[i])
                {
                    choice = i;
                    break;
                }
        
            if (beans_[p_id] < consume[choice])
                continue;
            break;
        }
    }

    // deal with the choice
    last_[p_id] = (Option)choice;
    beans_[p_id] -= consume[choice];
    cout << players_[p_id]->GetName() << " uses " << option_name[choice] << ".";
    switch (choice)
    {
        case 1:
            RandomAttack(p_id, single_shot);
            comp_use_attack = true;
            break;
        case 2:
            RandomAttack(p_id, double_shot);
            comp_use_attack = true;
            break;
        case 3:
            RandomAttack(p_id, triple_shot);
            comp_use_attack = true;
            break;
        case 8:
            RandomAttack(p_id, break_super_defense);
            break;
        case 9:
            RandomAttack(p_id, kill);
            break;
        case 12:
            is_anti_rebound_ = true;
            comp_use_anti_rebound = true;
            break;
        case 13:
            has_disturb_[players_[p_id]->GetTeam()] = true;
            comp_use_disturb = true;
            break;
        case 14:
            num_taunt_[players_[p_id]->GetTeam()]++;
            comp_use_taunt = true;
            break;
        case 15:
            for (int i = 0; i < num_t; i++)
            {
                if (i != players_[p_id]->GetTeam())
                    is_purified_[i] = true;
            }
            comp_use_purify = true;
            break;
        default:
            break;
    }
    cout << endl;
    
    cout << "\033[0m";
}

void Bean::PartialEasy(int p_id, int round)
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
            int choice = rand() % 6;
            if (choice == 3 || beans_[p_id] < consume[choice])
                continue;
            last_[p_id] = (Option)choice;
            beans_[p_id] -= consume[choice];
            cout << players_[p_id]->GetName() << " uses " << option_name[choice] << ".";

            switch (choice)
            {
                case 1:
                    RandomAttack(p_id, single_shot);
                    break;
                case 2:
                    RandomAttack(p_id, double_shot);
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

void Bean::PartialHard(int p_id)
{
    cout << "\033[34;1m";

    int max_enemy_bean = 0;
    for (int i = 0; i < num_p; i++)
    {
        if (IsLiving(i) && !BeTeammate(i, p_id) && ori_beans_[i] > max_enemy_bean)
            max_enemy_bean = ori_beans_[i];
    }

    while (true)
    {
        int choice = rand() % 6;
        if (choice == 3 || beans_[p_id] < consume[choice])
            continue;
        // when enemy has at most 1 bean, don't use medium defense
        if (choice == 5 && max_enemy_bean <= 1)
            continue;
        // when enemy has no bean, don't use small defense
        if (choice == 4 && max_enemy_bean == 0)
            continue;
        // when itself has at least 2 beans, more probability to attack
        if (ori_beans_[p_id] >= 2 && (choice == 0 || choice == 4 || choice == 5))
        {
            int p = rand() % 3;
            if (p != 0)
                continue;
        }
        last_[p_id] = (Option)choice;
        beans_[p_id] -= consume[choice];
        cout << players_[p_id]->GetName() << " uses " << option_name[choice] << ".";

        switch (choice)
        {
            case 1:
                RandomAttack(p_id, single_shot);
                break;
            case 2:
                RandomAttack(p_id, double_shot);
                break;
            default:
                break;
        }
        cout << endl;
        break;
    }
    cout << "\033[0m";
}