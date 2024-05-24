#include "EventosClimaticos.cpp"
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <windows.h>

#define color SetConsoleTextAttribute
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int ronda = 1;
int MAX_RONDA = 10;

// CASILLAS DEL TABLERO CON LAS REGIONES
string aper = " ||==============================||"
              "\n"
              " ||";

string cier = " ||  "
              "\n"
              " ||==============================||  "
              "\n";

struct Region {
  string renom, dib;
  int tiempevento, numevento;
  Region *proxre, *prevre;
};

struct Jugador {
  string nombre, contnom;
  int contnum, puntos, dinero, reputacion, voluntarios, prob_eventos;
  Jugador *prox;
  Region *regiones;
};

int selecOp(string pregunta, string resp1, string resp2, string resp3,
            string resp4, int cantresp) {
  char input;
  int op = 1;
  color(hConsole, 11);
  cout << pregunta;
  color(hConsole, 15);
  do {
    cout << endl << endl;
    cout << "Utiliza las teclas '^' y 'v' para elegir tu opcion. Pulsa enter "
            "para confirmar:"
         << endl;
    if (op == 1)
      color(hConsole, 11);
    cout << "1.- " << resp1 << endl;
    color(hConsole, 15);
    if (op == 2)
      color(hConsole, 11);
    cout << "2.- " << resp2 << endl;
    color(hConsole, 15);
    if (op == 3)
      color(hConsole, 11);
    cout << "3.- " << resp3 << endl;
    color(hConsole, 15);
    if (op == 4)
      color(hConsole, 11);
    if (resp4 != "") {
      cout << "4.- " << resp4 << endl;
      color(hConsole, 15);
    }

    input = getch();
    while (input != 72 && input != 80 && input != 13)
      input = getch();
    if (input == 72 && op > 1)
      op--;
    else if (input == 80 && op < cantresp)
      op++;

  } while (input != 13);
  return op;
}

int numVal(int num, int min, int max) {
  if (num > max || num < min) {
    cout << "ERROR, el eumero introducido no se encuentra dentro del rango "
            "establecido ("
         << min << " - " << max << "). Intente nuevamente: ";
    cin >> num;
    num = numVal(num, min, max);
    return num;
  }
  return num;
}

string nomVal(Jugador *inicio, string nom) {
  Jugador *mover = inicio;
  while (mover != NULL) {
    if (nom.length() == 0 || nom[0] == ' ') {
      cout << "El nombre ingresado no puede estar vacío o empezar por un "
              "espacio en blanco, por favor intente nuevamente: ";
      cin >> nom;
      nomVal(inicio, nom);
      return nom;
    }
    if (nom == mover->nombre) {
      cout << "El nombre ingresado ya fue ocupado, por favor intente "
              "nuevamente: ";
      cin >> nom;
      nomVal(inicio, nom);
      return nom;
    }
    mover = mover->prox;
  }
  return nom;
}

bool contDis(Jugador *mover, int num) {
  while (mover != NULL) {
    if (num == mover->contnum)
      return false;
    mover = mover->prox;
  }
  return true;
}

Region *crearRegiones(string r1, string r2, string r3, string r4, string r5) {
  Region *inicio = new Region;
  inicio->numevento = 0;
  inicio->tiempevento = 0;
  inicio->proxre = NULL;
  inicio->prevre = NULL;
  inicio->renom = r1;
  inicio->dib = "   " + r1;
  while ((inicio->dib).length() < 29)
    inicio->dib += " ";
  Region *anterior = inicio;
  Region *aux = NULL;
  for (int i = 2; i < 6; i++) {
    aux = new Region;
    aux->numevento = 0;
    aux->tiempevento = 0;
    aux->proxre = NULL;
    aux->prevre = anterior;
    switch (i) {
    case 2: {
      aux->renom = r2;
      aux->dib = "   " + r2;
      while ((aux->dib).length() < 29)
        aux->dib += " ";
      break;
    }
    case 3: {
      aux->renom = r3;
      aux->dib = "   " + r3;
      while ((aux->dib).length() < 29)
        aux->dib += " ";
      break;
    }
    case 4: {
      aux->renom = r4;
      aux->dib = "   " + r4;
      while ((aux->dib).length() < 29)
        aux->dib += " ";
      break;
    }
    case 5: {
      aux->renom = r5;
      aux->dib = "   " + r5;
      while ((aux->dib).length() < 29)
        aux->dib += " ";
      break;
    }
    }
    anterior->proxre = aux;
    anterior = aux;
  }
  return inicio;
}

Jugador *crearJugador(Jugador *inicio, int numjug) {
  Jugador *nuevo = new Jugador;
  cout << "Jugador nro [" << numjug << "] por favor indica tu nombre\n>>: ";
  cin >> nuevo->nombre;
  nuevo->nombre = nomVal(inicio, nuevo->nombre);
  nuevo->puntos = 0;
  nuevo->voluntarios = 0;
  nuevo->dinero = 1000;
  nuevo->prob_eventos = 0;
  nuevo->reputacion = 0;
  nuevo->prox = NULL;
  int contrand = aleat(1, 4);
  while (!contDis(inicio, contrand))
    contrand = aleat(1, 4);
  switch (contrand) {
  case 1: {
    nuevo->contnom = "AMERICA";
    nuevo->contnum = 1;
    nuevo->regiones =
        crearRegiones("America del Norte", "Mesoamerica", "America Central",
                      "America del Sur", "Las Antillas");
    break;
  }

  case 2: {
    nuevo->contnom = "EUROPA";
    nuevo->contnum = 2;
    nuevo->regiones =
        crearRegiones("Escandinavia", "Europa Occidental", "Europa Oriental",
                      "Cuenca del Mediterraneo", "Rusia");
    break;
  }

  case 3: {
    nuevo->contnom = "ASIA Y OCEANIA";
    nuevo->contnum = 3;
    nuevo->regiones =
        crearRegiones("Medio Oriente", "Asia Meridional", "Asia Oriental",
                      "Sureste de Asia", "Oceania");
    break;
  }

  case 4: {
    nuevo->contnom = "AFRICA";
    nuevo->contnum = 4;
    nuevo->regiones =
        crearRegiones("Africa Septentrional", "Africa Occidental",
                      "Africa Central", "Africa Oriental", "Africa Austral");
    break;
  }
  }
  return nuevo;
}

Jugador *iniciarJugs(int cantjug) {
  cout << endl << endl;
  int numjug = 1;
  Jugador *inicio = NULL;
  inicio = crearJugador(inicio, numjug);
  cantjug--;
  numjug++;
  Jugador *anterior = inicio;
  Jugador *aux = NULL;
  while (cantjug > 0) {
    cout << endl;
    aux = crearJugador(inicio, numjug);
    anterior->prox = aux;
    anterior = aux;
    cantjug--;
    numjug++;
  }
  return inicio;
}

void mostrarContAsig(Jugador *actual) {
  color(hConsole, 11);
  cout << endl
       << endl
       << endl
       << "Asignando continentes . . ." << endl
       << endl
       << endl;
  color(hConsole, 15);
  Sleep(3000);
  while (actual != NULL) {
    cout << " A el jugador '";
    color(hConsole, 11);
    cout << actual->nombre;
    color(hConsole, 15);
    cout << "' le corresponde proteger el continente: " << actual->contnom
         << endl
         << endl;
    actual = actual->prox;
    Sleep(1000);
  }
  Sleep(3000);
}

void mostrarJugador(Jugador *actual) {
  cout << endl;
  color(hConsole, 11);
  cout << "____________________________________________________________________"
          "_____________________________"
       << endl;
  cout << "Ronda nro: " << ronda
       << "                                       Jugador: " << actual->nombre
       << "(" << actual->contnom << ")"
       << "\n";
  cout << "Dinero: " << actual->dinero
       << " $ |  Reputacion:" << actual->reputacion
       << " | Voluntarios en la region: " << actual->voluntarios
       << " | Puntos: " << actual->puntos << endl;
  cout << "____________________________________________________________________"
          "_____________________________"
       << endl;
  color(hConsole, 15);
}

void mostrarRe(Region *inicio, Region *actualRe) {
  cout << endl << endl;
  cout << "Utiliza las teclas '^' y 'v' para seleccionar la region que deseas "
          "visitar. Pulsa enter para confirmar"
       << endl;
  Region *auxRe = inicio;
  while (auxRe != NULL) {
    if (auxRe == actualRe) {
      color(hConsole, 11);
      cout << aper << auxRe->dib << cier;
      color(hConsole, 15);
    } else if (auxRe->numevento != 0) {
      color(hConsole, 14);
      cout << aper << auxRe->dib << cier;
      color(hConsole, 15);
    } else
      cout << aper << auxRe->dib << cier;

    auxRe = auxRe->proxre;
  }
}

Region *selectRe(Jugador *actual) {
  char input;
  Region *actualRe = NULL;
  mostrarRe(actual->regiones, actualRe);
  actualRe = actual->regiones;
  do {
    input = getch();
    if (input == 72 && actualRe->prevre != NULL) {
      actualRe = actualRe->prevre;
      mostrarJugador(actual);
      mostrarRe(actual->regiones, actualRe);
    } else if (input == 80 && actualRe->proxre != NULL) {
      actualRe = actualRe->proxre;
      mostrarJugador(actual);
      mostrarRe(actual->regiones, actualRe);
    } else if (input == 13) {
      cout << endl;
      cout << "Se ha seleccionado la region: " + actualRe->renom << endl
           << endl;
    }
  } while (input != 13);
  return actualRe;
}

bool eventosEnTodas(Region *mover) {
  int contEv = 0;
  while (mover != NULL) {
    if (mover->numevento != 0)
      contEv++;
    mover = mover->proxre;
  }
  if (contEv == 5)
    return true;
  else
    return false;
}

Evento *crearEvento(Region *inicio) {
  if (eventosEnTodas(
          inicio)) // Verifica que no hayan eventos en todas las regiones
    return NULL;
  int cont;
  Region *eventRe;
  do {
    eventRe = inicio;
    cont = aleat(0, 4);
    while (cont > 0) { // Selecciona una de las regiones
      eventRe = eventRe->proxre;
      cont--;
    }
  } while (eventRe->numevento != 0);
  Evento *Obt = ObtEvent(Lista_Eventos, aleat(1, 15));
  eventRe->numevento = Obt->EventoID;
  color(hConsole, 14);
  cout << "Hay un nuevo evento en la region " + eventRe->renom + Obt->NombreEvento << endl;
  color(hConsole, 15);
  return Obt;
}

void camp(Jugador *actual) {
  cout << "\n\n================\nHas concientizado de los problemas "
          "ambientales en la region. Dinero +100$ - Voluntarios +2 - "
          "Reputacion +5\n================"
       << endl;
  actual->dinero += 100;
  actual->voluntarios += 2;
  actual->reputacion += 5;
}

void reclutar(Jugador *actual) {
  cout << "\n\n================\nHas reclutado nuevos voluntarios, sin embargo "
          "tienes que comprar su equipo de trabajo. Dinero -50$ - Voluntarios "
          "+8\n================"
       << endl;
  actual->dinero -= 50;
  actual->voluntarios += 8;
}

void ayudar(Jugador *actual) {
  cout << "\n\n================\nAyudaste a la region. Dinero -200$ - "
          "Voluntarios +10 - Puntos +"
       << int((10 + actual->reputacion) / 2) << "\n================" << endl;
  actual->dinero -= 200;
  actual->voluntarios += 10;
  actual->puntos += int((10 + actual->reputacion) / 2);
  actual->reputacion += 10;
}

void MostrarRespDisponibles(Evento *RespDisp) {
  while (RespDisp != NULL) {
    cout << RespDisp->EventoID << ") " << RespDisp->Respuesta << endl;
    RespDisp = RespDisp->prox;
  }
}

void hacerEvento(Jugador *actual, Region *actualRe, Evento *Obtenido) {
  bool salir = false;
  int ObtenidoPos = aleat(1, 4);
  Evento *ListaRespDisp = CrearListaRespuestas(Obtenido, ObtenidoPos);
  cout << "El evento es: " << Obtenido->NombreEvento << endl;
  while (!salir) {
    if (RespuestaAcertada(selecOp("Que vas a hacer?", ListaRespDisp->Respuesta,
                                  ListaRespDisp->prox->Respuesta,
                                  ListaRespDisp->prox->prox->Respuesta,
                                  ListaRespDisp->prox->prox->prox->Respuesta,
                                  4),
                          ObtenidoPos)) { // AQUI
      cout << "\n\n================\nRespuesta Correcta! Reputacion +15 Puntos +100\n================\n\n";
      actual->reputacion += 15;
      actual->puntos += 100;
      actualRe->numevento = 0;
      salir = true;
    } else {
      cout << "\n\n================\nRespuesta Incorrecta :c Reputacion -10 Puntos -50\n================\n\n";
      actual->reputacion -= 10;
      actual->puntos -= 50;
      actualRe->tiempevento++;
      salir = true;
    }
  }
  freeEventList(&ListaRespDisp);
}

void acciones(Jugador *actual, Region *actualRe, Evento *Obtenido) {
  int input;
  bool salir = false;
  cout << endl << endl;
  while (!salir) {
    if (actualRe->numevento != 0) {
      color(hConsole, 14);
      cout << "HAY UN EVENTO!" << endl;
      color(hConsole, 15);
      cout << "Que vas a hacer?\n1) Para camp (Dinero +100$ - Voluntarios +2 - "
              "Reputacion +5)\n2) Para reclutar (Dinero -50$ - Voluntarios + "
              "8)\n3) Para "
              "ayudar (Dinero -200$ - Voluntarios +10 - Puntos +"
           << int((10 + actual->reputacion) / 2)
           << ")\n4) Para hacer evento (Respuesta Correcta: Puntos +100 || "
              "Respuesta Incorrecta: Puntos -50 )\n>>: ";
      cin >> input;
      int op = numVal(input, 1, 4);
      switch (op) {
      case 1:
        camp(actual);
        salir = true;
        break;
      case 2:
        reclutar(actual);
        salir = true;
        break;
      case 3:
        ayudar(actual);
        salir = true;
        break;
      case 4:
        hacerEvento(actual, actualRe, Obtenido);
        salir = true;
        break;
      }
    } else {
      cout << "Que vas a hacer?\n1) Para camp (Dinero +100$ - Voluntarios +2 - "
              "Reputacion +5)\n2) Para reclutar (Dinero -50$ - Voluntarios + "
              "8)\n3) Para "
              "ayudar (Dinero -200$ - Voluntarios +10 - Puntos +"
           << int((10 + actual->reputacion) / 2) << ")\n>>: ";
      cin >> input;
      int op = numVal(input, 1, 3);
      cout << endl;
      switch (op) {
      case 1:
        camp(actual);
        salir = true;
        break;
      case 2:
        reclutar(actual);
        salir = true;
        break;
      case 3:
        ayudar(actual);
        salir = true;
        break;
      }
    }
    cout << endl;
  }
}

void nuevaRonda(Jugador **inicio) {
  Jugador *actual = *inicio;
  int x;
  Sleep(1500);
  color(hConsole, 10);
  cout << endl
       << "-----------------------------------  RONDA" << ronda
       << "  ----------------------------------- " << endl;
  color(hConsole, 15);
  Sleep(2500);
  while (actual != NULL) {
    mostrarJugador(actual);
    x = aleat(1, 10);
    if (x + actual->prob_eventos >= 10) {
      actual->prob_eventos = 0;
      Evento *Obtenido = crearEvento(actual->regiones);
      acciones(actual, selectRe(actual), Obtenido);
    } else {
      actual->prob_eventos += 3;
      acciones(actual, selectRe(actual), NULL);
    }
    actual = actual->prox;
  }
}
