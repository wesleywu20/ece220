#include <stdio.h>
#include <stdlib.h>

/*
  This program prints out the coefficients in the nth row, 
  where n is from user input;
  it uses a loop to print out each coefficient,
  first printing out the coefficient, 
  then updating it using the pi notation formula to calculate the next coefficient in the row
  by multiplying it by the next term in the pi notation formula
*/

/*
  This function calculates and prints the Pascal's triangle coefficients for the row inputted
*/
void calc_row(int row) {
  unsigned long coeff = 1;
  for (int count = 0; count < row + 1; count++) {
    printf("%lu ", coeff);
    coeff *= row + 1 - (count + 1); 
    coeff /= count + 1; // applying the formula for calculating the next coefficient, using count + 1 as i
  }
  printf("\n");
}

int main()
{
  int row;

  printf("Enter the row index : ");
  scanf("%d",&row);

  // Write your code here
  calc_row(row);
  return 0;
}

