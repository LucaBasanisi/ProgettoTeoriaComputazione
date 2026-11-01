#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    // una struct è un contenitore di variabili
    const char *data;
    size_t count; // size_t è un tipo di int che non può essere negativo
} String_View;

// Costruttore di string view
String_View sv_costruttore(const char *data, size_t count) {
    String_View sv;
    sv.data = data;
    sv.count = count;
    return sv;
}

// Convertitore di stringhe in String view
String_View sv_converter(const char *str) {
    return sv_costruttore(str, strlen(str));
}


void swap(int *a, int *b) {
    // * valore (non esadecimale ma tipo int) puntato dalla variabile
    // &x contiene l'indirizzo esadecimale
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

int* crea_array(int n) {
    // Crea dinamicamente un array di interi
    int *array = malloc(n * sizeof(int));

    if (array == NULL)
        return NULL;

    for (int i = 0; i < n; ++i) {
        array[i] = i*2;
    }
    return array;
}

int main(void) {
    printf("Hello, World!\n");

    int x = 5, y = 10;

    printf("Valore di x %d \n", x);
    printf("Valore di x %d \n", y);

    swap(&x, &y);

    printf("Valore di x %d \n", x);
    printf("Valore di x %d \n", y);

    const char stringa[] = "algo\0ritmo";
    printf("La stringa ciao è lunga %d \n", my_strlen(stringa));


    // malloc alloca memoria
    // per sapere quante ne allochiamo: variabile * sizeof(tipo variabile)
    // Esempio  int *arr = malloc(5 * sizeof(int));
    // Poi facciamo free(arr);

    int *array = crea_array(3);
    for (int i = 0; i < 3; i++) {
        printf("%d  ", array[i]);
    }
    printf("\n \n \n \n");

    free(array);
    array = NULL;


    // -----------------------------

    String_View vista;
    vista.data = "Ciaone";
    vista.count = 4;

    printf("Stampo la StringView %s con conteggio %zu \n", vista.data, vista.count);

    // Per stampare n caratteri e fermarsi quando dice il count
    printf("%.*s \n", (int)vista.count, vista.data);

    String_View stringa1 = sv_converter("Prova");
    printf("%.*s \n", (int)stringa1.count, stringa1.data);

    return 0;
}
