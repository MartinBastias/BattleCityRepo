# BattleCity
Proyecto de Programación de Computadores grupo 11:
* Alan Flores
* Rodrigo Domínguez 
* Martin Bastias

# Compilar y correr

Este archivo está destinado para sistemas **Windows**

Este archivo contiene la libreria de visualizacion Raylib (necesaria) así que no hay que instalar nada además de un compilador

La linea para compilar es esta:

_gcc Main.c Funciones.c Variables.c Visuals.c -o BattleCity.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm_

 Para correr el juego:

_./BattleCity.exe_

# Gameplay

| Acción | Jugador 1 (Morado) | Jugador 2 (Cyan) | Sistema |
| :--- | :---: | :---: | :---: |
| **Moverse** | `W`, `A`, `S`, `D` | `I`, `J`, `K`, `L` | - |
| **Disparar** | `Q` | `U` | - |
| **Guardar Partida** | - | - | `ESPACIO` |
| **Cargar Partida** | - | - | `SHIFT IZQ` |
| **Reiniciar** | - | - | `ENTER` (Al finalizar) |

El objetivo es moverse por el laberinto para eliminar el tanque enemigo

Pierde quien quede sin vidas primero

# Mejoras a considerar

1.-Implementación de un sistema de Guardado y Carga:
* No solo se guarda el mapa, sino también:
    * Posición exacta de los jugadores
    * Vidas restantes
    * Tiempo transcurrido
    * Ubicación y estado de los power ups (bombas y corazones activos)

2.-Sistema de power ups y tiempo:
* Objetos como:
    * **Corazón:** Aparece aleatoriamente y recupera una vida al jugador que lo tome (siempre y cuando las vidas del jugador sean menores a 3)
    * **Bomba:** Aparece en el mapa y explota tras 3 segundos, rompiendo paredes y dañando tanques en un area de 3x3
* Temporizador: Limite de tiempo de 120 segundos por partida

3.-Visualización (Raylib):
* Uso de biblioteca Raylib para renderizado 2D
* Distintos sprites personalizados hechos a mano para los tanques (con rotacion segun dirección), muros (destruibles e indestructibles) y power ups.

4.-Sonidos y música personalizada compuesta por nosotros:
* Música de fondo en bucle.
* Música específica para la pantalla de "Game Over".
* Efectos de sonido (Disparos, Explosiones, Power-ups)

5.-Menu extra:
* Menú de fin de juego: Pantalla interactiva que anuncia al ganador y permite reiniciar la partida sin cerrar el programa.
