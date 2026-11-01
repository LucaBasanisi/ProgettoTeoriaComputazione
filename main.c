#include <stdio.h>
#include <string.h>

void swap(int *a, int *b) {
    // * valore (non esadecimale ma tipo int) puntato dalla variabile
    // &x contiene l'indirizzo esadecimale

    //
    int temp = *a;
    *a = *b;
    *b = temp;

}

int my_strlen(const char *stringa) {
    int lunghezza=0;
    for (int i = 0; stringa[i] != (char) 0; i++) {
        lunghezza++;
    }
    return lunghezza;
}

int main(void) {
    printf("Hello, World!\n");

    // int x = 5, y = 10;
    //
    // printf("Valore di x %d", x);
    // printf("Valore di x %d", y);
    //
    // swap(&x, &y);
    //
    // printf("Valore di x %d", x);
    // printf("Valore di x %d", y);

    const char stringa[] = "algo\0ritmo";
    printf("La stringa ciao è lunga %d", my_strlen(stringa));

    return 0;
}
