#include "player.h"

Player::Player(string name)
{
    name_ = name;
    state_ = ingame;
}

Player::Player(string name, int team_id)
{
    name_ = name;
    state_ = ingame;
    team_ = team_id;
}

void Player::SetState(State state)
{
    state_ = state;
}

State Player::GetState() const
{
    return state_;
}

string Player::GetName() const
{
    return name_;
}

int Player::GetTeam() const
{
    return team_;
}