#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 2048
#define MAX_TITLE_LENGTH 500
#define MAX_DATE_LENGTH 40

typedef struct {
    char title[MAX_TITLE_LENGTH];
    float vote_average;
    char release_date[MAX_DATE_LENGTH];
    float revenue;
} Movie;

Movie *readMoviesFromCSV(const char *filename, int *movieCount);
void printMovies(Movie *movies, int count);


void adjustThreads() {
    // Aquí puedes implementar la lógica para ajustar el número de hilos
    printf("Función para ajustar el número de hilos.\n");
}

int main() {
    int movieCount;
    Movie *movies = readMoviesFromCSV("movies.csv", &movieCount);
    int numThreads = 0;
    printMovies(movies, movieCount);
    if (!movies) {
        return 1; // Salir si no se pudieron cargar las películas
    }

    int choice;
    do {
        printf("Menú:\n");
        printf("1. Buscar películas\n");
        printf("2. Ajustar número de hilos\n");
        printf("3. Salir de la aplicación\n");
        printf("Seleccione una opción: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("hola");
                break;
            case 2:
                if (numThreads == 0){
                    printf("Configura el número de hilos");
                }else{
                    printf("Ingrese el número de hilos");
                    scanf("%d", &numThreads);
                }
                break;
            case 3:
                printf("Saliendo de la aplicación...\n");
                break;
            default:
                printf("Opción inválida. Por favor, intente de nuevo.\n");
                break;
        }
        printf("\n");
    } while (choice != 3);

    free(movies); // Liberar la memoria antes de salir

    return 0;
}
