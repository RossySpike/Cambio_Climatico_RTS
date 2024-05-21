#include "FuncionesYDeclaraciones.cpp"

int MAX_TURN = 10;
int main() {
  int input = 0;
  cout << "---- BIENVENIDO A ECOWORLD ----" << endl << endl;
  cout << "Indique cantidad de jugadores (2-4) \n>> ";
  cin >> input;
  int cantjug = numVal(input, 2, 4);
  Jugador *jugadores = iniciarJugs(cantjug - 1);
  cout << "Continentes asignados:" << endl;
  mostrarContAsig(jugadores);
  cout << "----- QUE COMIENCE EL JUEGO! -----" << endl;
  int turno = 1;
  while (turno <= MAX_TURN) {
    nuevoTurno(&jugadores, turno);
    turno++;
  }
  return 0;
}
