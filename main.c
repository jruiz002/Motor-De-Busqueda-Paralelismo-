#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "reader.h"

#define MAX_LINE_LENGTH 2048
#define MAX_TITLE_LENGTH 500
#define MAX_DATE_LENGTH 40

int main() {
    int movieCount;
    int numThreads = 0;
    int choice;

    do {
        printf("\nMenú:\n");
        printf("1. Buscar películas por: \n");
        printf("2. Salir de la aplicación \n");
        printf("Seleccione una opción: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int search_choice;
                char search_param[50];
                char search_value[100];

                // Submenú para buscar por diferentes criterios
                printf("\nBuscar películas por:\n");
                printf("1. Título\n");
                printf("2. Votación promedio\n");
                printf("3. Fecha de lanzamiento\n");
                printf("4. Ganancias generadas\n");
                printf("Seleccione una opción: ");
                scanf("%d", &search_choice);

                switch (search_choice) {
                    case 1:
                        strcpy(search_param, "title");
                        break;
                    case 2:
                        strcpy(search_param, "vote_average");
                        break;
                    case 3:
                        strcpy(search_param, "release_date");
                        break;
                    case 4:
                        strcpy(search_param, "revenue");
                        break;
                    default:
                        printf("Opción inválida. Regresando al menú principal...\n");
                        continue;
                }

                printf("Buscar: ");
                scanf("%s", search_value);
                printf("Ingrese el número de hilos: ");
                scanf("%d", &numThreads);
                printf("Resultados de busqueda en el archivo 'busqueda.csv'");

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