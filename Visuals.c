#include "Visuals.h"
#include <stdio.h>

//cargar texturas y ventana
Assets cargarAssets() {
    Assets r;
    
    r.tamCelda = 32;
    r.escala = 2;
    
    //cargar las texturas
    r.tanque1 = LoadTexture("assets/spr_tanque1.png");
    r.tanque1_der = LoadTexture("assets/spr_tanque1_derecha.png");
    r.tanque1_iz = LoadTexture("assets/spr_tanque1_izquierda.png");
    r.tanque1_ab = LoadTexture("assets/spr_tanque1_abajo.png");

    r.tanque2 = LoadTexture("assets/spr_tanque2.png");
    r.tanque2_der = LoadTexture("assets/spr_tanque2_derecha.png");
    r.tanque2_iz = LoadTexture("assets/spr_tanque2_iquierda.png");
    r.tanque2_ab = LoadTexture("assets/spr_tanque2_abajo.png");

    r.muro_destruible = LoadTexture("assets/spr_destructible.png");
    r.muro_indestructible = LoadTexture("assets/spr_indestructible.png");
    // r.bala = LoadTexture("assets/bala.png"); 
    
    //cargar audio
    r.musicaFondo = LoadMusicStream("assets/mus_main1.ogg");
    PlayMusicStream(r.musicaFondo);

    r.corazon_get_sound = LoadSound("assets/snd_corazon_get.ogg");
    r.disparo = LoadSound("assets/snd_disparo.ogg");
    
    int anchoMapa = COLUMNAS * r.tamCelda * r.escala;
    int altoMapa = FILAS * r.tamCelda * r.escala;
    
    //se calcula un offset para "centrar" los sprites
    r.offX = (1000 - anchoMapa) / 2;
    r.offY = (900 - altoMapa) / 2;
    
    return r;
}

//funcion para liberar memoria
void descargarAssets(Assets r) {
    UnloadTexture(r.tanque1);
    UnloadTexture(r.tanque1_der);
    UnloadTexture(r.tanque1_iz);
    UnloadTexture(r.tanque1_ab);
    UnloadTexture(r.tanque2);
    UnloadTexture(r.tanque2_der);
    UnloadTexture(r.tanque2_iz);
    UnloadTexture(r.tanque2_ab);
    UnloadTexture(r.muro_destruible);
    UnloadTexture(r.muro_indestructible);
    // UnloadTexture(r.bala)
    
    UnloadMusicStream(r.musicaFondo);
    UnloadSound(r.corazon_get_sound);
    UnloadSound(r.disparo);
}

//funcion para dibujar el frame
void dibujarJuego(int** lab, Assets r) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    //dibujar mapa
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            
            int tipo = lab[i][j];
            
            //posicion de los bloques respecto a la resolucion
            int posX = r.offX + j * r.tamCelda * r.escala;
            int posY = r.offY + i * r.tamCelda * r.escala;
            
            //dibujar fondo negro
            DrawRectangle(posX, posY, r.tamCelda*r.escala, r.tamCelda*r.escala, BLACK);

            //selecciona el sprite a dibujar segun la matriz
            if (tipo == 1) {
                DrawTextureEx(r.muro_destruible, (Vector2){posX, posY}, 0.0f, r.escala, WHITE);
            }
            else if (tipo == 2) {
                DrawTextureEx(r.muro_indestructible, (Vector2){posX, posY}, 0.0f, r.escala, WHITE);
            }
            else if (tipo == 5) {
                DrawCircle(posX + 16*r.escala, posY + 16*r.escala, 4*r.escala, YELLOW);
            }
            else if (tipo == 6) {
                DrawRectangle(posX + 10, posY + 10, 20*r.escala, 20*r.escala, RED);
            }
            else if (tipo == 7) {
                DrawCircle(posX + 16*r.escala, posY + 16*r.escala, 10*r.escala, ORANGE);
            }
        }
    }

    //dibujar los tanques
    //tanque1 -> cambia su sprite dependiendo de la direccion
    if (vidas1 > 0) {
        int x1 = r.offX + tx1 * r.tamCelda * r.escala;
        int y1 = r.offY + ty1 * r.tamCelda * r.escala;
        if(dir1X==1){
            DrawTextureEx(r.tanque1_der, (Vector2){x1, y1}, 0.0f, r.escala, WHITE);
        }
        else if(dir1X ==-1){
            DrawTextureEx(r.tanque1_iz, (Vector2){x1, y1}, 0.0f, r.escala, WHITE);
        }
        else if(dir1Y ==1){
            DrawTextureEx(r.tanque1_ab, (Vector2){x1, y1}, 0.0f, r.escala, WHITE);
        }
        else{
            DrawTextureEx(r.tanque1, (Vector2){x1, y1}, 0.0f, r.escala, WHITE);
        }
    }
    
    //tanque2 -> cambia su sprite dependiendo de la direccion
    if (vidas2 > 0) {
        int x2 = r.offX + tx2 * r.tamCelda * r.escala;
        int y2 = r.offY + ty2 * r.tamCelda * r.escala;
        if(dir2X ==1){
            DrawTextureEx(r.tanque2_der, (Vector2){x2, y2}, 0.0f, r.escala, WHITE);
        }
        else if(dir2X==-1){
            DrawTextureEx(r.tanque2_iz, (Vector2){x2, y2}, 0.0f, r.escala, WHITE);
        }
        else if(dir2Y==1){
            DrawTextureEx(r.tanque2_ab, (Vector2){x2, y2}, 0.0f, r.escala, WHITE);
        }
        else{
            DrawTextureEx(r.tanque2, (Vector2){x2, y2}, 0.0f, r.escala, WHITE);
        }
    }

    //hud con vidas y tiempo
    DrawText(TextFormat("JUGADOR 1: %d Vidas", vidas1), 1050, 250, 30, (Color){154,129,182,255});
    for(int i=1; i<vidas1+1;i++){
        DrawTextureEx(r.tanque1, (Vector2){980+64*i, 300}, 0.0f, r.escala, WHITE);
    }

    DrawText(TextFormat("JUGADOR 2: %d Vidas", vidas2), 1050, 600, 30, (Color){126,169,185,255});
    for(int i=1; i<vidas2+1;i++){
        DrawTextureEx(r.tanque2, (Vector2){980+64*i, 650}, 0.0f, r.escala, WHITE);
    }
    
    int tiempoRestante = 120 - (time(NULL) - inicio);
    if(tiempoRestante < 0) tiempoRestante = 0;
    DrawText(TextFormat("TIEMPO: %d", tiempoRestante), 1050, 150, 40, RAYWHITE);

    EndDrawing();
}