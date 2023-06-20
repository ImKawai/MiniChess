#pragma once
#include "../state/state.hpp"


/**
 * @brief Policy class for mcts policy,
 * your policy class should have get_move method
 */
class MCTS{
public:
  static Move get_move(State *state, int depth);
};