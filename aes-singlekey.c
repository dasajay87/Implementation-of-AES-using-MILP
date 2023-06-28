/*
 * Toolkit for Counting Active S-boxes using
 * Mixed-Integer Linear Programming (MILP).
 *
 * (c) 2012 Nicky Mouha
 *
 * Counting the number of active S-boxes for AES.
 * - Linear and differential cryptanalysis
 * - Non-related-key setting
 * - Any number of rounds (default = 4)
 *
 * The resulting MILP program is in CPLEX lp format.
 */
#include <stdio.h>
int i,j,r;
const int ROUNDS =4; /* number of rounds */
int next = 0; /* next unused state variable index */
int dummy = 0; /* next unused dummy variable index */

void ShiftRows(int a[4][4]) {
  int tmp[4];
  for(i = 1; i < 4; i++) {
    for(j = 0; j < 4; j++) tmp[j] = a[i][(j + i) % 4];
    for(j = 0; j < 4; j++) a[i][j] = tmp[j];
  }
}
void MixColumns(int a[4][4]) 
{
  for(j = 0; j < 4; j++) 
  {
    for (i = 0; i < 4; i++) printf("x%i + ",a[i][j]);
    for (i = 0; i < 3; i++) printf("x%i + ",next+i);
    printf("x%i - 5 d%i >= 0\n",next+3,dummy);

    for(i = 0; i < 4; i++)
      printf("d%i - x%i >= 0\n",dummy,a[i][j]);
    for(i = 0; i < 4; i++)
      printf("d%i - x%i >= 0\n",dummy,a[i][j]=next++);  //changing value of array by 16,17,18,19 coulumn wise
    dummy++;
  }
}
int main() 
{
  int a[4][4]; /* the bytes of the AES state */
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      a[i][j] = next++; /* initialize variable indices */

  printf("Minimize\n"); /* print objective function */
  for (i = 0; i < ROUNDS*16-1; i++) printf("x%i + ",i);
  printf("x%i\n\n",ROUNDS*16-1);

  printf("Subject To\n"); /* round function constraints */
  for (r = 0; r<ROUNDS; r++) { ShiftRows(a); MixColumns(a); }

  /* at least one S-box must be active */
  for (i = 0; i < ROUNDS*16-1; i++) printf("x%i + ",i);
  printf("x%i >= 1\n\n",ROUNDS*16-1);

  printf("Binary\n"); /* binary constraints */
  for (i = 0; i < 16; i++) printf("x%i\n",i);
  for (i = 0; i < dummy; i++) printf("d%i\n",i);
  printf ("End\n");
  return 0;
}
