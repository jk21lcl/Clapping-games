#include "player.h"

Player::Player(string name)
{
    name_ = name;
    state_ = ingame;
    team_ = 0;
    mode_ = human;
}

Player::Player(string name, int team_id)
{
    name_ = name;
    state_ = ingame;
    team_ = team_id;
    mode_ = human;
}

Player::Player(string name, int team_id, Mode mode)
{
    name_ = name;
    state_ = ingame;
    team_ = team_id;
    mode_ = mode;
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

Mode Player::GetMode() const
{
    return mode_;
}