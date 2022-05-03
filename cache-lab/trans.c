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

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";

// we got 32 rows and for each row we have 32bytes spaces, which means we can totally contain 4 * 32 int
// each row it can contain 8 int;
// we choose to use 8x8 matrix to scan the original matrix A
void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    int i, j, k, s, a0, a1, a2, a3, a4, a5, a6, a7;
    const int len = 8;
    if (N == 32) {
        for (i = 0; i < N; i += len) {
            for (j = 0; j < N; j += len) {
                for (k = i, s = j; k < i + len; k++, s++) {
                    a0 = A[k][j];
                    a1 = A[k][j + 1];
                    a2 = A[k][j + 2];
                    a3 = A[k][j + 3];
                    a4 = A[k][j + 4];
                    a5 = A[k][j + 5];
                    a6 = A[k][j + 6];
                    a7 = A[k][j + 7];
                    B[s][i] = a0;
                    B[s][i + 1] = a1;
                    B[s][i + 2] = a2;
                    B[s][i + 3] = a3;
                    B[s][i + 4] = a4;
                    B[s][i + 5] = a5;
                    B[s][i + 6] = a6;
                    B[s][i + 7] = a7;
                }
                for (k = 0; k < len; k++) {
                    for (s = k + 1; s < len; s++) {
                        a0 = B[k + j][s + i];
                        B[k + j][s + i] = B[s + j][k + i];
                        B[s + j][k + i] = a0;
                    }
                }
            }
        }
    } else if (N == 64) {
        for (i = 0; i < M; i += len) {
            for (j = 0; j < M; j += len) {
                for (k = 0; k < len / 2; ++k) {
                    a0 = A[k + i][j];
                    a1 = A[k + i][j + 1];
                    a2 = A[k + i][j + 2];
                    a3 = A[k + i][j + 3];

                    a4 = A[k + i][j + 4];
                    a5 = A[k + i][j + 5];
                    a6 = A[k + i][j + 6];
                    a7 = A[k + i][j + 7];

                    B[j][k + i] = a0;
                    B[j + 1][k + i] = a1;
                    B[j + 2][k + i] = a2;
                    B[j + 3][k + i] = a3;
                    B[j][k + i + 4] = a4;
                    B[j + 1][k + i + 4] = a5;
                    B[j + 2][k + i + 4] = a6;
                    B[j + 3][k + i + 4] = a7;
                }

                for (k = 0; k < len / 2; k++) {
                    a0 = A[i + 4][j + k];
                    a1 = A[i + 5][j + k];
                    a2 = A[i + 6][j + k];
                    a3 = A[i + 7][j + k];
                    a4 = A[i + 4][j + k + 4];
                    a5 = A[i + 5][j + k + 4];
                    a6 = A[i + 6][j + k + 4];
                    a7 = A[i + 7][j + k + 4];

                    s = B[j + k][i + 4], B[j + k][i + 4] = a0, a0 = s;
                    s = B[j + k][i + 5], B[j + k][i + 5] = a1, a1 = s;
                    s = B[j + k][i + 6], B[j + k][i + 6] = a2, a2 = s;
                    s = B[j + k][i + 7], B[j + k][i + 7] = a3, a3 = s;
                    // step 4
                    B[j + k + 4][i + 0] = a0, B[j + k + 4][i + 4 + 0] = a4;
                    B[j + k + 4][i + 1] = a1, B[j + k + 4][i + 4 + 1] = a5;
                    B[j + k + 4][i + 2] = a2, B[j + k + 4][i + 4 + 2] = a6;
                    B[j + k + 4][i + 3] = a3, B[j + k + 4][i + 4 + 3] = a7;
                }
            }
        }

    } else {
        for (i = 0; i < N; i+=16) {
            for (j = 0; j < M; j+=16) {
                for (k = i; k < i+16 &&k<N; ++k) {
                    for(s=j; s < j + 16 && s < M;++s){
                        B[s][k] = A[k][s];
                    }
                }
            }
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

void trans(int M, int N, int A[N][M], int B[M][N]) {
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
void registerFunctions() {
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
int is_transpose(int M, int N, int A[N][M], int B[M][N]) {
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

