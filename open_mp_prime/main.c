#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#define LIMITE 5000000

int
primo(int numero)
{
    int raiz, fator;
    raiz = (int) sqrt((double) numero);
    for (fator = 2; fator <= raiz; fator++)
        if (numero % fator == 0)
            return 0;
    return 1;
}

int
main(void)
{
    struct timeval tv;
    double start_t, end_t, tempo_gasto;

    int quantidade = 0, numero;

    gettimeofday(&tv, NULL);
    start_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
    for (numero = 2; numero < LIMITE; numero++)
    {
        int p = primo(numero);
        quantidade += p;
    }
    gettimeofday(&tv, NULL);
    end_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Total de numeros primos ate %d: %d\n", LIMITE, quantidade);
    printf("TEMPO GASTO %f\n", tempo_gasto);

}