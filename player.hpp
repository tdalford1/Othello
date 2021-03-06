#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    vector<Move> possibleMoves(Side player, Board * currBoard);
   	int minimax(int depth, Side player, Board* board);
	int minimax2(int depth, Side player, Board *board);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side side;
    Side oppSide;
    Board* board;
};

#endif
