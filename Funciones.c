#include "Definiciones.h"
#include "Visuals.h"
#include <raylib.h>

//Crea la matriz dinámica
int** crearLaberinto(int filas, int columnas) {
    int** lab = malloc(filas * sizeof(int*));  //Reserva memoria para las filas
    for(int i = 0; i < filas; i++)
        lab[i] = malloc(columnas * sizeof(int));  //Para las columnas 
    return lab;
}


//Genera el laberinto aleatorio
void generarLaberinto(int** lab, int filas, int columnas) {
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            if(i == 0 || i == filas - 1 || j == 0 || j == columnas - 1) {
                lab[i][j] = 2;  //Las orillas son paredes indestructibles
            } else {
                int r = rand() % 100;
                if(r < 50)       lab[i][j] = 0;  //50% camnino
                else if(r < 90)  lab[i][j] = 1;  //40% paredes destruibles  
                else             lab[i][j] = 2;  //10% paredesindestructibles
            }
        }
    }
}


//Coloca los tanques
void colocarTanques(int** lab, int filas, int columnas, int* x1, int* y1, int* x2, int* y2) {
    do { *x1 = rand() % filas;  *y1 = rand() % columnas; }  //Da una direccion random del mapa 
    while(lab[*x1][*y1] != 0);                              //Verifica que sea 0 y si no es asi, busca otra 
    lab[*x1][*y1] = 3;                                      //Pone el tanque 1

    do { *x2 = rand() % filas;  *y2 = rand() % columnas; }      //Lo mismo que arriba
    while(lab[*x2][*y2] != 0 || (*x2 == *x1 && *y2 == *y1));    //Pero ademas verifica si esta puesto el tanque 1 y si es asi busca otro lado
    lab[*x2][*y2] = 4;                                          //Pone el tanque 2
}


//Guarda el archivo
void guardarArchivo(int** lab, int filas, int columnas, const char* nombre) {   
    FILE* f = fopen(nombre, "w");
    if (!f) return;

    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++)
            fprintf(f, "%d ", lab[i][j]);
        fprintf(f, "\n");
    }
    fclose(f);
}


//Lee el archivo
void leerArchivo(int** lab, int filas, int columnas, const char* nombre) {
    FILE* f = fopen(nombre, "r");
    if (!f) return;

    for(int i = 0; i < filas; i++)
        for(int j = 0; j < columnas; j++)
            fscanf(f, "%d", &lab[i][j]);

    fclose(f);
}


//Muestra el laberinto en consola
void mostrarLaberinto(int** lab, int filas, int columnas) {
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++)
            printf("%d ", lab[i][j]);
        printf("\n");
    }
}


//Libera la memoria
void liberarLaberinto(int** lab, int filas) {
    for(int i = 0; i < filas; i++)
        free(lab[i]);
    free(lab);
}


//Muestra el mapa de juego
void mostrarMapa(int** lab) {

    printf("T1 Vidas: %d    T2 Vidas: %d\n", vidas1, vidas2);

    int tiempo = 120 - (time(NULL) - inicio);  //Guarda el tiempo (en segundos) del pc en el que empezo el juego y el actual, y los resta
    if (tiempo < 0) tiempo = 0;
    printf("Tiempo restante: %d segundos\n\n", tiempo);

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {

            if (i == ty1 && j == tx1 && vidas1 > 0)
                printf("3 ");
            else if (i == ty2 && j == tx2 && vidas2 > 0)
                printf("4 ");
            else
                printf("%d ", lab[i][j]);
        }
        printf("\n");
    }
}


//Define el resultado del juego
void finDelJuego() {

    printf("\n ||[FIN DEL JUEGO]||  \n\n");
    printf("Vidas T1: %d\n", vidas1);
    printf("Vidas T2: %d\n", vidas2);

    if (vidas1 > vidas2) {
        printf("\nGANADOR: Tanque 1\n");
    }
    else if (vidas2 > vidas1) {
        printf("\nGANADOR: Tanque 2\n");
    }
    else {                    
        printf("\nEMPATE\n");
    }

    exit(0);  //Termina el juego y deja mostrando los resultados finales
}


//Cuando una bala golpea algun tanque
void golpearTanque(int quien) {
    if (quien == 1) {   //Tanque 1
        vidas1--;
        if (vidas1 <= 0) {
            finDelJuego();
        }
        //Respawnea en la posición inicial
        tx1 = spawnX1;
        ty1 = spawnY1;
    }
    else {  //Tanque 2
        vidas2--;
        if (vidas2 <= 0) {
            finDelJuego();
        }
        //Respawnea en la posición inicial
        tx2 = spawnX2;
        ty2 = spawnY2;
    }
}


//Movimiento de los tanques
void moverTanque(int** lab, int *x, int *y, int dx, int dy, int *dirX, int *dirY, int vivo) {  //Se pasan como punteros algunos datos por que las modificaciones que se hagan con ellos se guardan
    if (!vivo) return;  //Si se murio alguno no sigue con la funcion

    int nx = *x + dx;  //Guarda la posible nueva posicion 
    int ny = *y + dy;  //Guarda la posible nueva posicion 

    if ((nx == tx1 && ny == ty1) || (nx == tx2 && ny == ty2)) {  //Evita que los tanques se superpongan 
        return;
    }
    if (nx >= 0 && nx < COLUMNAS && ny >= 0 && ny < FILAS && lab[ny][nx] == 0||lab[ny][nx] == 6) {  //Verifica que la nueva posicion esta dentro del mapa y si es 0 (solo se puede mover por los 0)
        *x = nx;                                                                   //Si cumple, guarda la nueva posicion
        *y = ny;
    }

    *dirX = dx;  //Actualiza la direccion del tanque (Se usa par cuando dispares, ver para que lado sale la bala)
    *dirY = dy;
}


//Interacciones de la bala
void disparar(int** lab, int x, int y, int dirX, int dirY) {

    int bx = x + dirX;  //Guarda la posicion de la bala delante del tanque
    int by = y + dirY;

    while (bx >= 0 && bx < COLUMNAS && by >= 0 && by < FILAS) {
        disparando = 1;
        if (lab[by][bx] == 1) {lab[by][bx] = 0; break;}//Si choca con una pared destruible desaparece junto con la pared
        if (lab[by][bx] == 2) {break;}  //Si choca con una pared indestructible solo la bala desaparece 

        if (bx == tx1 && by == ty1 && vidas1 > 0) {//Si choca con el tanque 1
            golpearTanque(1);
            break;
        }

        if (bx == tx2 && by == ty2 && vidas2 > 0) {//Si choca con el tanque 2
            golpearTanque(2);
            break;
        }

        int prev = lab[by][bx];  //Guarda el valor en el que va a estar la bala 
        lab[by][bx] = 5;         //Remplaza el valor por el de la bala     

        lab[by][bx] = prev;      //Restaura el valor en el que estaba la bala 

        bx += dirX;  //Suma la direccion en la que va la bala, por ende esta avanza
        by += dirY; 
    }
}


//Espawnea el corazon 
void aparecerCorazon(int** lab) {
    if (corazonX != -1) { //Si no hay ningun corzaon en el mapa no sigue con la funcion 
    return;
    }

    int x, y;
    do {
        x = rand() % COLUMNAS;
        y = rand() % FILAS;     //Busca algun espacio random 
    } while (lab[y][x] != 0);   //Lo repite hasta que encuentre un 0

    corazonX = x;  //Se guardan las posiciones 
    corazonY = y;
    lab[y][x] = 6;  //El corazon es 6

    spawnCorazon = time(NULL);  //Guarda el momento en el que se puso el corazon 
}


//Actualiza el corazon
void actualizarCorazon(int** lab) {
    if (corazonX == -1) {    //Si no hay corazon no actualiza nada
     return;
    }

    if (time(NULL) - spawnCorazon >= 5) {  //Si pasan los 5 segundos el corazon desaparece 
        lab[corazonY][corazonX] = 0;        //Vuelve la posicion a 0
        corazonX = corazonY = -1;           //Desaparece el corazon
        return;                             //Ya no sigue con la funcion 
    }

    //Si el tanque 1 lo pisa
    if (tx1 == corazonX && ty1 == corazonY && vidas1 > 0) {
        vidas1++;
        corazon_get = 1; //Marca que se tomo un corazon para reproducir sonido
        if (vidas1 > 3) {     //Las vidas no pueden ser mayores a 3
        vidas1 = 3;
        }
        lab[corazonY][corazonX] = 0;  //Vuelve la posicion a 0 si se lo come un tanque 
        corazonX = corazonY = -1;  //Elimina el corazon
    }

    //Si el tanque 2 lo pisa
    if (tx2 == corazonX && ty2 == corazonY && vidas2 > 0) {     //Lo mismo que arriba pero con el tanque 2
        vidas2++;
        corazon_get = 1;
        if (vidas2 > 3) {
            vidas2 = 3;
        }
        lab[corazonY][corazonX] = 0;
        corazonX = corazonY = -1;
    }
}


//Que hace la bomba
void explotarBomba(int** lab) {
    static int bombaActiva = 0; 
    static double tiempoInicio = 0;
    static int x = 0, y = 0;

    //(Si no hay bomba activa)
    if (bombaActiva == 0) {
        do {
            x = rand() % COLUMNAS;
            y = rand() % FILAS;
        } while (lab[y][x] != 0);

        lab[y][x] = 7;              //La bomba es 7
        tiempoInicio = GetTime();   //Guardamos el tiempo de Raylib
        bombaActiva = 1;            //Marcamos que hay una bomba esperando
    }
    //(Si hay bomba activa)
    else {
        if (GetTime() - tiempoInicio >= 3.0) {
            for (int dy = -1; dy <= 1; dy++) {      
                for (int dx = -1; dx <= 1; dx++) { 

                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < COLUMNAS && ny >= 0 && ny < FILAS) { 

                        if (nx == tx1 && ny == ty1 && vidas1 > 0) {
                            golpearTanque(1);
                        }
                        if (nx == tx2 && ny == ty2 && vidas2 > 0) { 
                            golpearTanque(2);
                        }
                        if (lab[ny][nx] == 1) { 
                            lab[ny][nx] = 0;
                        }
                    }
                }
            }

            lab[y][x] = 0;   // Borramos la bomba
            bombaActiva = 0; // Permitimos que se genere una nueva
        }
    }
}