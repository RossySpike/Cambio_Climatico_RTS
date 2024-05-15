#include <iostream>
#include <random>
using namespace std;

int AMERICA = 1;
int EUR_RU = 2;
int AFRICA = 3;
int ASIA_OCEANIA = 4;

struct Continente {
  int NOMBRE, Deforestacion, Acum_Basura, Cont_Agua;
};

struct ContDisponibles {
  Continente *continente;
  ContDisponibles *prox;
};

struct Jugador {
  string nombre;
  int ID, puntos, dinero, reputacion, voluntarios, trabajadores, prob_eventos;
  Continente *continente;
  Jugador *prox;
};

void mostrarContDisp(ContDisponibles *inicio) {
  cout << "Presiona el nro junto a la region para seleccionar" << endl;
  while (inicio != NULL) {
    if (inicio->continente->NOMBRE == AMERICA) {
      cout << "1) para America" << endl;
    }
    if (inicio->continente->NOMBRE == EUR_RU) {
      cout << "2) para Europa y Rusia" << endl;
    }
    if (inicio->continente->NOMBRE == AFRICA) {
      cout << "3) para Africa" << endl;
    }
    if (inicio->continente->NOMBRE == ASIA_OCEANIA) {
      cout << "4) para Asia y Oceania" << endl;
    }
    inicio = inicio->prox;
  }
  cout << ">>: ";
}

void EliminarContDisp(ContDisponibles **inicio, int seleccionado) {
  ContDisponibles *mover, *anterior = NULL;
  mover = *inicio;
  while (mover->continente->NOMBRE != seleccionado) {
    anterior = mover;
    mover = mover->prox;
  }
  if (mover == *inicio)
    *inicio = (*inicio)->prox;
  else
    anterior->prox = mover->prox;
  delete mover;
}

Continente *EncontrarSeleccionado(ContDisponibles *inicio, int seleccionado) {
  while (inicio->continente->NOMBRE != seleccionado) {
    inicio = inicio->prox;
  }
  return inicio->continente;
}

bool EstaDisponibe(ContDisponibles *inicio, int seleccion) {
  while (inicio != NULL) {
    if (inicio->continente->NOMBRE == seleccion) {
      return true;
    }
    inicio = inicio->prox;
  }
  cout << "Valor seleccionado no disponible" << endl;
  return false;
}

void asignarRandom(Continente **contr_ptr) {
  random_device rd;
  uniform_int_distribution<int> dist(1, 10);
  (*contr_ptr)->Deforestacion = dist(rd);
  (*contr_ptr)->Acum_Basura = dist(rd);
  (*contr_ptr)->Cont_Agua = dist(rd);
}

Jugador *crearJugador(Jugador **inicio) {
  if (*inicio == NULL) {
    *inicio = new Jugador;
    (*inicio)->prox = NULL;
    return *inicio;
  } else {
    Jugador *aux = *inicio;
    while (aux->prox != NULL) {
      aux = aux->prox;
    }
    aux->prox = new Jugador;
    aux->prox->prox = NULL;
    return aux->prox;
  }
}

Jugador *llenarJugador(int nro_jugador, ContDisponibles **lista_cont) {
  Jugador *aux = new Jugador;
  int cont_seleccionado;
  cout << "Jugador nro [" << nro_jugador
       << "] por favor indica tu nombre\n>>: ";
  cin >> aux->nombre;
  aux->ID = nro_jugador;
  aux->puntos = 0;
  aux->voluntarios = 0;
  aux->dinero = 1000;
  aux->prob_eventos = 0;
  mostrarContDisp(*lista_cont);
  cin >> cont_seleccionado;
  while (!EstaDisponibe(*lista_cont, cont_seleccionado)) {
    mostrarContDisp(*lista_cont);
    cin >> cont_seleccionado;
  }
  aux->continente = EncontrarSeleccionado(*lista_cont, cont_seleccionado);
  asignarRandom(&aux->continente);
  EliminarContDisp(lista_cont, cont_seleccionado);
  return aux;
}

void limpiar(Jugador **inicio) {
  Jugador *aux = *inicio;
  while (aux != NULL) {
    *inicio = (*inicio)->prox;
    delete aux->continente;
    delete aux;
    aux = *inicio;
  }
}

int crearEvento(Jugador *actual) {
  random_device rd;
  uniform_int_distribution<int> dist(1, 15);
  int evento = dist(rd);
  if (evento <= actual->continente->Deforestacion) {
    cout << "Evento de Deforestacion" << endl;
    return 1;
  } else if (evento <= actual->continente->Acum_Basura) {
    cout << "Evento de Acumulacion de Basura" << endl;
    return 2;
  } else if (evento <= actual->continente->Cont_Agua) {
    cout << "Evento de Contaminacion de Agua" << endl;
    return 3;
  }
  return 1;
}

ContDisponibles *crearListaCont() {
  ContDisponibles *inicio = new ContDisponibles;
  inicio->continente = new Continente;
  inicio->continente->NOMBRE = AMERICA;
  inicio->prox = new ContDisponibles;
  inicio->prox->continente = new Continente;
  inicio->prox->continente->NOMBRE = EUR_RU;
  inicio->prox->prox = new ContDisponibles;
  inicio->prox->prox->continente = new Continente;
  inicio->prox->prox->continente->NOMBRE = AFRICA;
  inicio->prox->prox->prox = new ContDisponibles;
  inicio->prox->prox->prox->continente = new Continente;
  inicio->prox->prox->prox->continente->NOMBRE = ASIA_OCEANIA;
  inicio->prox->prox->prox->prox = NULL;
  return inicio;
}

void camp(Jugador **actual) {
  cout << "Campaña realizada\n" << endl;
  (*actual)->dinero += 100;
  (*actual)->voluntarios += 2;
  (*actual)->reputacion += 5;
}

void reclutar(Jugador **actual) {
  cout << "Reclutaste trabajadores\n" << endl;
  (*actual)->dinero -= 50;
  (*actual)->trabajadores += 5;
}

void ayudar(Jugador **actual) {
  cout << "Ayudaste a la region\n" << endl;
  (*actual)->dinero -= 200;
  (*actual)->voluntarios += 10;
  (*actual)->puntos += (10 + (*actual)->reputacion) / 2;
  (*actual)->reputacion += 10;
}

void hacerEvento(Jugador **actual, int evento) {
  int input;
  bool salir = false;
  switch (evento) {
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
    switch (input) {
    case 1:
      cout << "Ayudaste a la region\n";
      salir = true;
      break;
    case 2:
      cout << "No hiciste nada\n";
      (*actual)->reputacion -= 10;
      salir = true;
      break;
    default:
      cout << "Valor no valido\n";
    }
  }
}

void mostrarJugador(Jugador *actual, int turno) {
  cout << "Turno nro: " << turno << "\nJugador: " << actual->nombre << "\n";
  if (actual->continente->NOMBRE == AMERICA) {
    cout << "Region: America" << endl;
  }
  if (actual->continente->NOMBRE == EUR_RU) {
    cout << "Region: Europa y Rusia" << endl;
  }
  if (actual->continente->NOMBRE == AFRICA) {
    cout << "Region: Africa" << endl;
  }
  if (actual->continente->NOMBRE == ASIA_OCEANIA) {
    cout << "Region: Asia y Oceania" << endl;
  }
  cout << "Estadisticas:\nDinero: " << actual->dinero
       << "\nReputacion: " << actual->reputacion
       << "\nVoluntarios: " << actual->voluntarios
       << "\nTrabajadores: " << actual->trabajadores
       << "\nPuntos: " << actual->puntos << "\n"
       << endl;
}

void acciones(Jugador **actual, bool EventDisp, int evento) {
  int input;
  bool salir = false;
  while (!salir) {
    if (EventDisp) {
      cout << "Que vas a hacer?\n1) Para camp\n2) Para reclutar\n3) Para "
              "ayudar\n4) Para hacer evento\n>>: ";
      cin >> input;
      switch (input) {
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
        hacerEvento(actual, evento);
        salir = true;
        break;
      default:
        cout << "Valor no valido\n";
        break;
      }
    } else {
      cout << "Que vas a hacer?\n1) Para camp\n2) Para reclutar\n3) Para "
              "ayudar\n>>: ";
      cin >> input;
      cout << endl;
      switch (input) {
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
      default:
        cout << "Valor no valido\n";
        break;
      }
    }
    cout << endl;
  }
}

void nuevoTurno(Jugador **inicio, int turno) {
  Jugador *actual = *inicio;
  random_device rd;
  while (actual != NULL) {
    uniform_int_distribution<int> dist(1, 10);
    mostrarJugador(actual, turno);
    if (dist(rd) + actual->prob_eventos >= 10) {
      int evento = crearEvento(actual);
      acciones(&actual, true, evento);
    } else
      acciones(&actual, false, 0);
    actual = actual->prox;
  }
}
