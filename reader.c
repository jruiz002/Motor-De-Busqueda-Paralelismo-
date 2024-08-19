#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 2048
#define MAX_TITLE_LENGTH 500
#define MAX_DATE_LENGTH 40

typedef struct {
    char title[MAX_TITLE_LENGTH];
    float vote_average;
    char release_date[MAX_DATE_LENGTH];
    float revenue;
} Movie;

// Función para analizar una línea del CSV
int parseCSVLine(char *line, Movie *movie) {
    char *token;
    int field = 0;

    // Reemplaza las comillas dobles por nada (opcional)
    char temp[MAX_LINE_LENGTH];
    strcpy(temp, line);
    for (char *p = temp; *p; ++p) {
        if (*p == '\"') *p = ' ';
    }

    token = strtok(temp, ",");
    while (token != NULL) {
        // Elimina espacios en blanco de los extremos
        char *start = token;
        while (*start == ' ') start++;
        char *end = start + strlen(start) - 1;
        while (end > start && *end == ' ') end--;
        *(end + 1) = '\0';

        switch (field) {
            case 1:  // "title"
                strncpy(movie->title, start, sizeof(movie->title));
                movie->title[sizeof(movie->title) - 1] = '\0';
                break;
            case 2:  // "vote_average"
                movie->vote_average = strtof(start, NULL);
                break;
            case 5:  // "release_date"
                strncpy(movie->release_date, start, sizeof(movie->release_date));
                movie->release_date[sizeof(movie->release_date) - 1] = '\0';
                break;
            case 6:  // "revenue"
                movie->revenue = strtof(start, NULL);
                break;
            default:
                break;
        }
        token = strtok(NULL, ",");
        field++;
    }

    // Asegurarse de que los campos necesarios se leyeron correctamente
    if (field < 7) {  
        return 0;
    }

    return 1;
}

// Función para leer el archivo CSV y almacenar los datos en un array dinámico
Movie *readMoviesFromCSV(const char *filename, int *movieCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    Movie *movies = NULL;
    int count = 0;
    int capacity = 10;

    movies = (Movie *)malloc(capacity * sizeof(Movie));
    if (!movies) {
        perror("Error al asignar memoria");
        fclose(file);
        return NULL;
    }

    // Leer la primera línea (cabecera) y descartarla
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Error al leer la cabecera");
        free(movies);
        fclose(file);
        return NULL;
    }

    // Leer el resto del archivo línea por línea
    while (fgets(line, sizeof(line), file)) {
        Movie movie;

        if (parseCSVLine(line, &movie)) {
            if (count >= capacity) {
                capacity *= 2;
                movies = (Movie *)realloc(movies, capacity * sizeof(Movie));
                if (!movies) {
                    perror("Error al redimensionar memoria");
                    fclose(file);
                    return NULL;
                }
            }
            movies[count++] = movie;
        } /*else {
            fprintf(stderr, "Advertencia: Línea ignorada (formato incorrecto): %s", line);
        }*/
    }

    fclose(file);

    *movieCount = count;
    return movies;
}

// Función para imprimir las primeras 10 películas
void printMovies(Movie *movies, int count) {
    for (int i = 0; i < 10; i++) {
        printf("Película %d:\n", i + 1);
        printf("Título: %s\n", movies[i].title);
        printf("Calificación: %.2f\n", movies[i].vote_average);
        printf("Fecha de lanzamiento: %s\n", movies[i].release_date);
        printf("Ingresos: %.2f\n", movies[i].revenue);
        printf("\n");
    }
}
