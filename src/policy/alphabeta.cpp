#include <cstdlib>

#include "../state/state.hpp"
#include "./alphabeta.hpp"


int alphabeta(State* state, int depth, int alpha, int beta)
{
    if (state->game_state == WIN)
        if (!state->player)
            return 2e9;
        else
            return -2e9;
    if (depth==1 || state->game_state==DRAW || state->legal_actions.empty())
        return state->evaluate();

    int nodeValue;
    if (!state->player)  // white
    {
        int value = -2e9;
        for (const Move& move : state->legal_actions)
        {
            nodeValue = alphabeta(state->next_state(move), depth-1, alpha, beta);  // delete?
            if (nodeValue > value)
            {
                value = nodeValue;
                alpha = value;
                if (alpha >= beta) break;
            }
        }
        return value;
    }
    else
    {
        int value = 2e9;
        for (const Move& move : state->legal_actions)
        {
            nodeValue = alphabeta(state->next_state(move), depth-1, alpha, beta);  // delete
            if (nodeValue < value)
            {
                value = nodeValue;
                beta = value;
                if (beta <= alpha) break;
            }
        }
        return value;
    }
}

/**
 * @brief get a legal action
 *
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move
 */
Move AlphaBeta::get_move(State *state, int depth){
    if(!state->legal_actions.size())
        state->get_legal_actions();

    int index = 0;
    int alpha = -2e9, beta = 2e9;
    const auto& actions = state->legal_actions;
    if (!state->player)  // white
    {
        int value = -2e9;
        for (int i=0, nodeValue; i<actions.size(); i++)
        {
            nodeValue = alphabeta(state->next_state(actions[i]), depth, alpha, beta);  // delete?
            if (nodeValue > value)
            {
                value = nodeValue;
                alpha = value;
                index = i;

                // if (alpha >= beta)
                //     break;
            }
        }
    }
    else
    {
        int value = 2e9;
        for (int i=0, nodeValue; i<actions.size(); i++)
        {
            nodeValue = alphabeta(state->next_state(actions[i]), depth, alpha, beta);  // delete?
            if (nodeValue < value)
            {
                value = nodeValue;
                beta = value;
                index = i;

                // if (beta <= alpha)
                //     break;
            }
        }
    }

    return actions[index];
}