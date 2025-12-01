#ifndef VISUALS_H
#define VISUALS_H
#include "Definiciones.h" 
#include <raylib.h>

//struct para agrupar los assets
typedef struct {
    Texture2D tanque1;
    Texture2D tanque1_der;
    Texture2D tanque1_iz;
    Texture2D tanque1_ab;

    Texture2D tanque2;
    Texture2D tanque2_der;
    Texture2D tanque2_iz;
    Texture2D tanque2_ab;

    Texture2D muro_destruible;
    Texture2D muro_indestructible;
    Texture2D bala;
    Texture2D corazon;
    Texture2D bomba;
    
    Music musicaFondo;
    Music musicaEx;
    Sound corazon_get_sound;
    Sound disparo;
    Sound explosion;
    
    int tamCelda;   // 32
    int escala;     // 2
    int offX;       // desplazamiento X para centrar
    int offY;       // desplazamiento Y para centrar
}Assets;

//funciones para las visuales
Assets cargarAssets();
void descargarAssets(Assets r);
void dibujarJuego(int** lab, Assets r);
void dibujarMenuFin(Assets r);

#endif