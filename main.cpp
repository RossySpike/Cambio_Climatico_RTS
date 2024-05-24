#include "FuncionesYDeclaraciones.cpp"

int main() {
  int cantjug = 0;
  color(hConsole, 10);
  cout<<"O))))))))    O))       O))))     O))        O))    O))))     O)))))))    O))      O))))) \n";
  cout<<"O))       O))   O))  O))    O))  O))        O))  O))    O))  O))    O))  O))      O))   O))\n";
  cout<<"O))      O))       O))        O))O))   O)   O))O))        O))O))    O))  O))      O))    O))\n";
  cout<<"O))))))  O))       O))        O))O))  O))   O))O))        O))O) O))      O))      O))    O))\n";
  cout<<"O))      O))       O))        O))O)) O) O)) O))O))        O))O))  O))    O))      O))    O))\n";
  cout<<"O))       O))   O))  O))     O)) O) O)    O))))  O))     O)) O))    O))  O))      O))   O))\n";
  cout<<"O))))))))   O))))      O))))     O))        O))    O))))     O))      O))O))))))))O)))))\n\n"<<endl;
  color(hConsole, 15);
  // sleep(2000);
  cantjug =
      selecOp("Indique cantidad de jugadores (2-4)", "2", "3", "4", "", 3);
  cantjug++;
  Jugador *jugadores = iniciarJugs(cantjug);
  mostrarContAsig(jugadores);
  cout << "QUE COMIENCE EL JUEGO!" << endl << endl << endl;
  while (ronda <= MAX_RONDA) {
    nuevaRonda(&jugadores);
    ronda++;
  }
  return 0;
}
