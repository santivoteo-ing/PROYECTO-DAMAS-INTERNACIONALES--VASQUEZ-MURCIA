#include "juego.h"
#include "tablero.h"
#include <stdio.h>
#include <string.h>

static int turno;

void mostrar_reglas() {
    printf("\nReglas basicas:\n");
    printf("- Tablero 10x10, piezas en casillas oscuras.\n");
    printf("- Movimiento simple diagonal hacia adelante para peones.\n");
    printf("- Captura obligatoria y capturas multiples.\n");
    printf("- Coronacion al alcanzar la fila opuesta.\n\n");
}

static int chequear_victoria() {
    if (contar_piezas(J1) == 0) { printf("Gana Jugador 2 (O)\n"); return 1; }
    if (contar_piezas(J2) == 0) { printf("Gana Jugador 1 (X)\n"); return 1; }
    if (!jugador_tiene_movimientos(J1)) { printf("Gana Jugador 2 por bloqueo\n"); return 1; }
    if (!jugador_tiene_movimientos(J2)) { printf("Gana Jugador 1 por bloqueo\n"); return 1; }
    return 0;
}

static void cambiar_turno() {
    turno = (turno == J1) ? J2 : J1;
}

void iniciar_juego() {
    inicializar_tablero();
    turno = J1;
    char linea[256];
    while (1) {
        imprimir_tablero();
        printf("Turno de: %s\n", turno == J1 ? "Jugador 1 (X)" : "Jugador 2 (O)");
        if (chequear_victoria()) return;
        printf("Escribe 0 0 0 0 para salir al menu.\n");
        printf("Formato: origen_f origen_c dest1_f dest1_c [dest2_f dest2_c ...]\n");
        printf("Entrada: ");
        if (!fgets(linea, sizeof(linea), stdin)) return;
        int valores[32];
        int vi = 0;
        char *tok = strtok(linea, " \t\n\r");
        while (tok && vi < 32) {
            valores[vi++] = atoi(tok);
            tok = strtok(NULL, " \t\n\r");
        }
        if (vi == 0) continue;
        if (vi == 4 && valores[0] == 0 && valores[1] == 0 && valores[2] == 0 && valores[3] == 0) return;
        if (vi < 4 || (vi % 2) != 0) { printf("Entrada invalida\n"); continue; }
        int sr = valores[0] - 1, sc = valores[1] - 1;
        if (!dentro(sr,sc)) { printf("Origen fuera del tablero\n"); continue; }
        int path_len = (vi - 2) / 2;
        int path_r[16], path_c[16];
        int i;
        int valido = 1;
        for (i = 0; i < path_len; ++i) {
            path_r[i] = valores[2 + 2*i] - 1;
            path_c[i] = valores[2 + 2*i + 1] - 1;
            if (!dentro(path_r[i], path_c[i])) { valido = 0; break; }
        }
        if (!valido) { printf("Destino invalido\n"); continue; }
        if (propietario_celda(sr,sc) != turno) { printf("No puedes mover pieza rival\n"); continue; }
        int ok = ejecutar_secuencia(sr, sc, path_r, path_c, path_len);
        if (!ok) { printf("Movimiento invalido o no permitido\n"); continue; }
        cambiar_turno();
    }
}
