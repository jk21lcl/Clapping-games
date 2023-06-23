#include "accumulating_beans.h"
#include <sstream>

int main()
{
    Bean game;
    int num_p;
    int num_t;
    int team_id;
    cout << "Enter the number of players:" << endl;
    cin >> num_p;
    cout << "Enter the number of teams:" << endl;
    cin >> num_t;

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
        cin >> team_id;
        stringstream s;
        s << "Player_";
        s << i + 1;
        game.AddPlayer(s.str(), team_id - 1);
    }

    game.Start();
    return 0;
}