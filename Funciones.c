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
    juegoTerminado =1;
    if (vidas1 > vidas2) {
        tanqueGanador = 1;
    }
    else if (vidas2 > vidas1) {
        tanqueGanador = 2;
    }
    else {                    
        tanqueGanador = 0;
    } 
}


//Cuando una bala golpea algun tanque
void golpearTanque(int quien) {
    if (quien == 1) {   //Tanque 1
        vidas1--;
        if (vidas1 <= 0) {
            WaitTime(0.5);
            finDelJuego();
        }
        //Respawnea en la posición inicial
        tx1 = spawnX1;
        ty1 = spawnY1;
    }
    else {  //Tanque 2
        vidas2--;
        if (vidas2 <= 0) {
            WaitTime(0.5);
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
            explotando = 1;
            break;
        }

        if (bx == tx2 && by == ty2 && vidas2 > 0) {//Si choca con el tanque 2
            golpearTanque(2);
            explotando = 1;
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
    int x, y;
    //(Si no hay bomba activa)
    if (bombaX == -1) {
        do {
            x = rand() % COLUMNAS;
            y = rand() % FILAS;
        } while (lab[y][x] != 0);

        lab[y][x] = 7;              //La bomba es 7
        tiempoBomba = time(NULL);  //Guardamos el tiempo de Raylib
        bombaX = x;
        bombaY = y;           //Marcamos que hay una bomba esperando
    }
    //(Si hay bomba activa)
    else {
        if (time(NULL)- tiempoBomba >=3) {
            for (int dy = -1; dy <= 1; dy++) {      
                for (int dx = -1; dx <= 1; dx++) { 

                    int nx = bombaX + dx;
                    int ny = bombaY + dy;

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
            lab[bombaY][bombaX] = 0;   // Borramos la bomba
            bombaX=-1;
            bombaY=-1;

            ultimaBomba = time(NULL);
            explotando =1;
        }
    }
}
void iniciarPartida(int** lab) {
    //resetear todas las variables por default
    vidas1 = 3;
    vidas2 = 3;
    
    juegoTerminado = 0;
    tanqueGanador = 0;
    
    corazonX = -1; corazonY = -1;
    bombaX = -1; bombaY = -1;
    explotando = 0;
    corazon_get = 0;
    disparando = 0;
    
    inicio = time(NULL);
    ultimoCorazon = time(NULL)-30;
    ultimaBomba = time(NULL);

    spawnCorazon = 0; 
    tiempoBomba = 0;
    
    //generar el laberinto
    generarLaberinto(lab, FILAS, COLUMNAS);
    
    //colocar tanques
    int x1, y1, x2, y2;
    colocarTanques(lab, FILAS, COLUMNAS, &x1, &y1, &x2, &y2);

    for(int i=0; i<FILAS; i++) {
        for(int j=0; j<COLUMNAS; j++) {
            if(lab[i][j] == 3) { tx1=j; ty1=i; spawnX1=j; spawnY1=i; lab[i][j]=0; }
            if(lab[i][j] == 4) { tx2=j; ty2=i; spawnX2=j; spawnY2=i; lab[i][j]=0; }
        }
    }
    
    //resetear sus direcciones
    dir1X = 0; dir1Y = -1;
    dir2X = 0; dir2Y = -1;
}

void guardarPartida(int** lab) {
    FILE* f = fopen("partida_guardada.txt", "w");
    if (f == NULL) {
        printf("Error al guardar la partida.\n");
        return;
    }

    //guardar tanque 1 y2 (x, y , vidas y direccion)
    fprintf(f, "%d %d %d %d %d\n", tx1, ty1, vidas1, dir1X, dir1Y);
    fprintf(f, "%d %d %d %d %d\n", tx2, ty2, vidas2, dir2X, dir2Y);

    //guardar tiempo
    long tiempoJugado = (long)(time(NULL) - inicio);
    fprintf(f, "%ld\n", tiempoJugado);

    //guardar posicion de corazon
    long deltaSpawnCorazon = (long)(spawnCorazon - inicio);
    long deltaUltimoCorazon = (long)(ultimoCorazon - inicio);
    fprintf(f, "%d %d %ld %ld\n", corazonX, corazonY, deltaSpawnCorazon, deltaUltimoCorazon);

    long deltaBomba = (long)(tiempoBomba - inicio);
    fprintf(f, "%d %d %ld\n", bombaX, bombaY, deltaBomba);

    //guardar el estado del mapa
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            fprintf(f, "%d ", lab[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void cargarPartida(int** lab) {
    FILE* f = fopen("partida_guardada.txt", "r");
    if (f == NULL) {
        printf("No existe partida guardada.\n");
        return; 
    }

    //cargar tanque 1 y 2, posicion, vidas y direccion
    fscanf(f, "%d %d %d %d %d", &tx1, &ty1, &vidas1, &dir1X, &dir1Y);
    fscanf(f, "%d %d %d %d %d", &tx2, &ty2, &vidas2, &dir2X, &dir2Y);

    //cargar tiempo
    long tiempoGuardado;
    fscanf(f, "%ld", &tiempoGuardado);
    inicio = time(NULL) - tiempoGuardado;

    //cargar corazon
    long deltaSpawn, deltaUltimo;
    fscanf(f, "%d %d %ld %ld", &corazonX, &corazonY, &deltaSpawn, &deltaUltimo);

    spawnCorazon = inicio + deltaSpawn;
    ultimoCorazon = inicio + deltaUltimo;

    long deltaBomba;
    fscanf(f, "%d %d %ld", &bombaX, &bombaY, &deltaBomba);
    tiempoBomba = inicio+ deltaBomba;

    //cargar estado del mapa
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            fscanf(f, "%d", &lab[i][j]);
        }
    }
    fclose(f);
}