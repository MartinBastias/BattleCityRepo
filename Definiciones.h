#ifndef DEFINICIONES_H
#define DEFINICIONES_H

//Librerias
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Tamaño Mapa
#define FILAS 13
#define COLUMNAS 13

//Tanque 1
extern int tx1, ty1;
extern int vidas1;
extern int dir1X, dir1Y;
extern int spawnX1, spawnY1;

//Tanque 2
extern int tx2, ty2;
extern int vidas2;
extern int dir2X, dir2Y;
extern int spawnX2, spawnY2;

//Hora
extern time_t inicio;

//Mejoras
extern int corazonX, corazonY;
extern time_t spawnCorazon;
extern time_t ultimoCorazon;
extern time_t ultimaBomba;

extern int bombaX, bombaY;
extern time_t tiempoBomba;

extern int corazon_get;
extern int disparando;

//Funciones laberinto
int** crearLaberinto(int filas, int columnas);
void generarLaberinto(int** lab, int filas, int columnas);
void colocarTanques(int** lab, int filas, int columnas, int* x1, int* y1, int* x2, int* y2);
void guardarArchivo(int** lab, int filas, int columnas, const char* nombre);
void leerArchivo(int** lab, int filas, int columnas, const char* nombre);
void mostrarLaberinto(int** lab, int filas, int columnas);
void liberarLaberinto(int** lab, int filas);

//Funciones juego
void mostrarMapa(int** lab);
void finDelJuego();
void golpearTanque(int quien);
void moverTanque(int** lab, int *x, int *y, int dx, int dy, int *dirX, int *dirY, int vivo);
void disparar(int** lab, int x, int y, int dirX, int dirY);

//Mejoras
void aparecerCorazon(int** lab);
void actualizarCorazon(int** lab);
void explotarBomba(int** lab);

#endif