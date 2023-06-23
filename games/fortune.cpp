#include "fortune.h"

void Fortune::ShowInfo() const
{
    cout << "\033[0;32mNow players have fortunes:" << endl;
    for (int i = 0; i < num_p; i++)
        if (IsLiving(i))
            cout << "  " << players_[i]->GetName() << ": " << fortunes_[i] << endl;
    cout << "\033[0m";
}

void Fortune::ShowOption() const
{
    cout << "\033[0;33m  Options:" << endl;
    cout << "    0: fortune    1: defend    2: spear" << endl;
    cout << "    3: double spear    4: trace    5: gold\033[0m" << endl;
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

void Fortune::Start()
{
    last_.resize(num_p);
    fortunes_.resize(num_p);

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
                    cout << "It's " << players_[i]->GetName() << "'s turn." << endl;
                    ShowOption();
                    Input(i, round);
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