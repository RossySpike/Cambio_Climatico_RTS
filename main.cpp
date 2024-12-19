#include "FuncionesYDeclaraciones.cpp"

/*INTEGRANTES:
MATIAS LEON
JOHNNY CAMERO*/

int main() {
  int op;
  if (No_existe("Eventos.txt")) {
    color(hConsole,12);
    cout << "ERROR! NO SE HA ENCONTRADO EL ARCHIVO DE EVENTOS" << endl;
    exit(-1);
  }
  cargar_eventos();

  while (!SALIR) {

    mostrar_menu_inicio();

    if (SALIR)
      continue;

    mostrarContAsig(JUGADORES);
    if (RONDA == 1)
      cout << "QUE COMIENCE EL JUEGO!" << endl << endl << endl;
    else
      cout << "QUE CONTINUE EL JUEGO!" << endl << endl << endl;

    // SE INICIA UNA NUEVA RONDA
    while (RONDA <= MAX_RONDA) {
      // SI TODOS LOS JUGADORES SON ELIMINADOS, ACABA EL JUEGO
      if (JUGADORES == NULL) {
        cout << endl
             << endl
             << "TODOS LOS JUGADORES FUERON ELIMINADOS. EL JUEGO HA FINALIZADO"
             << endl
             << endl;
        Sleep(2500);
        RONDA = 11; // SE LE ASIGNA ESTE VALOR PARA QUE SE IMPRIMAN LOS
                    // RESULTADOS Y SE ALMACENEN EN EL TOP TEN
        break;
      }
      // SI SOLO QUEDA UN JUGADOR VIVO, ACABA EL JUEGO
      else if (JUGADORES->prox == NULL) {
        cout << endl
             << endl
             << "EL JUGADOR " << JUGADORES->nombre
             << " ES EL UNICO SOBREVIVIENTE. EL JUEGO HA FINALIZADO " << endl
             << endl;

        // ANTES DE GUARDAR LOS PUNTOS DEL JUGADOR, SE LE AGREGAN 100 PUNTOS POR
        // CADA RONDA FALTANTE
        color(hConsole, 10);
        cout << "Bonificacion del jugador " << JUGADORES->nombre
             << "por sobrevivir: +" << 100 * (MAX_RONDA - RONDA + 1)
             << " puntos" << endl;
        color(hConsole, 15);
        JUGADORES->puntos += (100 * (MAX_RONDA - RONDA + 1));
        guardarJug(JUGADORES->nombre);
        Sleep(2500);
        RONDA = 11; // SE LE ASIGNA ESTE VALOR PARA QUE SE IMPRIMAN LOS
                    // RESULTADOS Y SE ALMACENEN EN EL TOP TEN
        break;
      }
      // COMIENZA UNA NUEVA RONDA
      else {
        op = selecOp("Deseas continuar la partida?", "Continuar la partida", "Salir y guardar", "Salir sin guardar", "", 3, NULL, NULL);

        if(op == 1)
          nuevaRonda();
        else if (op == 2) {
          guardar_partida(JUGADORES);
          break;
        } else
          break;
        
        RONDA++;
      }
    }

    // EN CASO DE QUE TERMINE LA PARTIDA
    if (RONDA > 10) {
      // SE MUESTRAN LOS RESULTADOS, LAS  POSICIONES SE ASIGNARAN DANDO
      // PRIORIDAD A LOS PUNTOS ACUMULADOS
      mostrarResultados(LISTA_PUNTOS);

      // SE ACTUALIZA EL TOP TEN  DE SER NECESARIO;
      actualizarTop(copiarCont("TOPTEN.txt"), LISTA_PUNTOS);

      // TERMINA EL JUEGO
      color(hConsole, 11);
      cout << "Muchas gracias por jugar" << endl << endl;
      Sleep(2000);

      // SE ELIMINA EL ARCHIVO CON LOS DATOS DE LA PARTIDA
      remove("PARTIDA.txt");
    }

    // SE VACIA LA LISTA DE PUNTOS JUGADORES Y REGIONES Y SE REINICIA EL VALOR
    // DE LA RONDA POR SI EL USUARIO DECIDE VOLVER A JUGAR UNA NUEVA PARTIDA
    vaciarPuntos();
    limpiarJugs();
    RONDA = 1;
  }
  limpiar_list_event();
  return 0;
}
