#include <stdio.h>
#include <raylib.h>
#include "Definiciones.h" 
#include "Visuals.h"

int main() {
    srand(time(NULL));
    remove("partida_guardada.txt");

    //Crea la matriz dinamica
    int** laberinto = crearLaberinto(FILAS, COLUMNAS);

    iniciarPartida(laberinto);

    //inicializacion de raylib
    InitWindow(1500, 900, "Battle City");//se abre una ventana de 1500x900
    InitAudioDevice();
    SetTargetFPS(60);

    //se cargan los assets
    Assets assets = cargarAssets();
    PlayMusicStream(assets.musicaFondo);

    //gameloop
    while (!WindowShouldClose()) {

        if(!juegoTerminado){
        if(IsKeyPressed(KEY_SPACE)) guardarPartida(laberinto);
        if(IsKeyPressed(KEY_LEFT_SHIFT)) cargarPartida(laberinto);
        //se va actualizando la musica
        UpdateMusicStream(assets.musicaFondo);
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
        if (time(NULL) - ultimoCorazon >= 18) { aparecerCorazon(laberinto); ultimoCorazon = time(NULL); }
        actualizarCorazon(laberinto);
        //ahora la bomba aparece y se queda por 3 segundos antes de explotar
        explotarBomba(laberinto);

        //condicionales para que suenen los disparos, corazones, etc
        if(corazon_get==1){
            PlaySound(assets.corazon_get_sound);
            corazon_get =0;
        }
        if(explotando==1){
            float pitchRandom = GetRandomValue(97,103)/100.0f;
            SetSoundPitch(assets.explosion, pitchRandom);
            PlaySound(assets.explosion);
            explotando=0;
        }
        if(disparando==1){
            //float pitchRandom = GetRandomValue(97,103)/100.0f;
            //SetSoundPitch(misAssets.disparo, pitchRandom);
            PlaySound(assets.disparo);
            disparando =0;
        }
        int tiempoPasado = time(NULL) -inicio;
        if(tiempoPasado>=120){
            finDelJuego();
        }
        //dibujar el mapa con la matriz dinamica y los assets
        dibujarJuego(laberinto, assets);
        }
        else{
            StopMusicStream(assets.musicaFondo);
            if(!IsMusicStreamPlaying(assets.musicaEx)){
                PlayMusicStream(assets.musicaEx);
            }
            UpdateMusicStream(assets.musicaEx);
            remove("partida_guardada.txt");
            dibujarMenuFin(assets);
            if(IsKeyPressed(KEY_ENTER)){
                StopMusicStream(assets.musicaEx);
                iniciarPartida(laberinto);
                PlayMusicStream(assets.musicaFondo);
            }
        }   
    }

    //limpieza de memria
    descargarAssets(assets);
    CloseAudioDevice();
    CloseWindow();
    
    liberarLaberinto(laberinto, FILAS);

    return 0;
}