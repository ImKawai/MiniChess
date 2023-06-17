#include <cstdlib>
#include <algorithm>

#include "../state/state.hpp"
#include "./minimax.hpp"


int minimax(State* state, int depth)
{
    if (state->game_state == WIN)
        if (!state->player)
            return 2e9;
        else
            return -2e9;
    if (depth==1 || state->game_state==DRAW || state->legal_actions.empty())
        return state->evaluate();

    if (!state->player)  // white
    {
        int value = -2e9;
        for (const Move& move : state->legal_actions)
            value = std::max(value, minimax(state->next_state(move), depth-1));  // delete?
        return value;
    }
    else
    {
        int value = 2e9;
        for (const Move& move : state->legal_actions)
            value = std::min(value, minimax(state->next_state(move), depth-1));  // delete?
        return value;
    }
}

/**
 * @brief Randomly get a legal action
 *
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move
 */
Move MiniMax::get_move(State *state, int depth){
    if(!state->legal_actions.size())
        state->get_legal_actions();

    int index = 0;
    const auto& actions = state->legal_actions;
    if (!state->player)  // white
    {
        int value = -2e9;
        for (int i=0, nodeValue; i<actions.size(); i++)
        {
            nodeValue = minimax(state->next_state(actions[i]), depth);  // delete?
            if (nodeValue > value)
            {
                value = nodeValue;
                index = i;
            }
        }
    }
    else
    {
        int value = 2e9;
        for (int i=0, nodeValue; i<actions.size(); i++)
        {
            nodeValue = minimax(state->next_state(actions[i]), depth);  // delete?
            if (nodeValue < value)
            {
                value = nodeValue;
                index = i;
            }
        }
    }

    return actions[index];
}