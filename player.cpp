#include "player.hpp"
#include <vector>


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    board = new Board();
    this->side = side;
    if (side == BLACK)
    {
        oppSide = WHITE;
    }
    else
    {
        oppSide = BLACK;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;

}

vector<Move> Player::possibleMoves(Side player, Board* currBoard)
{
    vector<Move> possibles;
    if (currBoard->hasMoves(player)) //check and find valid moves for black
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                Move testMove(i, j);
                if (currBoard->checkMove(&testMove, player) == true)
                {
                    possibles.push_back(testMove);
                }
            }
        }
    }
    else
    {
        return possibles;
    }

    return possibles;
}

int Player::minimax2(int depth, Side player, Board* currBoard){
	if (depth == 0)
    {

        if (testingMinimax)
        {
            cerr << currBoard->evalBoard(2, 15, 6, player) << endl;
            return abs(currBoard->countWhite() - currBoard->countBlack());
        }

        //evaluate board for given player
		int cornWeight = 100;
		int nxtCornWeight = 50;
		int edgeWeight = 5;
        cerr << currBoard->evalBoard(edgeWeight, nxtCornWeight, cornWeight, player) << endl;
        return currBoard->evalBoard(edgeWeight, nxtCornWeight, cornWeight, side);
    }

	if(player == side){
		int bestVal = -1e8;
		vector<Move> possibles = possibleMoves(player, currBoard);
		Move * testMove = new Move(0,0);

		for(unsigned int i=0; i<possibles.size(); i++){
			Board * boardCopy = currBoard->copy();
			*testMove = possibles[i];
			boardCopy->doMove(testMove, player);
			Side oppPlayer;
			if(player == WHITE){oppPlayer = BLACK;}
			else {oppPlayer = WHITE;}
			int score = minimax(depth - 1, oppPlayer, boardCopy);
			bestVal = std::max(bestVal, score);
		}
		return bestVal;
	}
	else{
		int bestVal = 1e8;
		vector<Move> possibles = possibleMoves(player, currBoard);
		Move * testMove = new Move(0,0);

		for(unsigned int i=0; i<possibles.size(); i++){
			Board * boardCopy = currBoard->copy();
			*testMove = possibles[i];
			boardCopy->doMove(testMove, player);
			Side oppPlayer;
			if(player == WHITE) {oppPlayer = BLACK;}
			else {oppPlayer = WHITE;}
			int score = minimax(depth - 1, oppPlayer, boardCopy);
			bestVal = std::min(bestVal, score);
		}
		return bestVal;
	}
}

int Player::minimax(int depth, Side player, Board* currBoard)
{

    Side oppPlayer;
    if (player == BLACK)
    {
        oppPlayer = WHITE;
    }
    else
    {
        oppPlayer = BLACK;
    }
    if (depth == 0)
    {

        if (testingMinimax)
        {
            //cerr << currBoard->evalBoard(2, 20, 4, player) << endl;
            if (side == BLACK)
            {
                return (currBoard->countBlack() - currBoard->countWhite());
            }
            else
            {
                return (currBoard->countWhite() - currBoard->countBlack());
            }
        }

        //evaluate board for given player

        int cornWeight;
        int nxtCornWeight;
        int edgeWeight;

        if (side == BLACK) //change heuristic for given side
        {
    		cornWeight = 50;
    		nxtCornWeight = 60;
    		edgeWeight = 10;
        }
        else
        {
            cornWeight = 100;
            nxtCornWeight = 200;
            edgeWeight = 2;
        }

        //cerr << currBoard->evalBoard(edgeWeight, nxtCornWeight, cornWeight, player) << endl;
        return currBoard->evalBoard(edgeWeight, nxtCornWeight, cornWeight, side);
    }

    Move * testMove = new Move(0,0);
    int bestScore;
    vector<Move> possibles = possibleMoves(player, currBoard);
    if (possibles.size() == 0)
    {
        //have to pass
        int score = minimax(depth - 1, oppPlayer, currBoard);
        if (player == side) //maximise
        {
            bestScore = 1e8;
            if (score < bestScore)
            {
                bestScore = score;
            }
        }
        else //minimise
        {   
            bestScore = -1e8;
            if (score > bestScore)
            {
                bestScore = score;
            }
        }
    }

    for (unsigned int i = 0; i < possibles.size(); i++)
    {
        Board * boardCopy = new Board();
        boardCopy = currBoard->copy();
        *testMove = possibles[i];
        boardCopy->doMove(testMove, player);
        //cerr << "curr test " << testMove->getX() << " " << testMove->getY() << endl;
        int score = minimax(depth - 1, oppPlayer, boardCopy);
        //cerr << "score = " << score << endl;
        if (player == side) //maximise
        {
            bestScore = -1e8;
            if (score > bestScore)
            {
                bestScore = score;
            }
        }
        else //minimise
        {   
            bestScore = 1e8;
            if (score < bestScore)
            {
                bestScore = score;
            }
        }
        delete boardCopy;
    }
    return bestScore;

}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    board->doMove(opponentsMove, oppSide);
    vector<Move> testMoves;
    //vector<Move> badMoves;
    Move * moveToPlay = new Move(0, 0);
    Move * testMove = new Move(0, 0);
    int depth;
    if (testingMinimax)
    {   
        depth = 1;
    }
    else
    {
        depth = 3;
    }

    int bestScore = -1e8;
    vector<Move> possibles = possibleMoves(side, board);
    for (unsigned int i = 0; i < possibles.size(); i++)
    {
        Board* boardCopy = new Board();
        boardCopy = board->copy();
        *testMove = possibles[i];
        boardCopy->doMove(testMove, side);
        //cerr << "current test " << testMove->getX() << " " << testMove->getY() << endl;
        //int score = minimax2(depth, side, boardCopy);
        int score = minimax(depth, oppSide, boardCopy);
        //cerr << "final score = " << score << endl;
        if (score > bestScore)
        {
            bestScore = score;
            *moveToPlay = possibles[i];
        }
        delete boardCopy;
    }


    if (possibles.size() == 0)
    {
        return nullptr;
    }

    board->doMove(moveToPlay, side);
    return moveToPlay;
	
}
