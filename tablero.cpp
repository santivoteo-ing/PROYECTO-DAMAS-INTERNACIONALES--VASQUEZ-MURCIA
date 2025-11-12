#include "tablero.h"
#include <stdio.h>
#include <stdlib.h>

static char tablero[10][10];

int NONE = 0;
int J1 = 1;
int J2 = 2;

int es_oscura(int r, int c) {
    return ((r + c) % 2) == 1;
}

void inicializar_tablero() {
    int r, c;
    for (r = 0; r < 10; ++r) {
        for (c = 0; c < 10; ++c) {
            if (!es_oscura(r,c)) tablero[r][c] = '.';
            else tablero[r][c] = '_';
        }
    }
    for (r = 0; r < 4; ++r) {
        for (c = 0; c < 10; ++c) {
            if (es_oscura(r,c)) tablero[r][c] = 'o';
        }
    }
    for (r = 6; r < 10; ++r) {
        for (c = 0; c < 10; ++c) {
            if (es_oscura(r,c)) tablero[r][c] = 'x';
        }
    }
}

void imprimir_tablero() {
    int r, c;
    printf("    ");
    for (c = 0; c < 10; ++c) printf("%2d ", c+1);
    printf("\n");
    for (r = 0; r < 10; ++r) {
        printf("%2d  ", r+1);
        for (c = 0; c < 10; ++c) {
            char ch = tablero[r][c];
            if (ch == '_') printf("_  ");
            else if (ch == '.') printf(".  ");
            else printf("%c  ", ch);
        }
        printf("\n");
    }
}

int dentro(int r, int c) {
    return r >= 0 && r < 10 && c >= 0 && c < 10;
}

int propietario_celda(int r, int c) {
    char ch = tablero[r][c];
    if (ch == 'x' || ch == 'X') return J1;
    if (ch == 'o' || ch == 'O') return J2;
    return NONE;
}

int es_dama(int r, int c) {
    char ch = tablero[r][c];
    if (ch == 'X' || ch == 'O') return 1;
    return 0;
}

int contar_piezas(int jugador) {
    int r, c, cnt = 0;
    for (r = 0; r < 10; ++r) for (c = 0; c < 10; ++c) if (propietario_celda(r,c) == jugador) ++cnt;
    return cnt;
}

int man_tiene_captura(int r, int c) {
    int jugador = propietario_celda(r,c);
    if (jugador == NONE) return 0;
    int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    int i;
    for (i = 0; i < 4; ++i) {
        int mr = r + dirs[i][0];
        int mc = c + dirs[i][1];
        int jr = r + 2*dirs[i][0];
        int jc = c + 2*dirs[i][1];
        if (dentro(jr,jc) && propietario_celda(jr,jc) == NONE && propietario_celda(mr,mc) != NONE && propietario_celda(mr,mc) != jugador) return 1;
    }
    return 0;
}

int rey_tiene_captura(int r, int c) {
    int jugador = propietario_celda(r,c);
    if (jugador == NONE) return 0;
    int drs[2] = {-1,1};
    int dcs[2] = {-1,1};
    int di, dj;
    for (di = 0; di < 2; ++di) for (dj = 0; dj < 2; ++dj) {
        int dr = drs[di], dc = dcs[dj];
        int rr = r + dr, cc = c + dc;
        while (dentro(rr,cc)) {
            if (propietario_celda(rr,cc) == NONE) { rr += dr; cc += dc; continue; }
            if (propietario_celda(rr,cc) != jugador) {
                int br = rr + dr, bc = cc + dc;
                if (dentro(br,bc) && propietario_celda(br,bc) == NONE) return 1;
                else break;
            } else break;
        }
    }
    return 0;
}

int pieza_tiene_captura(int r, int c) {
    if (!dentro(r,c)) return 0;
    if (propietario_celda(r,c) == NONE) return 0;
    if (es_dama(r,c)) return rey_tiene_captura(r,c);
    return man_tiene_captura(r,c);
}

int hay_captura_para_jugador(int jugador) {
    int r, c;
    for (r = 0; r < 10; ++r) for (c = 0; c < 10; ++c) if (propietario_celda(r,c) == jugador) if (pieza_tiene_captura(r,c)) return 1;
    return 0;
}

int jugador_tiene_movimientos(int jugador) {
    if (contar_piezas(jugador) == 0) return 0;
    int r, c;
    for (r = 0; r < 10; ++r) for (c = 0; c < 10; ++c) {
        if (propietario_celda(r,c) != jugador) continue;
        if (es_dama(r,c)) {
            int drs[2] = {-1,1}, dcs[2] = {-1,1};
            int di, dj;
            for (di = 0; di < 2; ++di) for (dj = 0; dj < 2; ++dj) {
                int dr = drs[di], dc = dcs[dj];
                int rr = r + dr, cc = c + dc;
                while (dentro(rr,cc)) {
                    if (propietario_celda(rr,cc) == NONE) return 1;
                    if (propietario_celda(rr,cc) != jugador) {
                        int br = rr + dr, bc = cc + dc;
                        if (dentro(br,bc) && propietario_celda(br,bc) == NONE) return 1;
                        else break;
                    } else break;
                    rr += dr; cc += dc;
                }
            }
        } else {
            int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
            int i;
            for (i = 0; i < 4; ++i) {
                int nr = r + dirs[i][0], nc = c + dirs[i][1];
                if (dentro(nr,nc) && propietario_celda(nr,nc) == NONE) {
                    if (jugador == J1 && nr == r-1) return 1;
                    if (jugador == J2 && nr == r+1) return 1;
                }
                int jr = r + 2*dirs[i][0], jc = c + 2*dirs[i][1];
                int mr = r + dirs[i][0], mc = c + dirs[i][1];
                if (dentro(jr,jc) && propietario_celda(jr,jc) == NONE && propietario_celda(mr,mc) != NONE && propietario_celda(mr,mc) != jugador) return 1;
            }
        }
    }
    return 0;
}

int ejecutar_secuencia(int sr, int sc, int path_r[], int path_c[], int path_len) {
    if (!dentro(sr,sc)) return 0;
    if (propietario_celda(sr,sc) == NONE) return 0;
    int jugador = propietario_celda(sr,sc);
    int esrey = es_dama(sr,sc);
    int anyCapture = hay_captura_para_jugador(jugador);
    int cr = sr, cc = sc;
    char mover = tablero[cr][cc];
    tablero[cr][cc] = es_oscura(cr,cc) ? '_' : '.';
    int hizoCaptura = 0;
    int step;
    for (step = 0; step < path_len; ++step) {
        int tr = path_r[step], tc = path_c[step];
        if (!dentro(tr,tc)) { tablero[sr][sc] = mover; return 0; }
        if (tablero[tr][tc] != '_' ) { if (!(tablero[tr][tc] == '.')) { tablero[sr][sc] = mover; return 0; } tablero[sr][sc] = mover; return 0; }
        int dr = tr - cr, dc = tc - cc;
        int adr = dr >= 0 ? dr : -dr, adc = dc >= 0 ? dc : -dc;
        if (!esrey) {
            if (adr == 1 && adc == 1 && !hizoCaptura) {
                if (anyCapture) { tablero[sr][sc] = mover; return 0; }
                if (jugador == J1 && tr != cr-1) { tablero[sr][sc] = mover; return 0; }
                if (jugador == J2 && tr != cr+1) { tablero[sr][sc] = mover; return 0; }
                cr = tr; cc = tc;
            } else if (adr == 2 && adc == 2) {
                int mr = (cr + tr) / 2, mc = (cc + tc) / 2;
                if (propietario_celda(mr,mc) == NONE || propietario_celda(mr,mc) == jugador) { tablero[sr][sc] = mover; return 0; }
                tablero[mr][mc] = es_oscura(mr,mc) ? '_' : '.';
                hizoCaptura = 1;
                cr = tr; cc = tc;
            } else {
                tablero[sr][sc] = mover; return 0;
            }
        } else {
            if (adr != adc || adr == 0) { tablero[sr][sc] = mover; return 0; }
            int sdr = (dr > 0) ? 1 : -1, sdc = (dc > 0) ? 1 : -1;
            int rr = cr + sdr, cc2 = cc + sdc;
            int vioEnemigo = 0, er = -1, ec = -1;
            while (!(rr == tr && cc2 == tc)) {
                if (propietario_celda(rr,cc2) != NONE) {
                    if (propietario_celda(rr,cc2) == jugador) { tablero[sr][sc] = mover; return 0; }
                    if (vioEnemigo) { tablero[sr][sc] = mover; return 0; }
                    vioEnemigo = 1; er = rr; ec = cc2;
                }
                rr += sdr; cc2 += sdc;
                if (!dentro(rr,cc2)) break;
            }
            if (vioEnemigo) {
                tablero[er][ec] = es_oscura(er,ec) ? '_' : '.';
                hizoCaptura = 1;
            } else {
                if (anyCapture) { tablero[sr][sc] = mover; return 0; }
            }
            cr = tr; cc = tc;
        }
    }
    tablero[cr][cc] = mover;
    coronar_si_necesario(cr,cc);
    if (anyCapture && !hizoCaptura) return 0;
    return 1;
}

void coronar_si_necesario(int r, int c) {
    if (!dentro(r,c)) return;
    char ch = tablero[r][c];
    if (ch == 'x' && r == 0) tablero[r][c] = 'X';
    if (ch == 'o' && r == 9) tablero[r][c] = 'O';
}
