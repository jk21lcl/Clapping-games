#include "fortune.h"

void Fortune::ShowInfo() const
{
    cout << "\033[0;32m";
    cout << "Now all players have fortunes: (id of each player is before the name)" << endl;
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i))
            cout << " " << i + 1 << "   " << players_[i]->GetName() << ": " << fortunes_[i] << endl;
    cout << "\033[0m";
}

void Fortune::ShowOption() const
{
    cout << "\033[0;33m";
    cout << "  Options:" << endl;
    cout << "    0: fortune    1: defend    2: spear" << endl;
    cout << "    3: double spear    4: trace    5: gold" << endl;
    cout << "\033[0m";
}

void Fortune::Input(int p_id, int round)
{
    int choice;
    while (true)
    {
        cin >> choice;
        if (choice >= 0 && choice <= 5)
        {
            if (round == 1 && choice != 0)
            {
                cout << "You should use fortune at the first round." << endl;
                continue;
            }
            if (fortunes_[p_id] >= consume[choice])
            {
                last_[p_id] = (Option)choice;
                fortunes_[p_id] -= consume[choice];
                break;
            }
            else
            {
                cout << "You don't have enough fortunes." << endl;
                continue;
            }
        }
        cout << "Illegal input. Please input again." << endl;
    }
}

void Fortune::Judge()
{
    for (int i = 0; i < num_p; i++)
        for (int j = 0; j < num_p; j++)
        {
            if (IsLiving(i) && IsLiving(j) && (!BeTeammate(i, j)))
                if (compare[last_[i]][last_[j]] == 2)
                    players_[i]->SetState(dying);
        }
}

void Fortune::ComputerAct(int p_id, int round)
{
    cout << "\033[34;1m";
    if (round == 1)
    {
        last_[p_id] = fortune;
        fortunes_[p_id]++;
        cout << players_[p_id]->GetName() << " uses fortune." << endl;
    }
    else
    {
        while (true)
        {
            int choice = rand() % 6;
            if (fortunes_[p_id] < consume[choice])
                continue;
            last_[p_id] = (Option)choice;
            fortunes_[p_id] -= consume[choice];
            cout << players_[p_id]->GetName() << " uses " << option_name[choice] << "." << endl;
            break;
        }
    }
}

void Fortune::Start()
{
    last_.resize(num_p);
    fortunes_.resize(num_p);
    srand(time(NULL));

    int game = 1;
    while (CheckTeam())
    {
        cout << "\033[1;34m" << endl << "Game " << game << ":\033[0m" << endl;

        for (int i = 0; i < num_p; i++)
            fortunes_[i] = 0;
        int round = 1;
        while (CheckInGame())
        {
            cout << "\033[0;35m" << endl << "Round " << round << ":\033[0m" << endl << endl;
            
            ShowInfo();
            for (int i = 0; i < num_p; i++)
            {
                if (IsLiving(i))
                {
                    if (IsHuman(i))
                    {
                        cout << "It's " << players_[i]->GetName() << "'s turn." << endl;
                        ShowOption();
                        Input(i, round);
                    }
                    else
                        ComputerAct(i, round);
                }
            }
            Judge();
            round++;
        }
        for (int i = 0; i < num_p; i++)
            if (players_[i]->GetState() == dying)
                cout << "\033[0;31m" << players_[i]->GetName() << " is out.\033[0m" << endl;
        UpdatePlayerState();
        UpdateTeamState();
        game++;
    }

    End();
}