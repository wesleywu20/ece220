#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
    netID: wwu70
    This program implements three functions to solve a maze given from a text file:
    createMaze:
        creates and fills a maze structure from the given file
    destroyMaze: 
        frees all memory associated with the maze structure, 
        including the maze structure itself
    printMaze:
        prints out the maze in a human readable format
        using the cells array from the maze structure
    solveMazeDFS: 
        recursively solves the maze using depth first search
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    /* allocate memory for maze_t structure, 
    then read in width and height from inputted maze file, 
    and store the width and height into the maze_t structure */
    FILE *maze_file;
    maze_t *maze = (maze_t *)malloc(sizeof(maze_t));
    maze_file = fopen(fileName, "r");
    fscanf(maze_file, "%d %d\n", &(maze->width), &(maze->height));
    /* allocate memory for row pointers */
    maze->cells = (char **)malloc((maze->height) * sizeof(char **));
    int r, c;
    for (r = 0; r < maze->height; r++) {
        /* allocate memory for column pointers within each row */
        maze->cells[r] = (char *)malloc((maze->width) * sizeof(char *));
        for (c = 0; c < maze->width; c++) {
            /* store the read character into the current (col, row) of the cells matrix in the maze_t structure */
            fscanf(maze_file, "%c", &maze->cells[r][c]);
            /* if current cell is 'S', 
            mark the current (col, row) as the startRow and startColumn in the maze_t structure */
            if (maze->cells[r][c] == START) {
                maze->startRow = r;
                maze->startColumn = c;
            }
            /* if current cell is 'E', 
            mark the current (col, row) as the endRow and endColumn in the maze_t structure  */
            if (maze->cells[r][c] == END) {
                maze->endRow = r;
                maze->endColumn = c;
            }
        }
        /* read the newline after each row and toss it */
        fgetc(maze_file); 
    }
    /* close the maze file
    and return the maze_t structure pointer */
    fclose(maze_file);
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    /* loop through all the rows in the cells and free memory for column pointers */
    int r;
    for (r = 0; r < maze->height; r++) {
        free(maze->cells[r]);
    }
    /* then, free the memory for the row pointers */
    free(maze->cells);
    /* finally, free the memory for the maze_t structure */
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    int r, c;
    for (r = 0; r < maze->height; r++) {
        for (c = 0; c < maze->width; c++) {
            /* loop through all the rows and columns of the maze cells
            and print out the current cell */
            printf("%c", maze->cells[r][c]);
        }
        /* print a newline after each row */
        printf("\n");
    }
}

/*
 * solveMazeDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{ 
    // if (col, row) outside bounds of maze or not an empty cell, return false (i.e. return 0)
    if ((col >= maze->width || row >= maze->height || col < 0 || row < 0) || 
        maze->cells[row][col] == WALL || maze->cells[row][col] == VISITED || maze->cells[row][col] == PATH) {
        return 0;
    }
    // if (col, row) is end of maze, return true
    if (maze->cells[row][col] == END) {
        return 1;
    }
    // if (col, row) is empty, initially mark it as part of the solution path
    if (maze->cells[row][col] == EMPTY) {
        maze->cells[row][col] = PATH;
    }
    // try right of (col, row)
    if (solveMazeDFS(maze, col + 1, row)) {
        return 1;
    }
    // try down of (col, row)
    if (solveMazeDFS(maze, col, row + 1)) {
        return 1;
    }
    // try left of (col, row)
    if (solveMazeDFS(maze, col - 1, row)) {
        return 1;
    } 
    // try up of (col, row)
    if (solveMazeDFS(maze, col, row - 1)) {
        return 1;
    }
    // if none of the cells tested are valid, mark it as visited and return to prior call of solveMazeDFS
    maze->cells[row][col] = VISITED;
    return 0;
}
