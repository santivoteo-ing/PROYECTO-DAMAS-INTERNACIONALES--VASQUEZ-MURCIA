#ifndef TABLERO_H
#define TABLERO_H

int NONE;
int J1;
int J2;

void inicializar_tablero();
void imprimir_tablero();
int dentro(int r, int c);
int es_oscura(int r, int c);
int propietario_celda(int r, int c);
int es_dama(int r, int c);
int contar_piezas(int jugador);
int jugador_tiene_movimientos(int jugador);
int hay_captura_para_jugador(int jugador);
int pieza_tiene_captura(int r, int c);
int ejecutar_secuencia(int sr, int sc, int path_r[], int path_c[], int path_len);
void coronar_si_necesario(int r, int c);

#endif
