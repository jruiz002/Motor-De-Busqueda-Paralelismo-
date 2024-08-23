
#include <stdio.h>
#include <stdlib.h>
#include "reader.h"

#define MAX_LINE_LENGTH 2048
#define MAX_TITLE_LENGTH 500
#define MAX_DATE_LENGTH 40


int main() {
    int movieCount;
    int numThreads = 0;
    int choice;

    do {
        printf("Menú:\n");
        printf("1. Buscar películas\n");
        printf("2. Salir de la aplicación\n");
        printf("Seleccione una opción: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char search_param[50];
                char search_value[100];
                printf("Ingrese el parámetro de búsqueda (title, vote_average, release_date, revenue): ");
                scanf("%s", search_param);
                printf("Ingrese el valor de búsqueda: ");
                scanf("%s", search_value);
                printf("Ingrese el número de hilos: ");
                scanf("%d", &numThreads);

                searchMovies("movies.csv", search_param, search_value, numThreads);
                break;
            }
            case 2:
                printf("Saliendo de la aplicación...\n");
                break;
            default:
                printf("Opción inválida. Por favor, intente de nuevo.\n");
                break;
        }
        printf("\n");
    } while (choice != 2);

    return 0;
}
