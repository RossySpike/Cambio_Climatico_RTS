#include "primitivas.cpp"

int main() {
  ContDisponibles *listaCont = crearListaCont();
  Jugador *jugadores = llenarJugador(1, &listaCont);
  cout << endl;
  jugadores->prox = llenarJugador(2, &listaCont);
  cout << endl;
  jugadores->prox->prox = llenarJugador(3, &listaCont);
  cout << endl;
  jugadores->prox->prox->prox = llenarJugador(4, &listaCont);
  cout << endl;
  jugadores->prox->prox->prox->prox = NULL;
  mostrarJugador(jugadores);
  limpiar(&jugadores);
  return 0;
}
