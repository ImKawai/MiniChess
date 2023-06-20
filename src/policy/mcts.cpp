#include <vector>
#include <set>
#include <cmath>
#include <iostream>

#include "../state/state.hpp"
#include "./mcts.hpp"
#define printf(...) ;

struct Node
{
    Node(State* state, Move move) : state(state), move(move) {}

    State* state;
    int wi = 0, ni = 0;
    int step;
    Node* parent = nullptr;
    // std::vector<Node*> children;
    std::set<Node*> children;
    Move move;
};

static Node* root;

static Node* selectFromRoot();
static Node* expansion(Node* node);
static void rollout(Node* node);
static void backPropagation(Node* node, bool win);

inline double UCB(Node* node)
{
    if (!node->ni)
        return 2e9;
    return (double)node->wi/node->ni + 1.414214*sqrt(log(root->ni) / node->ni);
}

Node* selectFromRoot()
{
    Node* node = root;

    Node* bestChild;
    double ucb;
    while (!node->children.empty())
    {
        double maxUCB = -1;
        for (Node* child : node->children)
            if ((ucb = UCB(child)) > maxUCB)
                { bestChild = child;  maxUCB = ucb; }
        node = bestChild;
    }

    return node;
}

Node* expansion(Node* node)
{
    if (node->state->game_state == WIN)
    {
        printf(node->state->player ^ root->state->player ? "lose\n" : "win\n");
        if (node->state->player ^ root->state->player)  // different
            backPropagation(node, false);
        else
            backPropagation(node, true);
        if (node->step > 1)
            node->parent->children.erase(node);
        return nullptr;
    }
    if (node->state->game_state == DRAW)
    {
        printf("lose\n");
        backPropagation(node, false);
        if (node->step > 1)
           node->parent->children.erase(node);
        return nullptr;
    }
    if (node->state->legal_actions.empty())
    {
        printf(node->state->player ^ root->state->player ? "win\n" : "lose\n");
        if (node->state->player ^ root->state->player)  // different
            backPropagation(node, true);
        else
            backPropagation(node, false);
        if (node->step > 1)
           node->parent->children.erase(node);
        return nullptr;
    }
    if (node->ni>1 && node->children.empty())
    {
        backPropagation(node, node->wi>node->ni-node->wi);
        if (node->step > 1)
           node->parent->children.erase(node);
        return nullptr;
    }
    printf("%d\n", node->step);

    Node* n;
    for (const Move& move : node->state->legal_actions)
    {
        n = new Node(node->state->next_state(move), move);  // delete?
        n->parent = node;
        // node->children.emplace_back(n);
        node->children.insert(n);
        n->step = node->step+1;
    }
    // return node->children[0];
    return *node->children.begin();
}

void rollout(Node* node)
{
    State* s = node->state;
    // int round = (node->step > 10 ? node->step : 10);
    // int round = 50;
    // printf("%d\n", round);

    while (s->game_state!=WIN  && s->game_state!=DRAW  && !s->legal_actions.empty())
        s = s->next_state(s->legal_actions[rand() % s->legal_actions.size()]);  // delete?

    if (s->game_state == WIN)
    {
        if (s->player ^ root->state->player)  // different
            backPropagation(node, false);
        else
            backPropagation(node, true);
    }
    else if (s->game_state == DRAW)
    {
        backPropagation(node, false);
    }
    else if (s->legal_actions.empty())
    {
        if (s->player ^ root->state->player)  // different
            backPropagation(node, true);
        else
            backPropagation(node, false);
    }
    else
    {
        if ((s->evaluate()>0) ^ root->state->player)
            backPropagation(node, true);
        else
            backPropagation(node, false);
    }
}

void backPropagation(Node* node, bool win)
{
    while (node != nullptr)
    {
        node->wi += win;
        node->ni ++;;
        node = node->parent;
    }
}

/**
 * @brief get a legal action
 *
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move
 */
Move MCTS::get_move(State *state, int depth){
    if(!state->legal_actions.size())
        state->get_legal_actions();


    int simulation = 100000;
    root = new Node(state, {{0,0}, {0,0}});
    root->step = 0;
    Node* node = expansion(root);
    if (node)
        rollout(node);

    while (simulation--)
    {
        node = selectFromRoot();
        if (!node->ni)
            rollout(node);
        else
        {
            node = expansion(node);
            if (node)
                rollout(node);
        }
        // printf("%d\n", simulation);
    }

    // double maxUCB = -1, ucb;
    // int index = 0;
    // for (int i=0; i<root->children.size(); i++)
    //     if ((ucb = UCB(root->children[i]) > maxUCB))
    //         { index = i;  maxUCB = ucb; }
    Move move;
    double maxN = -1;
    for (Node* node : root->children)
    {
        if (!node->ni) continue;
        if ((double)node->wi/node->ni > maxN)
            { move = node->move;  maxN = (double)node->wi/node->ni; }
    }

    // return state->legal_actions[index];
    return move;
}