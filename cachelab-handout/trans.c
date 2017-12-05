//Kolby Nottingham, kolbytn
//Jeremy Teela,

/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans32(int M, int N, int A[N][M], int B[M][N]);
void trans64(int M, int N, int A[N][M], int B[M][N]);
void trans61(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  if(M == 32)
    trans32(M, N, A, B);
  if(M == 64)
    trans64(M, N, A, B);
  if(M == 61)
    trans61(M, N, A, B);
}

void trans32(int M, int N, int A[N][M], int B[M][N]) {
  int i, ii, j, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

  for(ii = 0; ii < 32; ii += 8) {
    for(j = 0; j < 32; j += 8) {
      for(i = ii; i < ii + 8; i++) {
        tmp0 = A[i][j];
        tmp1 = A[i][j + 1];
        tmp2 = A[i][j + 2];
        tmp3 = A[i][j + 3];
        tmp4 = A[i][j + 4];
        tmp5 = A[i][j + 5];
        tmp6 = A[i][j + 6];
        tmp7 = A[i][j + 7];
        B[j][i] = tmp0;
        B[j + 1][i] = tmp1;
        B[j + 2][i] = tmp2;
        B[j + 3][i] = tmp3;
        B[j + 4][i] = tmp4;
        B[j + 5][i] = tmp5;
        B[j + 6][i] = tmp6;
        B[j + 7][i] = tmp7;
      }
    }
  }
}

void trans64(int M, int N, int A[N][M], int B[M][N]) {
  int i, jj, j, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

  for(i = 0; i < 64; i += 4) {
    for(jj = 0; jj < 64; jj += 4) {
      for(j = jj; j < jj + 4; j += 2) {
          tmp0 = A[i][j];
          tmp1 = A[i + 1][j];
          tmp2 = A[i + 2][j];
          tmp3 = A[i + 3][j];
          tmp4 = A[i][j + 1];
          tmp5 = A[i + 1][j + 1];
          tmp6 = A[i + 2][j + 1];
          tmp7 = A[i + 3][j + 1];
          B[j][i] = tmp0;
          B[j][i + 1] = tmp1;
          B[j][i + 2] = tmp2;
          B[j][i + 3] = tmp3;
          B[j + 1][i] = tmp4;
          B[j + 1][i + 1] = tmp5;
          B[j + 1][i + 2] = tmp6;
          B[j + 1][i + 3] = tmp7;
      }
    }
  }
}

void trans61(int M, int N, int A[N][M], int B[M][N]) {

  int i, ii, j, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

  for(ii = 0; ii < 64; ii += 4) {
    for(j = 0; j < 60; j += 4) {
      for(i = ii; i < ii + 4; i++) {
        tmp0 = A[i][j];
        tmp1 = A[i][j + 1];
        tmp2 = A[i][j + 2];
        tmp3 = A[i][j + 3];
        B[j][i] = tmp0;
        B[j + 1][i] = tmp1;
        B[j + 2][i] = tmp2;
        B[j + 3][i] = tmp3;
      }
    }
  }

  j = 60;
  for(i = 0; i < 64; i += 4) {
    tmp0 = A[i][j];
    tmp1 = A[i + 1][j];
    tmp2 = A[i + 2][j];
    tmp3 = A[i + 3][j];
    B[j][i] = tmp0;
    B[j][i + 1] = tmp1;
    B[j][i + 2] = tmp2;
    B[j][i + 3] = tmp3;
  }

  for(i = 64; i < 67; i += 3) {
    for(j = 0; j < 61; j += 3) {
      tmp0 = A[i][j];
      tmp1 = A[i][j + 1];
      tmp2 = A[i][j + 2];
      tmp3 = A[i + 1][j];
      tmp4 = A[i + 1][j + 1];
      tmp5 = A[i + 1][j + 2];
      tmp6 = A[i + 2][j];
      tmp7 = A[i + 2][j + 1];
      tmp8 = A[i + 2][j + 2];
      B[j][i] = tmp0;
      B[j + 1][i] = tmp1;
      B[j + 2][i] = tmp2;
      B[j][i + 1] = tmp3;
      B[j + 1][i + 1] = tmp4;
      B[j + 2][i + 1] = tmp5;
      B[j][i + 2] = tmp6;
      B[j + 1][i + 2] = tmp7;
      B[j + 2][i + 2] = tmp8;      
    }
  }

}


/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
