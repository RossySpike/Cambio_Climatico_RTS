#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <random>
#include <windows.h>

#define color SetConsoleTextAttribute
using namespace std;

random_device rd;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// CASILLAS DEL TABLERO CON LAS REGIONES
string aper = " ||====================================||"
              "\n"
              " ||";

string cier = "||  "
              "\n"
              " ||====================================||  "
              "\n";

struct Region {
  string renom, dib;
  int tiempevento, numevento;
  Region *proxre, *prevre;
};

struct Jugador {
  string nombre, contnom;
  int ID, puntos, dinero, reputacion, voluntarios, prob_eventos, contnum;
  Jugador *prox;
  Region *regiones;
};

int aleat(int min, int max) {
  uniform_int_distribution<int> dist(min, max);
  int num = dist(rd);
  return num;
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

string nombreDif(Jugador *inicio, string nom) {
  Jugador *mover = inicio;
  while (mover != NULL) {
    if (nom == mover->nombre) {
      cout << "El nombre ingresado ya fue ocupado, por favor intente "
              "nuevamente: ";
      cin >> nom;
      nombreDif(inicio, nom);
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
  nuevo->ID = numjug;
  string nombre;
  cout << "Jugador nro [" << numjug << "] por favor indica tu nombre\n>>: ";
  cin >> nombre;
  nuevo->nombre = nombreDif(inicio, nombre);
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
  int numjug = 1;
  Jugador *inicio = NULL;
  inicio = crearJugador(inicio, 1);
  Jugador *anterior = inicio;
  Jugador *aux = NULL;
  numjug++;
  while (cantjug > 0) {
    aux = crearJugador(inicio, numjug);
    anterior->prox = aux;
    anterior = aux;
    numjug++;
    cantjug--;
  }
  return inicio;
}

void mostrarContAsig(Jugador *actual) {
  while (actual != NULL) {
    cout << " A el jugador '" << actual->nombre
         << "' le corresponde proteger al continente: " << actual->contnom
         << endl;
    actual = actual->prox;
  }
}

void mostrarRe(Region *inicio, Region *actualRe) {
  cout << "Utiliza las flechas '^' y 'v' para seleccionar la region que deseas "
          "visitar."
       << endl;
  cout << "Pulsa enter para confirmar" << endl;
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
  Region *actualRe = actual->regiones;
  cout << endl << endl;
  mostrarRe(actual->regiones, actualRe);
  do {
    input = getch();
    if (input == 72 && actualRe->prevre != NULL) {
      actualRe = actualRe->prevre;
      cout << endl << endl;
      mostrarRe(actual->regiones, actualRe);
    } else if (input == 80 && actualRe->proxre != NULL) {
      actualRe = actualRe->proxre;
      cout << endl << endl;
      mostrarRe(actual->regiones, actualRe);
    } else if (input == 13) {
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

void crearEvento(Region *inicio) {
  if (eventosEnTodas(inicio))
    return;
  int cont;
  Region *eventRe;
  do {
    eventRe = inicio;
    cont = aleat(0, 4);
    while (cont > 0) {
      eventRe = eventRe->proxre;
      cont--;
    }
  } while (eventRe->numevento != 0);
  eventRe->numevento = aleat(1, 3);
  color(hConsole, 14);
  cout << "Hay un nuevo evento en la region " + eventRe->renom << endl;
  color(hConsole, 15);
}

void camp(Jugador *actual) {
  cout << "Campaña realizada\n" << endl;
  actual->dinero += 100;
  actual->voluntarios += 2;
  actual->reputacion += 5;
}

void reclutar(Jugador *actual) {
  cout << "Reclutaste voluntarios\n" << endl;
  actual->dinero -= 50;
  actual->voluntarios += 5;
}

void ayudar(Jugador *actual) {
  cout << "Ayudaste a la region\n" << endl;
  actual->dinero -= 200;
  actual->voluntarios += 10;
  actual->puntos += (10 + actual->reputacion) / 2;
  actual->reputacion += 10;
}

void hacerEvento(Jugador *actual, Region *actualRe) {
  int input;
  bool salir = false;
  switch (actualRe->numevento) {
  case 1:
    cout << "Evento de Deforestacion" << endl;
    break;
  case 2:
    cout << "Evento de Acumulacion de Basura" << endl;
    break;
  case 3:
    cout << "Evento de Contaminacion de Agua" << endl;
    break;
  }
  while (!salir) {
    cout << "Que vas a hacer?\n1) Para ayudar\n2) Para no hacer nada\n";
    cin >> input;
    int op = numVal(input, 1, 2);
    switch (op) {
    case 1:
      cout << "Ayudaste a la region\n";
      actual->reputacion += 15;
      actualRe->numevento = 0;
      salir = true;
      break;
    case 2:
      cout << "No hiciste nada\n";
      actual->reputacion -= 10;
      actualRe->tiempevento++;
      salir = true;
      break;
    }
  }
}

void mostrarJugador(Jugador *actual, int turno) {
  cout << "Turno nro: " << turno << "\nJugador: " << actual->nombre << "\n";
  cout << "Continente asignado: " << actual->contnom << endl;
  cout << "Estadisticas:\nDinero: " << actual->dinero
       << "\nReputacion: " << actual->reputacion
       << "\nVoluntarios: " << actual->voluntarios
       << "\nPuntos: " << actual->puntos << "\n"
       << endl;
}

void acciones(Jugador *actual, Region *actualRe) {
  int input;
  bool salir = false;
  while (!salir) {
    if (actualRe->numevento != 0) {
      color(hConsole, 14);
      cout << "HAY UN EVENTO!" << endl;
      color(hConsole, 15);
      cout << "Que vas a hacer?\n1) Para camp\n2) Para reclutar\n3) Para "
              "ayudar\n4) Para hacer evento\n>>: ";
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
        hacerEvento(actual, actualRe);
        salir = true;
        break;
      }
    } else {
      cout << "Que vas a hacer?\n1) Para camp\n2) Para reclutar\n3) Para "
              "ayudar\n>>: ";
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

void nuevoTurno(Jugador **inicio, int turno) {
  Jugador *actual = *inicio;
  int x;
  while (actual != NULL) {
    mostrarJugador(actual, turno);
    x = aleat(1, 10);
    if (x + actual->prob_eventos >= 10) {
      actual->prob_eventos = 0;
      crearEvento(actual->regiones);
      acciones(actual, selectRe(actual));
    } else {
      actual->prob_eventos += 3;
      acciones(actual, selectRe(actual));
    }
    actual = actual->prox;
  }
}
