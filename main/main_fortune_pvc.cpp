#include "fortune.h"
#include <sstream>

int main()
{
    Fortune game;
    int num_p;
    int num_c;
    cout << "Enter the number of players:" << endl;
    cin >> num_p;
    cout << "Enter the number of computers:" << endl;
    cin >> num_c;

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

    game.Start();
    return 0;
}