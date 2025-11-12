#include <stdio.h>
#include "juego.h"

int main() {
    while (1) {
        printf("\n--- DAMAS INTERNACIONALES - MINIMO ---\n");
        printf("1) Iniciar partida\n");
        printf("2) Ver reglas\n");
        printf("3) Salir\n");
        printf("Opcion: ");
        int opt = 0;
        if (scanf("%d", &opt) != 1) return 0;
        int c = getchar();
        if (opt == 1) {
            iniciar_juego();
        } else if (opt == 2) {
            mostrar_reglas();
        } else if (opt == 3) {
            return 0;
        } else {
            printf("Opcion invalida\n");
        }
    }
    return 0;
}
