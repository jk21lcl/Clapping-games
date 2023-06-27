#include "accumulating_beans.h"
#include <sstream>

int InputPositiveInteger()
{
    int n;
    while (true)
    {
        cin >> n;
        if (n > 0)
            break;
        else
            cout << "Illegal input. Please input again." << endl;
    }
    return n;
}

int main()
{
    int mode;
    cout << "Select game mode." << endl;
    cout << "\033[0;33m";
    cout << "  0: no computer" << endl;
    cout << "  1: silly computer, full game" << endl;
    cout << "  2: smart computer, full game" << endl;
    cout << "  3: silly computer, partial game (only support accumulate, single/double shot, small/medium defense)" << endl;
    cout << "  4: smart computer, partial game (only support accumulate, single/double shot, small/medium defense)" << endl;
    cout << "\033[0m";
    while (true)
    {
        cin >> mode;
        if (mode >= 0 && mode <= 4)
            break;
        else
            cout << "Illegal input. Please input again." << endl;
    }

    GameMode game_mode = (GameMode)mode;
    Bean game(game_mode);
    switch (game_mode)
    {
        case no_computer:
        {
            int num_p;
            int num_t;
            int team_id;
            cout << "Enter the number of players:" << endl;
            num_p = InputPositiveInteger();
            cout << "Enter the number of teams:" << endl;
            num_t = InputPositiveInteger();

            for (int i = 0; i < num_t; i++)
            {
                stringstream s;
                s << "Team_";
                s << i + 1;
                game.AddTeam(s.str());
            }

            cout << "Enter the team_id of each player in a line with a blank between each entry." << endl;
            for (int i = 0; i < num_p; i++)
            {
                while (true)
                {
                    cin >> team_id;
                    if (team_id > 0 && team_id <= num_t)
                        break;
                    else
                        cout << "Illegal input. Please input again." << endl;
                }
                stringstream s;
                s << "Player_";
                s << i + 1;
                game.AddPlayer(s.str(), team_id - 1);
            }
            break;
        }
        case full_easy: case full_hard: case partial_easy: case partial_hard:
        {
            int num_p;
            int num_c;
            cout << "Enter the number of human players:" << endl;
            num_p = InputPositiveInteger();
            cout << "Enter the number of computers:" << endl;
            num_c = InputPositiveInteger();

            game.AddTeam("Player");
            game.AddTeam("Computer");

            for (int i = 0; i < num_p; i++)
            {
                stringstream s;
                s << "Player_";
                s << i + 1;
                game.AddPlayer(s.str(), 0, human);
            }

            for (int i = 0; i < num_c; i++)
            {
                stringstream s;
                s << "Computer_";
                s << i + 1;
                game.AddPlayer(s.str(), 1, computer);
            }
        }
        default:
            break;
    }
    
    game.Start();
    return 0;
}