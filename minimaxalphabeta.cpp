#include <climits>
#include "minimaxalphabeta.h"
#include <iostream>
#include <cstdlib>
#include <chrono> //track cpu search time
using namespace std;
using namespace std::chrono;

namespace AI {

   AlphaBeta::some_struct AlphaBeta::alphaBetaSearch(
      Connect4Game gameState, int playerType, int maxDepth) {

      some_struct best;
      best.value = maxValue(gameState, playerType, INT_MIN, INT_MAX);

      //return action in _ACTIONS(gameState) with value v
     return best;
  }

   bool AlphaBeta::cutoffTest(Connect4Game gameState){
      if (gameState.getDepthPlayed() <= 0) {
        return true;
      }
      return false;
   }

   int AlphaBeta::maxValue(
      Connect4Game gameState, int playerType, int alpha, int beta){

        vector<int> playableCols = actions(gameState);
        cout << "In max value with current depth of "<< gameState.getDepthPlayed() <<
            " playerType="<< playerType << ". Doing cutoff test." <<endl;
        // check if deep enough
        if ( cutoffTest(gameState) ) {
            int minEval = INT_MAX;
            int bestCol;
            int tempEval;
            //get max of playableCols
            cout << " ..In max with player type of " << playerType <<
               " and depth of " << gameState.getDepthPlayed() <<
               ". utilities of: ";
            for (int i = 0; i < playableCols.size(); i++) {
               if(evalType == 1) {
                  tempEval = gameState.evalBAlphaBeta(playerType,playableCols.at(i));
               } else /* evalType is -1 */{
                  tempEval = gameState.evalCAlphaBeta(playerType,playableCols.at(i));
               }
                nodesGenerated.push_back(1); // keep track of node generated
                cout << tempEval <<" ";
                if (tempEval < minEval) {
                    minEval = tempEval;
                    bestCol = playableCols.at(i);
                }

            }
            //bestPath.push_back(bestCol); //save best col
            cout << "..In max with player type of " << playerType <<
            " returning minEval=" << minEval <<  endl;
            return minEval; //return max eval
        }

      int v = INT_MIN; // -inf

      // must decrement current depth before next set of recursive calls
      gameState.setDepthPlayed(gameState.getDepthPlayed() - 1 );
      //switch player over for calcuations
      //for each possible column, get the highest of min's choices
      //this->evalType *= -1; //uncomment to run alpha vs alpha
      cout << "In max with playerType=" << playerType << " doing action loop." <<
      "depth is " << gameState.getDepthPlayed() << endl;
      for (int i = 0; i < playableCols.size(); i++) {
         cout << ". In max. Calculating if " << playerType << " played col " <<
         playableCols.at(i) << " depth is " << gameState.getDepthPlayed() << endl;
         int m = minValue(result(gameState, playerType, playableCols.at(i)), -1*playerType, alpha, beta); //intermediate calculation
         cout << ". In max. minValue returned m=" << m  << " depth is " << gameState.getDepthPlayed() << endl;
         // choose max of v or m
         // if m is higher, that means playableCols(i) is the max of min so far
         if (v < m) {
            v = m;
            cout << "In max at depth " << gameState.getDepthPlayed() << " pushed back new max col=" << playableCols.at(i) << endl;
            bestPath.push_back(playableCols.at(i));
         }

         //beta is a cutoff value
         if (v >= beta) {
            cout << ". In max. Pruned by beta=" << beta << " depth is " << gameState.getDepthPlayed() << " returning v=" << v << endl;
            return v;
         }
         alpha = (alpha<v) ? v : alpha; //alpha is max of v or itself
         nodesGenerated.push_back(1); // keep track of nodesGenerated
      }
      cout << "Last line in max. returning v=" << v << endl;
      return v;
   }

   int AlphaBeta::minValue(
      Connect4Game gameState, int playerType, int alpha, int beta) {

        vector<int> playableCols = actions(gameState);
        cout << "In min value with current depth of "<< gameState.getDepthPlayed() <<
        " playerType="<< playerType << ". Doing cutoff test." <<endl;
        if ( cutoffTest(gameState) ) {
            int maxEval = INT_MIN;
            int bestCol;
            int tempEval;
            //get max of playableCols
            cout << " ..In min with player type of " << playerType <<
               " and depth of " << gameState.getDepthPlayed() <<
               ". utilities of: ";
            for (int i = 0; i < playableCols.size(); i++) {
               if(this->evalType == 1) {
                  tempEval = gameState.evalBAlphaBeta(playerType,playableCols.at(i));
               } else /* evalType is -1 */{
                  tempEval = gameState.evalCAlphaBeta(playerType,playableCols.at(i));
               }
                nodesGenerated.push_back(1); // keep track of node generated
                cout << tempEval <<" ";
                if (maxEval < tempEval) {
                    maxEval = tempEval;
                    bestCol = playableCols.at(i);
                }
            }
            cout << endl;
            bestPath.push_back(bestCol); //save best col
            cout << "..In min with player type of " << playerType <<
            " returning maxEval=" << maxEval <<  endl;
            return maxEval; //return max eval
        }

      int v = INT_MAX; //+inf

      // must decrement current depth before next set of recursive calls
      gameState.setDepthPlayed( gameState.getDepthPlayed() - 1);
      //for each possible column, get the min of max's values
      //this->evalType *= -1; //uncomment to run alpha vs alpha
      cout << "In min with playerType=" << playerType << " doing action loop." <<
      "depth is " << gameState.getDepthPlayed() << endl;
      for (int i = 0; i < playableCols.size(); i++) {
         cout << ". In min. Calculating if " << playerType << " played col " <<
         playableCols.at(i) << " depth is " << gameState.getDepthPlayed() << endl;
         int m = maxValue(result(gameState, playerType, playableCols.at(i)), -1*playerType, alpha, beta); //intermediate calculation
        cout << ". In min. maxValue returned m=" << m  << " depth is " << gameState.getDepthPlayed() << endl;

          // choose min of v or m
         if (m < v){
            v = m;
            //cout << "In min at depth " << gameState.getDepthPlayed() << " pushed back new min col=" << playableCols.at(i) << endl;
            //bestPath.back() = playableCols.at(i);
         }

         if (v <= alpha) {
            cout << ". In min. Pruned by alpha=" << alpha << " depth is " << gameState.getDepthPlayed() <<  " returning v=" << v << endl;
           return v;
         }

         beta = !(beta<v)?v:beta; //beta is min of v or itself

         nodesGenerated.push_back(1); // keep track of nodesGenerated
      }
      cout << "Last line in min. returning v=" << v << endl;
      return v;
   }

   /**
   * result(s,a) is transition model that defines result of doing action a
   * in a state s. It returns a successor state.
   * Eg Result(In(Arad),Go(Zerind)) = In(Zerind)
   */
   Connect4Game AlphaBeta::result(
      Connect4Game gameState, int playerType, int col) {
         gameState.dropPiece(playerType, col);
         gameState.printBoard();
         return gameState;
   }

   /**
   * Actions(s) returns vector set of legal moves in a state s.
   * Eg Actions(In(Arad)) = {Go(Sibiu),Go(Timisora),Go(Zerind)}
   */
   vector<int> AlphaBeta::actions(Connect4Game gameState) {
      vector<int> playableCols;
      playableCols = gameState.getOpenColumns();
      return playableCols;
   }

  /**
   * clears internal game board
   **/
   void AlphaBeta::clearGameBoard() {
    this->gameState.clearBoard();
   }

   /**
   * prints internal game board
   **/
   void AlphaBeta::printGameBoard() {
       this->gameState.printBoard();

   }

  /**
   * Plays the game based on computer working as max player
   */
   void AlphaBeta::humanVsComputerAlphaBeta(int maxDepth, int playerType){
        some_struct s;
        srand(0);
        cout << "Playing Human VS CPU with search depth=" << maxDepth << endl;
        this->gameState.setDepthPlayed(maxDepth);
        this->evalType = 1;
        cout << "Computer is " << playerType << endl << endl;
        int humanChoice;
        int maxRounds = 21;
        // Game statistic variables
        int plyCount = 0; //increment after each player's
        int executionTime;
        for (int i = 0; i < maxRounds; i++) {
            cout<< "********************** ROUND " << i << endl;
            auto start = high_resolution_clock::now(); //start
            s = alphaBetaSearch(this->gameState, playerType, maxDepth);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            executionTime += duration.count();
            cout << "********** Computer will play  " << bestPath.back() << endl;
            this->gameState.dropPiece(playerType, bestPath.back());
            plyCount++;
            printGameBoard();

            if (this->gameState.checkWin(this->gameState)) {
                cout << "Computer won" << endl;
                break;
            }
            playerType *= -1;
            cout << "********** Human making move" << endl;
            //this->gameState.dropPiece(playerType, i%7);
            cin >> humanChoice; this->gameState.dropPiece(playerType,humanChoice);
            plyCount++;
            printGameBoard();
            if (this->gameState.checkWin(this->gameState)) {
                cout << "Human won" << endl;
                break;
            }
            playerType *= -1;

        }
        cout << "Game length (ply count) = " << plyCount << endl;
        cout << "Nodes expanded = " << nodesGenerated.size() << endl;
        int memorySize = nodesGenerated.size();
        cout << "Memory usage = " << memorySize << endl;
        cout << "Total CPU search time in microseconds = " << executionTime << endl;
   }
} // end namespace AI
