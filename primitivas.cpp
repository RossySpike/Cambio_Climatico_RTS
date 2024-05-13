#include <iostream>
using namespace std;

int AMERICA = 1;
int EUR_RU = 2;
int AFRICA = 3;
int ASIA_OCEANIA = 4;

struct Continente {
  int NOMBRE;
};

struct ContDisponibles {
  Continente *continente;
  ContDisponibles *prox;
};

struct Jugador {
  string nombre;
  int ID, puntos, dinero, reputacion, voluntarios, trabajadores;
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
  mostrarContDisp(*lista_cont);
  cin >> cont_seleccionado;
  while (!EstaDisponibe(*lista_cont, cont_seleccionado)) {
    mostrarContDisp(*lista_cont);
    cin >> cont_seleccionado;
  }
  aux->continente = EncontrarSeleccionado(*lista_cont, cont_seleccionado);
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

void mostrarJugador(Jugador *inicio) {
  Jugador *aux = inicio;
  while (aux != NULL) {
    cout << "Nombre: " << aux->nombre << " ID: " << aux->ID
         << " Dinero: " << aux->dinero << " Reputacion: " << aux->reputacion
         << " voluntarios: " << aux->voluntarios
         << " Tranajadores: " << aux->trabajadores << " Continente "
         << aux->continente->NOMBRE << endl;
    aux = aux->prox;
  }
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
