//para compilar
//gcc -O3 -march=native -fopenmp -std=c11 matriz_omp.c -o matriz_omp


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000
#define REPETICOES 3

static double A[N][N];
static double B[N][N];
static double BT[N][N];
static double C[N][N];

static void inicializar(void) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)((i + j) % 10 + 1) / 10.0;
            B[i][j] = (double)((i * 2 + j) % 10 + 1) / 10.0;
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            BT[j][i] = B[i][j];
}

static void multiplicar(void) {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double soma = 0.0;

            for (int k = 0; k < N; k++)
                soma += A[i][k] * BT[j][k];

            C[i][j] = soma;
        }
    }
}

static double checksum(void) {
    double soma = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            soma += C[i][j];
    return soma;
}

int main(void) {
    inicializar();

    printf("Threads;Execucao;Tempo_s\n");

    for (int threads = 1; threads <= 6; threads++) {
        omp_set_num_threads(threads);
        double total = 0.0;

        for (int r = 0; r < REPETICOES; r++) {
            double inicio = omp_get_wtime();
            multiplicar();
            double fim = omp_get_wtime();

            double tempo = fim - inicio;
            total += tempo;

            printf("%d;%d;%.6f\n",
                   threads, r + 1, tempo);
        }

        printf("# Media threads %d: %.6f s\n",
               threads, total / REPETICOES);
    }

    printf("Checksum;%.10f\n", checksum());

    return 0;
}
