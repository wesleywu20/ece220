
/*
 * netID: wwu70
 * This code implements three functions for the Game of Life: countLiveNeighbor, updateBoard, and aliveStable
 * countLiveNeighbor:
 *      returns the count of the number of alive neighbors 
 *      (cells that are horizontally, vertically, or diagonally adjacent)
 *      for the inputted (row, col) on the board.
 *      
 * updateBoard:
 *      updates the game board to the next step using the following conditions using the countLiveNeighbor function:
 *          1. A live cell stays alive if it has 2 or 3 alive neighbors,
 *          2. otherwise the cell dies.
 *          3. A dead cell turns alive if it has exactly 3 live neighbors.
 *          
 * aliveStable:
 *      checks if the game board is going to change or not for the next step using the countLiveNeighbor function
 *      on the alive cells, if condition (2) in the updateBoard function is satisfied, it will return 0;
 *      if none of the alive cells change or there are no alive cells at all, it will return 1.
 */

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
    int row_idx, col_idx, num_live = 0;
    /* loop through rows until all three rows of neighbors have been checked or the last row of the board has been reached */
    for(row_idx = row - 1; row_idx < row + 2 && row_idx < boardRowSize; row_idx++) {
        /* loop through rows until all three columns of neighbors have been checked or the last column of the board has been reached */
        for(col_idx = col - 1; col_idx < col + 2 && col_idx < boardColSize; col_idx++) {
            /* if the current coordinate is equal to the inputted row and col or 
            either the row_idx and col_idx are below the first row/first column, skip this iteration */
            if ((row_idx == row && col_idx == col) || (row_idx < 0 || col_idx < 0)) { 
                continue;
            }
            /* if the current neighbor being checked is live, increment count */
            if (board[boardColSize * row_idx + col_idx] == 1) { 
                num_live++;
            } 
        }
    }
    return num_live;
}

/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int row, col, live_neighbor_count; // initialize loop indicies, live neighbor count
    int copy_board[boardRowSize * boardColSize]; // initialize buffer array
    /* copy cells of the original board into buffer array */
    for (row = 0; row < boardRowSize; row++) {
        for (col = 0; col < boardColSize; col++) {
            copy_board[row * boardColSize + col] = board[row * boardColSize + col]; 
        }
    }
    /* update cells of original board using the buffer array, counting the number of live neighbors on each cell in the original board */
    for (row = 0; row < boardRowSize; row++) {
        for (col = 0; col < boardColSize; col++) {
            live_neighbor_count = countLiveNeighbor(board, boardRowSize, boardColSize, row, col); // get live neighbor count of current cell on original board
            /* if current cell on original board doesn't have 2 or 3 live neighbors and is currently live, it dies -> update this value on buffer array */
            if ((live_neighbor_count < 2 || live_neighbor_count > 3) && board[row * boardColSize + col] == 1) { 
                copy_board[row * boardColSize + col] = 0;
            }
            /* if current cell on original board has 3 live neighbors and is currently dead, it turns alive -> update this value on buffer array */
            if (live_neighbor_count == 3 && board[row * boardColSize + col] == 0) { 
                copy_board[row * boardColSize + col] = 1;
            }
        }
    }
    /* copy the buffer array values back into the original array */
    for (row = 0; row < boardRowSize; row++) {
        for (col = 0; col < boardColSize; col++) {
            board[row * boardColSize + col] = copy_board[row * boardColSize + col];
        }
    }
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){
    int row, col, live_neighbor_count;
    for (row = 0; row < boardRowSize; row++) {
        for (col = 0; col < boardColSize; col++) {
            live_neighbor_count = countLiveNeighbor(board, boardRowSize, boardColSize, row, col); // get live neighbor count of current cell
            /* if current cell doesn't have 2 or 3 live neighbors and is currently live, it dies -> an alive cell changes -> return 0 */
            if ((live_neighbor_count < 2 || live_neighbor_count > 3) && board[row * boardColSize + col] == 1) { 
                return 0;
            }
        }
    }
    /* if the end of the loop is reached, then the alive cells for the next step are exactly the same, or there are no alive cells at all */
    return 1;
}