#include <stdio.h>
#include <raylib.h>
#include "Definiciones.h" 
#include "Visuals.h"

int main() {
    srand(time(NULL));

    //Crea la matriz dinamica
    int** laberinto = crearLaberinto(FILAS, COLUMNAS);

    //Generar el contenido
    generarLaberinto(laberinto, FILAS, COLUMNAS);
    
    int x1, y1, x2, y2;
    colocarTanques(laberinto, FILAS, COLUMNAS, &x1, &y1, &x2, &y2);

    guardarArchivo(laberinto, FILAS, COLUMNAS, "laberinto.txt");
    leerArchivo(laberinto, FILAS, COLUMNAS, "laberinto.txt");
    
    // Extraer coordenadas iniciales reales (Importante)
    for(int i=0; i<FILAS; i++) {
        for(int j=0; j<COLUMNAS; j++) {
            if(laberinto[i][j] == 3) { tx1=j; ty1=i; spawnX1=j; spawnY1=i; laberinto[i][j]=0; }
            if(laberinto[i][j] == 4) { tx2=j; ty2=i; spawnX2=j; spawnY2=i; laberinto[i][j]=0; }
        }
    }
    inicio = time(NULL);

    //inicializacion de raylib
    InitWindow(1500, 900, "Battle City");//se abre una ventana de 1500x900
    InitAudioDevice();
    SetTargetFPS(60);

    //se cargan los assets
    Assets misAssets = cargarAssets();

    //gameloop
    while (!WindowShouldClose()) {
        //se va actualizando la musica
        UpdateMusicStream(misAssets.musicaFondo);

        // TANQUE 1
        if (IsKeyPressed(KEY_W)) moverTanque(laberinto, &tx1, &ty1, 0, -1, &dir1X, &dir1Y, vidas1 > 0);
        else if (IsKeyPressed(KEY_S)) moverTanque(laberinto, &tx1, &ty1, 0, 1, &dir1X, &dir1Y, vidas1 > 0);
        else if (IsKeyPressed(KEY_A)) moverTanque(laberinto, &tx1, &ty1, -1, 0, &dir1X, &dir1Y, vidas1 > 0);
        else if (IsKeyPressed(KEY_D)) moverTanque(laberinto, &tx1, &ty1, 1, 0, &dir1X, &dir1Y, vidas1 > 0);
        if (IsKeyPressed(KEY_Q) && vidas1 > 0) disparar(laberinto, tx1, ty1, dir1X, dir1Y);

        // TANQUE 2
        if (IsKeyPressed(KEY_I)) moverTanque(laberinto, &tx2, &ty2, 0, -1, &dir2X, &dir2Y, vidas2 > 0);
        else if (IsKeyPressed(KEY_K)) moverTanque(laberinto, &tx2, &ty2, 0, 1, &dir2X, &dir2Y, vidas2 > 0);
        else if (IsKeyPressed(KEY_J)) moverTanque(laberinto, &tx2, &ty2, -1, 0, &dir2X, &dir2Y, vidas2 > 0);
        else if (IsKeyPressed(KEY_L)) moverTanque(laberinto, &tx2, &ty2, 1, 0, &dir2X, &dir2Y, vidas2 > 0);
        if (IsKeyPressed(KEY_U) && vidas2 > 0) disparar(laberinto, tx2, ty2, dir2X, dir2Y);

        //automatizadores de corazon y bomba
        if (time(NULL) - ultimoCorazon >= 30) { aparecerCorazon(laberinto); ultimoCorazon = time(NULL); }
        actualizarCorazon(laberinto);
        //ahora la bomba aparece y se queda por 3 segundos antes de explotar
        explotarBomba(laberinto);

        //condicionales para que suenen los disparos, corazones, etc
        if(corazon_get==1){
            PlaySound(misAssets.corazon_get_sound);
            corazon_get =0;
        }
        if(disparando==1){
            PlaySound(misAssets.disparo);
            disparando =0;
        }

        //dibujar el mapa con la matriz dinamica y los assets
        dibujarJuego(laberinto, misAssets);
    }

    //limpieza de memria
    descargarAssets(misAssets);
    CloseAudioDevice();
    CloseWindow();
    
    liberarLaberinto(laberinto, FILAS);

    return 0;
}