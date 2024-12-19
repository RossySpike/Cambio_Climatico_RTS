#include <conio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <windows.h>

#define color SetConsoleTextAttribute
using namespace std;

random_device rd;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// VARIABLES, STRUCTS Y PUNTEROS PRINCIPALES DEL PROGRAMA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DISENO DE LAS CASILLAS DEL TABLERO DE CADA CONTINENTE
const string aper = " ||==============================||"
                    "\n"
                    " ||";

const string cier = " ||  "
                    "\n"
                    " ||==============================||  "
                    "\n";

// STRUCT PARA CREAR UNA LISTA CON LOS PUNTOS OBTENIDOS DE CADA JUGADOR Y HASTA
// CUAL RONDA SOBREVIVIERON
struct PuntosFinales {
  int puntosjug, ultRonda;
  string nomjug;
  PuntosFinales *prox;
};

// STRUCT PARA CREAR UNA LISTA  CON LOS EVENTOS CLIMATICOS POSIBLES Y SU ACCION
// DE CONSERVACION CORRESPONDIENTE (RESPUESTA)
struct Evento {
  string NombreEvento;
  int EventoID;
  string Respuesta;
  Evento *prox;
};

// STRUCT PARA CREAR UNA LISTA DOBLEMENTE ENLAZADA PARA QUE CADA JUGADOR PUEDA
// DESPLAZARSE EN SU CONTINENTE
struct Region {
  string renom, dib;
  int tiempevento, voluntarios;
  Region *proxre, *prevre;
  Evento *eventoAc;
};

// STRUCT PARA CREAR UNA MULTILISTA  CON LOS JUGADORES, SUS RECURSOS,
// ESTADISTICAS Y REGIONES.
struct Jugador {
  string nombre, contnom;
  int contnum, puntos, dinero, reputacion, prob_eventos;
  Jugador *prox;
  Region *regiones;
};

// STRUCT PARA CREAR UNA PILA CON EL FIN DE ORDENAR LOS JUGADORES AL
// CARGAR LA PARTIDA
struct Pila_jugadores {
  Jugador *actual;
  Pila_jugadores *prox;
};

// VARIABLES Y PUNTEROS GLOBALES
static int RONDA = 1;
static const int MAX_RONDA = 10;
static int CANT_JUGS;
static bool SALIR = false;
static Evento *LISTA_EVENTOS;
static PuntosFinales *LISTA_PUNTOS = NULL;
static Jugador *JUGADORES = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION PARA GENERAR UN NUMERO ALEATORIO EN UN RANGO DETERMINADO
int aleat(int min, int max) {
  uniform_int_distribution<int> dist(min, max);
  int num = dist(rd);
  return num;
}

// INICIALIZACION DE JUGADORES Y REGIONES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION A QUE VALIDA SI EL NOMBRE INTRODUCIDO POR UN JUGADOR NO FUE
// PREVIAMENTE UTILIZADO Y QUE NO ESTE VACIO
string nomVal(Jugador *inicio, string nom) {
  Jugador *mover = inicio;
  while (mover != NULL) {
    if (nom.length() == 0 || nom == "null") {
      cout << "El nombre ingresado no puede estar vacio "
              "o ser null. Por favor intente nuevamente: ";
      cin >> nom;
      nom = nomVal(inicio, nom);
    } else if (nom == mover->nombre) {
      cout << "El nombre ingresado ya fue ocupado, por favor intente "
              "nuevamente: ";
      cin >> nom;
      nom = nomVal(inicio, nom);
    }
    mover = mover->prox;
  }
  return nom;
}

// FUNCION PARA SABER SI EL CONTINENTE ASIGNADO A UN JUGADOR ESTA DISPONIBLE.
bool contDis(Jugador *mover, int num) {
  while (mover != NULL) {
    if (num == mover->contnum)
      return false;
    mover = mover->prox;
  }
  return true;
}

// FUNCION PARA CREAR E INSERTAR EN UNA LISTA DOBLEMENTE ENLAZADA LAS
// REGIONES ASIGNADAS A CADA JUGADOR
Region *crearRegiones(string r1, string r2, string r3, string r4, string r5) {
  Region *inicio = new Region;
  inicio->tiempevento = 0;
  inicio->voluntarios = 0;
  inicio->eventoAc = NULL;
  inicio->proxre = NULL;
  inicio->prevre = NULL;
  inicio->renom = r1;
  inicio->dib = "   " + r1;
  // ESTE CICLO WHILE ASEGURA QUE LA STRING DE LAS CASILLAS DE CADA REGION
  // TENGAN LA MISMA LONGITUD PARA UNA MEJOR APARIENCIA DEL PROGRAMA AL MOMENTO
  // DE IMPRIMIR LOS TABLEROS
  while ((inicio->dib).length() < 29)
    inicio->dib += " ";
  Region *anterior = inicio;
  Region *aux = NULL;
  for (int i = 2; i < 6; i++) {
    aux = new Region;
    aux->tiempevento = 0;
    aux->voluntarios = 0;
    aux->eventoAc = NULL;
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

// SE CREA UN JUGADOR NUEVO.
Jugador *crearJugador(Jugador *inicio, int numjug) {
  Jugador *nuevo = new Jugador;
  cout << "Jugador nro [" << numjug << "] por favor indica tu nombre\n>>: ";
  cin >> nuevo->nombre;
  nuevo->nombre = nomVal(inicio, nuevo->nombre);
  nuevo->puntos = 0;
  nuevo->dinero = 400;
  nuevo->prob_eventos = 0;
  nuevo->reputacion = 0;
  nuevo->prox = NULL;
  int contrand = aleat(1, 4);
  // SE GENERARA UN NUMERO ALEATORIO HASTA QUE EL VALOR GENERADO NO CORRESPONDA
  // A UN CONTINENTE PREVIAMENTE ASIGNADO
  while (!contDis(inicio, contrand))
    contrand = aleat(1, 4);
  // SE ASIGNA AL JUGADOR UNA LISTA CON LAS REGIONES DE SU TABLERO.
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

// SE CREA LA LISTA CON TODOS LOS JUGADORES DE ACUERDO A LA CANTIDAD  INDICADA
// POR EL USUARIO
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

// SE ELIMINAN LOS JUGADORES VIVOS Y SUS REGIONES SI YA SE GUARDARON EN EL
// ARCHIVO
void limpiarJugs() {
  Jugador *borrar = NULL;
  Region *borrarRe = NULL;
  while (JUGADORES != NULL) {
    borrar = JUGADORES;
    JUGADORES = JUGADORES->prox;
    while (borrar->regiones != NULL) {
      borrarRe = borrar->regiones;
      borrar->regiones = borrar->regiones->proxre;
      delete borrarRe;
    }
    delete borrar;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MOSTRAR ESTADISTICAS, TABLERO, OPCIONES Y DATOS AL USUARIO
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SE IMPRIMEN EN LA TERMINAL LOS CONTINENTES ASIGNADOS ALEATORIAMENTE A CADA
// JUGADOR
void mostrarContAsig(Jugador *actual) {
  color(hConsole, 11);
  cout << endl
       << endl
       << endl
       << "continentes asignados . . ." << endl
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

// SE MUESTRA EN LA TERMINAL UN ENCABEZADO CON LAS ESTADISTICAS DE CADA JUGADOR
// AL MOMENTO DE SU TURNO
void mostrarJugador(Jugador *actual, Region *actualRe) {
  cout << endl;
  color(hConsole, 11);
  cout << "____________________________________________________________________"
          "_____________________________"
       << endl;
  cout << "Ronda nro: " << RONDA
       << "                                       Jugador: " << actual->nombre
       << "(" << actual->contnom << ")"
       << "\n";
  cout << "Dinero: " << actual->dinero
       << " $ |  Reputacion:" << actual->reputacion
       << " | Voluntarios en la region: " << actualRe->voluntarios
       << " | Puntos: " << actual->puntos << endl;
  cout << "____________________________________________________________________"
          "_____________________________"
       << endl;
  color(hConsole, 15);
}

// SE MUESTRAN LAS ESTADISTICAS DE LOS JUGADORES
void mostrarJugsStats(Jugador *inicio) {
  Jugador *moverjugs = inicio;
  Region *moverRe = NULL;
  while (moverjugs != NULL) {
    cout << endl << endl << "--------------------------------" << endl;
    color(hConsole, 11);
    cout << "Jugador " << moverjugs->nombre << ": " << endl << endl;
    color(hConsole, 15);
    cout << "Puntos: " << moverjugs->puntos << endl;
    cout << "Dinero: " << moverjugs->dinero << endl;
    cout << "Reputacion: " << moverjugs->reputacion << endl << endl;
    moverRe = moverjugs->regiones;
    while (moverRe != NULL) {
      cout << "Voluntarios en la region " << moverRe->renom << ": "
           << moverRe->voluntarios << endl;
      moverRe = moverRe->proxre;
    }
    moverjugs = moverjugs->prox;
  }
}

// FUNCION PARA QUE EL USUARIO SELECCIONE UNA RESPUESTA O ACCION USANDO '^' Y
// 'v'. PERMITE MOSTRAR 4 O 3 RESPUESTAS (EN CASO DE SER 3 RESPUESTAS EL
// ARGUMENTO 'resp4' DEBE QUEDAR VACIO: "" )
int selecOp(string pregunta, string resp1, string resp2, string resp3,
            string resp4, int cantresp, Jugador *actual, Region *actualRe) {
  char input;
  int op = 0;

  do {
    cout << endl << endl;
    if (actual != NULL)
      mostrarJugador(actual, actualRe);
    color(hConsole, 11);
    cout << pregunta;
    color(hConsole, 15);

    cout
        << ". (Utiliza las teclas '^' y 'v' para elegir tu opcion. Pulsa enter "
           "para confirmar)";
    // SE PUEDEN VER LAS ESTADISTICAS DE LOS JUGADORES SOLO SI YA FUERON
    // INICIALIZADOS
    if (actual != NULL)
      cout << endl
           << "Pulsa M para mostar las estadisticas de cada jugador" << endl
           << endl;
    else
      cout << endl << endl;

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
    while ((input != 72 || op <= 1) && (input != 80 || op >= cantresp) &&
           (input != 13 || op == 0) && (input != 77 || actual == NULL))
      input = getch();
    if (input == 72)
      op--;
    else if (input == 80)
      op++;
    else if (input == 77)
      mostrarJugsStats(JUGADORES);

  } while (input != 13);
  return op;
}

// SE MUESTRA EN LA TERMINAL EL TABLERO DEL JUGADOR ACTUAL
void mostrarRe(Region *inicio, Region *actualRe) {
  cout << endl << endl;
  cout << "Utiliza las teclas '^' y 'v' para seleccionar la region que deseas "
          "visitar. Pulsa enter para confirmar"
       << endl;
  Region *auxRe = inicio;
  while (auxRe != NULL) {
    // LA CASILLA DONDE ESTA EL JUGADOR SE IMPRIME EN COLOR CIAN
    if (auxRe == actualRe) {
      color(hConsole, 11);
      cout << aper << auxRe->dib << cier;
      color(hConsole, 15);
    }
    // LAS CASILLAS DONDE HAY EVENTOS SE IMPRIMEN EN COLOR AMARILLO O ROJO
    else if (auxRe->eventoAc != NULL && auxRe->tiempevento < 4) {
      color(hConsole, 14);
      cout << aper << auxRe->dib << cier;
      color(hConsole, 15);
    } else if (auxRe->tiempevento >= 4) {
      color(hConsole, 12);
      cout << aper << auxRe->dib << cier;
      color(hConsole, 15);
    } else
      cout << aper << auxRe->dib << cier;

    auxRe = auxRe->proxre;
  }
}

// FUNCION PARA QUE EL JUGADOR ACTUAL SELECCIONE LA REGION EN LA CUAL QUIERE
// REALIZAR UNA ACCION.
Region *selectRe(Jugador *actual) {
  char input;
  char pausa;
  Region *actualRe = NULL;
  mostrarRe(actual->regiones, actualRe);
  actualRe = actual->regiones;
  pausa = getch();
  mostrarRe(actual->regiones, actualRe);
  do {
    if (input == 72 && actualRe->prevre != NULL) {
      actualRe = actualRe->prevre;
      mostrarJugador(actual, actualRe);
      mostrarRe(actual->regiones, actualRe);
    } else if (input == 80 && actualRe->proxre != NULL) {
      actualRe = actualRe->proxre;
      mostrarJugador(actual, actualRe);
      mostrarRe(actual->regiones, actualRe);
    } else if (input == 13) {
      color(hConsole, 10);
      cout << endl;
      cout << "============ Se ha seleccionado la region: " + actualRe->renom
           << " ============" << endl
           << endl;
      color(hConsole, 15);
    }
    input = getch();
  } while (input != 13);
  return actualRe;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CREACION Y ASIGNACION DE EVENTOS ALEATORIOS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION PARA CREAR CADA EVENTO CON SU ACCION DE CONSERVACION CORRESPONDIENTE
void CrearNodoEvento(Evento **inicio, string nombre, int ID, string respuesta) {
  Evento *aux = new Evento;
  aux->NombreEvento = nombre;
  aux->EventoID = ID;
  aux->Respuesta = respuesta;
  if (*inicio == NULL) {
    *inicio = aux;
    (*inicio)->prox = NULL;
  } else {
    aux->prox = *inicio;
    *inicio = aux;
  }
}

void cargar_eventos() {
  int cont = 15;
  ifstream arc;
  arc.open("Eventos.txt", ios::in);
  while (!arc.eof()) {
    Evento *aux = new Evento;
    getline(arc, aux->NombreEvento);
    getline(arc, aux->Respuesta);
    aux->EventoID = cont;
    cont--;
    aux->prox = LISTA_EVENTOS;
    LISTA_EVENTOS = aux;
  }
  Evento *aux = LISTA_EVENTOS;
  LISTA_EVENTOS = LISTA_EVENTOS->prox;
  delete aux;
}

void limpiar_list_event() {
  while (LISTA_EVENTOS != NULL) {
    Evento *aux = LISTA_EVENTOS;
    LISTA_EVENTOS = LISTA_EVENTOS->prox;
    delete aux;
  }
}

// FUNCION PARA OBTENER UN EVENTO POR SU ID
Evento *ObtEvent(Evento *mover, int ID) {
  while (mover != NULL && mover->EventoID != ID) {
    mover = mover->prox;
  }
  return mover;
}

// SE CREA LA LISTA CON LAS ACCIONES DE CONSERVACION ALEATORIAS (SOLO UNA
// CORRECTA) QUE EL USUARIO PUEDE EJECUTAR PARA RESOLVER EL EVENTO DE LA REGION
// ACTUAL
Evento *CrearListaRespuestas(Evento *correcta, int correctaPos) {
  Evento *aux1 = NULL;
  Evento *aux2 = NULL;
  int randResp;
  for (int i = 4; i > 0; i--) {
    if (i == correctaPos) {
      CrearNodoEvento(&aux1, correcta->NombreEvento, correcta->EventoID,
                      correcta->Respuesta);
    } else {
      // SE GENERA UN NUMERO ALEATORIO HASTA QUE EL VALOR GENERADO SEA DIFERENTE
      // A LOS ID DE LOS EVENTOS EN LA LISTA DE RESPUESTAS
      do {
        randResp = aleat(1, 15);
      } while (ObtEvent(aux1, randResp) != NULL ||
               randResp == correcta->EventoID);

      aux2 = ObtEvent(LISTA_EVENTOS, randResp);
      CrearNodoEvento(&aux1, aux2->NombreEvento, aux2->EventoID,
                      aux2->Respuesta);
      aux2 = NULL;
    }
  }
  return aux1;
}

// FUNCION QUE INIDICA SI LA ACCION SELECCIONADA POR EL JUGADOR FUE LA CORRECTA
bool RespuestaAcertada(int RespBuena, int RespSeleccionada) {
  return RespBuena == RespSeleccionada;
}

// FUNCION PARA VERIFICAR QUE NO HAYAN EVENTOS EN TODAS LAS REGIONES
bool eventosEnTodas(Region *mover) {
  int contEv = 0;
  while (mover != NULL) {
    if (mover->eventoAc != NULL)
      contEv++;
    mover = mover->proxre;
  }
  if (contEv == 5)
    return true;
  else
    return false;
}

// FUNCION PARA CREAR EVENTO EN UNA REGION ALEATORIA
void crearEvento(Region *inicio) {
  // SI HAY UN EVENTO EN TODAS LAS REGIONES, NO SE CREA UN NUEVO EVENTO
  if (eventosEnTodas(inicio))
    return;
  else {
    // SE SLECCIONA UNA REGION ALEATORIA QUE NO TENGA EVENTO
    Region *eventRe;
    do {
      eventRe = inicio;
      for (int cont = aleat(0, 4); cont > 0; cont--)
        eventRe = eventRe->proxre;
    } while (eventRe->eventoAc != NULL);

    // SE LE ASIGNA UN EVENTO ALEATORIA A LA REGION SELECCIONADA
    eventRe->eventoAc = ObtEvent(LISTA_EVENTOS, aleat(1, 15));
    color(hConsole, 14);
    cout << "Hay un nuevo evento en la region " << eventRe->renom << "! "
         << endl;
    color(hConsole, 15);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ACCIONES QUE PUEDE REALIZAR EL JUGADOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ACCION PARA QUE EL JUGADOR REALICE UNA CAMPANA DE CONCIENTIZACION
void camp(Jugador *actual, Region *actualRe) {
  color(hConsole, 10);
  cout << "\n============ Has concientizado de los problemas "
          "ambientales en la region. Dinero +120$ || Voluntarios +2 || "
          "Reputacion +5 ============ "
       << endl;
  color(hConsole, 15);
  actual->dinero += 120;
  actualRe->voluntarios += 2;
  actual->reputacion += 5;
}

// ACCION PARA QUE EL JUGADOR RECLUTE VOLUNTARIOS EN LA REGION ACTUAL
void reclutar(Jugador *actual, Region *actualRe) {
  color(hConsole, 10);
  cout
      << "\n============ Has reclutado nuevos voluntarios, sin embargo "
         "tienes que comprar su equipo de trabajo. Dinero -100$ || Voluntarios "
         "+7 ============"
      << endl;
  color(hConsole, 15);
  actual->dinero -= 100;
  actualRe->voluntarios += 7;
}

// ACCION PARA QUE EL USUARIO CONTRIBUYA A LA PROTECCION DE LA REGION ACTUAL Y
// GANE PUNTOS POR ELLO
void ayudar(Jugador *actual, Region *actualRe) {
  color(hConsole, 10);
  cout << "\n============ Ayudaste a la region. Dinero -100$ || "
          "Voluntarios -3 || Reputacion +10 || Puntos +"
       << int((100 + actual->reputacion * 10) / 2) << " ============" << endl;
  color(hConsole, 15);
  actual->dinero -= 100;
  actualRe->voluntarios -= 3;
  actual->puntos += int((100 + actual->reputacion * 10) / 2);
  actual->reputacion += 10;
}

// ACCION PARA QUE EL JUGADOR RESUELVA EL EVENTO CLIMATICO EN LA REGION ACTUAL
void hacerEvento(Jugador *actual, Region *actualRe) {
  actual->dinero -= 120;
  actualRe->voluntarios -= 10;
  // SE CREA LA LISTA DE RESPUESTAS Y SE ASIGNA A LA ACCION DE CONSERVACION
  // CORRECTA UNA POSICION ALEATORIA
  int correctPos = aleat(1, 4);
  Evento *ListaRespDisp = CrearListaRespuestas(actualRe->eventoAc, correctPos);
  color(hConsole, 14);
  cout << endl << "El evento es: " << actualRe->eventoAc->NombreEvento << endl;
  color(hConsole, 15);

  // SE ALMACENA EN UNA VARIABLE LA RESPUESTA DEL JUGADOR
  int resp =
      selecOp("Que vas a hacer? Evento: " + actualRe->eventoAc->NombreEvento,
              ListaRespDisp->Respuesta, ListaRespDisp->prox->Respuesta,
              ListaRespDisp->prox->prox->Respuesta,
              ListaRespDisp->prox->prox->prox->Respuesta, 4, actual, actualRe);

  // SI LA RESPUESTA ES CORRECTA SE RECOMPENSA AL JUGADOR
  if (RespuestaAcertada(correctPos, resp)) {
    color(hConsole, 10);
    cout << "\n============ Respuesta Correcta! Dinero - 120 || Voluntarios "
            "-10 || Reputacion +15"
            "|| Puntos +1000 ============\n";
    color(hConsole, 15);
    actual->reputacion += 15;
    actual->puntos += 1000;
    actualRe->eventoAc = NULL;
    actualRe->tiempevento = 0;
  }
  // SI ES INCORRECTA SE PENALIZA
  else {
    color(hConsole, 12);
    cout << "\n============ Respuesta Incorrecta. Dinero - 120 || Voluntarios "
            "-10 || Reputacion -10"
            "|| Puntos -100 ============\n";
    color(hConsole, 15);
    actual->reputacion -= 10;
    actual->puntos -= 100;
  }
}

// MENU QUE MUESTRA AL JUGADOR LAS POSIBLES ACCIONES EN LA REGION ACTUAL
void acciones(Jugador *actual, Region *actualRe) {
  cout << endl << endl;
  int op;
  // SI HAY UN EVENTO EL JUGADOR PODRA REALIZAR LAS CUATRO ACCIONES
  // ESPECIFICADAS
  if (actualRe->eventoAc != NULL) {
    color(hConsole, 14);
    cout << "HAY UN EVENTO!" << endl;
    color(hConsole, 15);
    op = selecOp("Que vas a hacer?",
                 "LLevar a cabo una campana (Dinero +120$  || Voluntarios +2 "
                 "|| Reputacion +5)",
                 "reclutar voluntarios (Dinero -100$ || Voluntarios +7)",
                 "Salvaguardar los espacios naturales de la region (Dinero "
                 "-100$ || Voluntarios -3 || Reputacion +10)",
                 "Resolver el evento ambiental actual (Dinero -120$ || "
                 "Voluntarios -10 || Reputacion +15/-10 )",
                 4, actual, actualRe);
  }
  // SI NO HAY UN EVENTO EL JUGADOR SOLO PODRA HACER TRES DE LAS ACCIONES (NO
  // PODRA RESOLVER UN EVENTO CLIMATICO)
  else {
    op = selecOp("Que vas a hacer?",
                 "LLevar a cabo una campana (Dinero +120$  || Voluntarios +2 "
                 "|| Reputacion +5)",
                 "reclutar voluntarios (Dinero -100$ || Voluntarios +7)",
                 "Salvaguardar los espacios naturales de la region (Dinero "
                 "-100$ || Voluntarios -3 || Reputacion +10)",
                 "", 3, actual, actualRe);
  }
  // SE EVALUA LA ACCION INDICADA POR EL USUARIO, EN CASO DE NO CONTAR CON LOS
  // RECURSOS SUFICIENTES SE VUELVE A MOSTRAR EL MENU DE ACCIONES
  switch (op) {
  case 1: {
    camp(actual, actualRe);
  } break;
  case 2: {
    if (actual->dinero < 100) {
      color(hConsole, 14);
      cout << endl
           << "ERROR. No tienes suficiente dinero para reclutar voluntarios"
           << endl;
      color(hConsole, 15);
      Sleep(2000);
      mostrarJugador(actual, actualRe);
      acciones(actual, actualRe);
      return;
    } else
      reclutar(actual, actualRe);
  } break;
  case 3: {
    if (actual->dinero < 100 || actualRe->voluntarios < 3) {
      color(hConsole, 14);
      cout << endl
           << "ERROR. No tienes suficientes recursos para proteger los "
              "espacios naturales de la region"
           << endl;
      color(hConsole, 15);
      Sleep(2000);
      mostrarJugador(actual, actualRe);
      acciones(actual, actualRe);
      return;
    } else
      ayudar(actual, actualRe);
  } break;
  case 4: {
    if (actual->dinero < 120 || actualRe->voluntarios < 10) {
      color(hConsole, 14);
      cout << endl
           << "ERROR. No tienes suficientes recursos para solventar el evento "
              "climatico de la region"
           << endl;
      color(hConsole, 15);
      Sleep(2000);
      mostrarJugador(actual, actualRe);
      acciones(actual, actualRe);
      return;
    } else
      hacerEvento(actual, actualRe);
  } break;
  }

  cout << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// REGISTRO DE LOS JUGADORES EN LA LISTA DE PUNTOS FINALES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SE REGISTRA LOS PUNTOS Y ULTIMA RONDA DE CADA JUGADOR
void insertarPuntos(string nombre, int puntos, int ultRonda) {
  PuntosFinales *nuevo = new PuntosFinales;
  nuevo->puntosjug = puntos;
  nuevo->ultRonda = ultRonda;
  nuevo->nomjug = nombre;
  nuevo->prox = NULL;
  if ((LISTA_PUNTOS) == NULL)
    LISTA_PUNTOS = nuevo;
  else {
    PuntosFinales *mover = LISTA_PUNTOS;
    PuntosFinales *anterior = NULL;
    while (mover != NULL && mover->puntosjug > nuevo->puntosjug) {
      anterior = mover;
      mover = mover->prox;
    }
    if (mover == LISTA_PUNTOS) {
      nuevo->prox = LISTA_PUNTOS;
      LISTA_PUNTOS = nuevo;
    } else {
      nuevo->prox = mover;
      anterior->prox = nuevo;
    }
  }
}

// SE GUARDA REGISTRO DE UN JUGADOR EN LA LISTA DE PUNTOS FINALES DE LA PARTIDA
void guardarJug(string nomJug) {
  Jugador *mover = JUGADORES;
  Jugador *anterior = NULL;
  while (mover->nombre != nomJug) {
    anterior = mover;
    mover = mover->prox;
  }
  // SE ALMACENAN LAS ESTADISTICAS DEL JUGADOR EN  LA LISTA DE PUNTOS
  insertarPuntos(mover->nombre, mover->puntos, RONDA);

  // SE ELIMINA EL JUGADOR Y SUS REGIONES
  if (mover == JUGADORES)
    JUGADORES = JUGADORES->prox;
  else
    anterior->prox = mover->prox;

  Region *borrarRe = NULL;
  while (mover->regiones != NULL) {
    borrarRe = mover->regiones;
    mover->regiones = mover->regiones->proxre;
    delete borrarRe;
  }
  delete mover;
}

// SE MUESTRA EN QUE LUGAR QUEDO CADA JUGADOR, CON SUS PUNTOS ACUMULADOS Y HASTA
// CUAL RONDA SOBREVIVIERON
void mostrarResultados(PuntosFinales *mover) {
  int pos = 1;
  color(hConsole, 11);
  cout << endl
       << endl
       << endl
       << "Calculando resultados . . ." << endl
       << endl
       << endl;
  color(hConsole, 15);
  Sleep(3000);
  while (mover != NULL) {
    cout << " El jugador'";
    color(hConsole, 11);
    cout << mover->nomjug;
    color(hConsole, 15);
    cout << " quedo en la posicion  ";
    color(hConsole, 10);
    cout << pos;
    color(hConsole, 15);
    cout << " Sobreviviendo hasta la ronda " << mover->ultRonda
         << " y acumulando un total de " << mover->puntosjug << " puntos.";
    cout << endl << endl;
    mover = mover->prox;
    pos++;
    Sleep(1000);
  }
  Sleep(3000);
}

// SE LIMPIA LA LISTA CON LOS PUNTAJES DE LOS JUGADORES
void vaciarPuntos() {
  PuntosFinales *borrar = NULL;
  while (LISTA_PUNTOS != NULL) {
    borrar = LISTA_PUNTOS;
    LISTA_PUNTOS = LISTA_PUNTOS->prox;
    delete borrar;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCIONES QUE SE EJECUTAN AL INICIO DE CADA TURNO
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION PARA DARLE AL USUARIO UNA CANTIDAD ALEATORIA DE VOLUNTARIOS (1-4) EN
// ALGUNAS DE SUS REGIONES
void darVolunAleat(Jugador *actual) {
  int cantVol = aleat(2, 5);
  cout << "Tienes " << cantVol
       << " voluntarios nuevos en algunas de tus regiones, que maravilla!"
       << endl;
  Sleep(1500);
  while (cantVol > 0) {
    // SE SELECCIONA UNA REGION ALEATORIA Y SE LE ASIGNA UN VOLUNTARIO MAS
    Region *aleatRe = actual->regiones;
    for (int cont = aleat(0, 4); cont > 0; cont--)
      aleatRe = aleatRe->proxre;
    aleatRe->voluntarios++;
    cantVol--;
  }
}

// SE INCREMENTA EL TIEMPO DE LOS EVENTOS EN LAS REGIONES DEL JUGADOR ACTUAL
bool incremenTiempEvento(Jugador *actual) {
  Region *mover = actual->regiones;
  bool eliminar = false;
  while (mover != NULL) {
    if (mover->eventoAc != NULL) {
      mover->tiempevento++;
      if (mover->tiempevento == 5) {
        color(hConsole, 12);
        // SI UNA REGION DURA CON UN EVENTO MAS DE 4 RONDAS, EL JUGADOR SE
        // ELIMINA
        cout << "El jugador " << actual->nombre
             << " ha sido eliminado por descuidar la region " << mover->renom
             << endl
             << endl
             << endl;
        color(hConsole, 15);
        eliminar = true;
        break;
      }
    }
    mover = mover->proxre;
  }
  return eliminar;
}

// SE PENALIZAA LOS JUGADORES DEPENDIENDO DEL TIEMPO DE EVENTO DE CADA UNA DE
// SUS REGIONES
void penalizaciones(Jugador *actual) {
  Region *mover = actual->regiones;
  int restPuntos;
  cout << endl;
  while (mover != NULL) {
    if (mover->tiempevento >= 4) {
      restPuntos = 20 * mover->tiempevento;
      color(hConsole, 12);
      cout << "El estado de la region " << mover->renom
           << " es critico. "
              "Solucionar el evento climatico URGENTEMENTE ( -"
           << restPuntos << " Puntos)" << endl;
      color(hConsole, 15);
      actual->puntos -= restPuntos;
    } else if (mover->tiempevento >= 2 && mover->tiempevento < 4) {
      restPuntos = 15 * mover->tiempevento;
      color(hConsole, 14);
      cout << "El estado de la region " << mover->renom
           << " es grave. "
              "Se aconseja solventar el evento climatico ( -"
           << restPuntos << " Puntos)" << endl;
      color(hConsole, 15);
      actual->puntos -= restPuntos;
    }
    mover = mover->proxre;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TRIVIAS Y DESAFIOS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION PARA HACERLE UNA PREGUNTA ALEATORIA AL JUGADOR ACTUAL
void preguntaalet(Jugador *actual) {
  color(hConsole, 10);
  cout << endl << "PREGUNTA ALEATORIA" << endl;
  color(hConsole, 15);
  Evento *evento = ObtEvent(LISTA_EVENTOS, aleat(1, 15));
  int correctPos = aleat(1, 4);
  Evento *ListaRespDisp = CrearListaRespuestas(evento, correctPos);
  color(hConsole, 14);
  cout << "En caso de existir: " << evento->NombreEvento;
  color(hConsole, 15);

  // SE ALMACENA EN UNA VARIABLE LA RESPUESTA DEL JUGADOR
  int resp = selecOp(
      "Que harias?", ListaRespDisp->Respuesta, ListaRespDisp->prox->Respuesta,
      ListaRespDisp->prox->prox->Respuesta,
      ListaRespDisp->prox->prox->prox->Respuesta, 4, actual, actual->regiones);

  if (resp == correctPos) {
    color(hConsole, 10);
    cout << endl
         << "RESPUESTA CORRECTA, +200 puntos +20 reputacion +100 dinero"
         << endl;
    color(hConsole, 15);
    actual->reputacion += 20;
    actual->puntos += 200;
    actual->dinero += 50;
  } else {
    color(hConsole, 12);
    cout << endl << "RESPUESTA INCORRECTA" << endl;
    color(hConsole, 15);
  }
  Sleep(1000);
  cout << "Ahora selecciona la region donde deseas realizar una accion" << endl;
  Sleep(1500);
}

// FUNCION PARA DESAFIAR AL MEJOR JUGADOR HASTA EL MOMENTO
void desafio(Jugador *primer) {
  char input;
  Jugador *mejor = primer;
  while (primer != NULL) {
    if (primer->puntos > mejor->puntos) {
      mejor = primer;
    }
    primer = primer->prox;
  }
  color(hConsole, 14);
  cout << endl
       << "DESAFIO AL MEJOR JUGADOR HASTA EL MOMENTO: " << mejor->nombre
       << endl;
  color(hConsole, 15);
  cout << "Pulsa ENTER para aceptar (Coste del desafio: dinero -100)." << endl
       << endl;
  input = getch();
  if (input == 13) {
    mejor->dinero -= 100;
    Evento *evento = ObtEvent(LISTA_EVENTOS, aleat(1, 15));
    int correctPos = aleat(1, 4);
    Evento *ListaRespDisp = CrearListaRespuestas(evento, correctPos);
    color(hConsole, 14);
    cout << endl << "En caso de existir " << evento->NombreEvento << endl;
    color(hConsole, 15);

    int resp = selecOp(
        "Que harias?", ListaRespDisp->Respuesta, ListaRespDisp->prox->Respuesta,
        ListaRespDisp->prox->prox->Respuesta,
        ListaRespDisp->prox->prox->prox->Respuesta, 4, mejor, mejor->regiones);

    if (resp == correctPos) {
      color(hConsole, 10);
      cout << endl
           << "RESPUESTA CORRECTA, +200 puntos +20 reputacion +100 dinero"
           << endl;
      color(hConsole, 15);
      mejor->reputacion += 20;
      mejor->puntos += 200;
      mejor->dinero += 100;
    } else {
      color(hConsole, 12);
      cout << endl
           << "RESPUESTA INCORRECTA, -200 puntos -20 reputacion" << endl;
      color(hConsole, 15);
      mejor->reputacion -= 20;
      mejor->puntos -= 200;
    }
  } else {
    cout << endl << "Reto rechazado" << endl << endl;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DESARROLLO DE CADA RONDA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// EMPIEZA UNA RONDA NUEVA COMENZANDO DESDE EL PRIMER JUGADOR
void nuevaRonda() {
  Jugador *actual = JUGADORES;
  Jugador *aux = NULL;
  int x;
  Sleep(1750);
  color(hConsole, 10);
  cout << endl
       << "-----------------------------------  RONDA " << RONDA
       << "  ----------------------------------- " << endl;
  color(hConsole, 15);
  Sleep(2500);
  while (actual != NULL) {
    color(hConsole, 11);
    cout << endl << "JUGADOR: " << actual->nombre << endl;
    color(hConsole, 15);
    Sleep(1500);
    darVolunAleat(actual);
    penalizaciones(actual);
    if (aleat(1, 10) > 9)
      preguntaalet(actual);
    else
      mostrarJugador(actual, actual->regiones);

    x = aleat(1, 9);
    // SE REINICIA LA PROBABILIDAD DE EVENTOS DEL JUGADOR
    if (x + actual->prob_eventos >= 10) {
      actual->prob_eventos = 0;
      crearEvento(actual->regiones);
    }
    // SI NO SE CREA UN EVENTO, AUMENTE LA PROBABILIDAD DE EVENTOS DEL JUGADOR
    else
      actual->prob_eventos += 4;
    acciones(actual, selectRe(actual));

    if (incremenTiempEvento(actual)) {
      aux = actual;
      actual = actual->prox;
      // SE ELIMINA EL JUGADOR SI UNA DE SUS REGIONES PERMANECE CON EL MISMO
      // EVENTO POR MAS DE CUATRO RONDAS
      guardarJug(aux->nombre);
    } else
      actual = actual->prox;
    Sleep(2000);
  }
  if (aleat(1, 10) > 7 && JUGADORES != NULL)
    desafio(JUGADORES);
  // SI EXISTEN DOS O MAS JUGADORES VIVOS EN LA RONDA 10, SE LLEGA AL FINAL DEL
  // JUEGO
  if (RONDA == MAX_RONDA) {
    Sleep(1500);
    color(hConsole, 10);
    cout << endl << endl << "EL JUEGO HA FINALIZADO !!" << endl << endl;
    color(hConsole, 15);
    // SE GUARDA REGISTRO DE LOS JUGADORES QUE SOBREVIVIERON
    actual = JUGADORES;
    while (actual != NULL) {
      aux = actual;
      actual = actual->prox;
      guardarJug(aux->nombre);
    }
    Sleep(1500);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CARGAR PARTIDA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// COLOCAR A LA PILA LOS JUGADORES VIVOS GUARDADOS EN EL ARCHIVO
void Apilar(Pila_jugadores **pil, Jugador *jugador_ptr) {
  Pila_jugadores *aux = new Pila_jugadores;
  aux->actual = jugador_ptr;
  aux->prox = *pil;
  *pil = aux;
}

// DESAPILAR PILA DE JUGADORES CARGADOS
void Desapilar(Pila_jugadores **pil) {
  Pila_jugadores *aux = *pil;
  *pil = (*pil)->prox;
  delete aux;
}

// OBTENER EL JUGADOR DEL TOPE DE LA PILA
Jugador *Tope_pila(Pila_jugadores *pil) { return pil->actual; }

// CARGAR LAS REGIONES DE CADA JUGADOR
void cargar_regiones(ifstream &archivo, Region **reg) {
  string cont;
  int numevento;
  int hay_event;
  getline(archivo, cont);
  istringstream ss(cont);
  ss >> (*reg)->tiempevento >> numevento >> (*reg)->voluntarios;
  if (numevento == 0)
    return;
  (*reg)->eventoAc = ObtEvent(LISTA_EVENTOS, numevento);
}

// CARGAR EN EL JUEGO TODOS LOS JUGADORES
void cargar_jugador(ifstream &archivo, Pila_jugadores **pil) {
  Jugador *jugador = new Jugador;
  string cont;
  getline(archivo, cont); // 1
  jugador->nombre = cont; // Se carga el nombre
  getline(archivo, cont); // 2
  istringstream ss(cont);
  ss >> jugador->contnum >> jugador->puntos >> jugador->dinero >>
      jugador->reputacion >> jugador->prob_eventos; // Se cargan ESTADISTICAS
  switch (jugador->contnum) { // Se le atribuye el continente correspiente
  case 1: {
    jugador->contnom = "AMERICA";
    jugador->regiones =
        crearRegiones("America del Norte", "Mesoamerica", "America Central",
                      "America del Sur", "Las Antillas");
    break;
  }

  case 2: {
    jugador->contnom = "EUROPA";
    jugador->regiones =
        crearRegiones("Escandinavia", "Europa Occidental", "Europa Oriental",
                      "Cuenca del Mediterraneo", "Rusia");
    break;
  }

  case 3: {
    jugador->contnom = "ASIA Y OCEANIA";
    jugador->regiones =
        crearRegiones("Medio Oriente", "Asia Meridional", "Asia Oriental",
                      "Sureste de Asia", "Oceania");
    break;
  }

  case 4: {
    jugador->contnom = "AFRICA";
    jugador->regiones =
        crearRegiones("Africa Septentrional", "Africa Occidental",
                      "Africa Central", "Africa Oriental", "Africa Austral");
    break;
  }
  }
  cargar_regiones(archivo, &jugador->regiones);
  cargar_regiones(archivo, &jugador->regiones->proxre);
  cargar_regiones(archivo, &jugador->regiones->proxre->proxre);
  cargar_regiones(archivo, &jugador->regiones->proxre->proxre->proxre);
  cargar_regiones(archivo, &jugador->regiones->proxre->proxre->proxre->proxre);
  Apilar(*&pil, jugador);
}

// ORDENAR LOS JUGADORES CARGADOS EN EL ORDEN CORRESPONDIENTE
void obt_jug_ord(Pila_jugadores **pil) {
  Pila_jugadores *aux = NULL;
  Jugador *aux2;
  while ((*pil) != NULL) {
    if (JUGADORES == NULL) {
      JUGADORES = Tope_pila(*pil);
      JUGADORES->prox = NULL;
    } else {
      aux2 = Tope_pila(*pil);
      aux2->prox = JUGADORES;
      JUGADORES = aux2;
    }
    Desapilar(*&pil);
  }
}

// CARGAR LAS ESTADISTICAS Y PUNTOS DE LOS JUGADORES ELIMINADOS O GUARDADOS
void cargar_puntosfinales(ifstream &archivo, int jugs_guardados) {
  int puntosjug;
  int ultRonda;
  string cont;
  string nomjug;

  while (jugs_guardados > 0) {
    getline(archivo, cont);
    istringstream ss(cont);
    ss >> nomjug >> puntosjug >> ultRonda;
    insertarPuntos(nomjug, puntosjug, ultRonda);
    jugs_guardados--;
  }
}

// SE CARGAN TODOS LOS DATOS DEL ARCHIVO DE PARTIDA
void cargar_partida() {
  string cont;
  int jugs_vivos;
  int jugs_guardados;
  ifstream arc;
  arc.open("PARTIDA.txt", ios::in); //"PARTIDA.txt"
  getline(arc, cont);
  RONDA = stoi(cont);

  getline(arc, cont);
  jugs_vivos = stoi(cont);

  Pila_jugadores *pila = NULL;
  for (int i = 0; i < jugs_vivos; i++) {
    cargar_jugador(arc, &pila);
  }

  getline(arc, cont);
  jugs_guardados = stoi(cont);
  CANT_JUGS = jugs_vivos + jugs_guardados;
  cargar_puntosfinales(arc, jugs_guardados);

  arc.close();
  obt_jug_ord(&pila);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GUARDAR PARTIDA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DEVUELVE LA CANTIDAD DE JUGADORES VIVOS
int jugs_vivos(Jugador *jugs) {
  int vivos = 0;
  while (jugs != NULL) {
    jugs = jugs->prox;
    vivos++;
  }
  return vivos;
}

// ALMACENA EN EL ARCHIVO LAS ESTADISTICAS DE LAS REGIONES
void guardar_reg(ofstream &arch, Region *reg) {
  arch << to_string(reg->tiempevento) + " ";
  if (reg->eventoAc != NULL)
    arch << to_string(reg->eventoAc->EventoID);
  else
    arch << "0";
  arch << " " + to_string(reg->voluntarios);
  arch << "\n";
}

// ALMACENA EN EL ARCHIVO LOS PUNTOS DE LOS JUGADORES ELIMINADOS
void guardar_puntos(ofstream &arch, PuntosFinales *listaPts) {
  int jugs_guardados = CANT_JUGS - jugs_vivos(JUGADORES);
  arch << to_string(jugs_guardados) + "\n";

  while (listaPts != NULL) {
    arch << listaPts->nomjug + " " + to_string(listaPts->puntosjug) + " " +
                to_string(listaPts->ultRonda) + "\n";
    listaPts = listaPts->prox;
  }
}

// ALMACENA TODOS LOS DATOS DE LA PARTIDA EN EL ARCHIVO
void guardar_partida(Jugador *inicio) {
  string cont;
  ofstream arch;
  arch.open("PARTIDA.txt", ios::out);
  arch << to_string(RONDA) + "\n";
  arch << to_string(jugs_vivos(inicio)) + "\n";
  while (inicio != NULL) {
    arch << inicio->nombre + "\n";
    arch << to_string(inicio->contnum) + " " + to_string(inicio->puntos) + " " +
                to_string(inicio->dinero) + " " +
                to_string(inicio->reputacion) + " " +
                to_string(inicio->prob_eventos) + "\n";
    guardar_reg(arch, inicio->regiones);
    guardar_reg(arch, inicio->regiones->proxre);
    guardar_reg(arch, inicio->regiones->proxre->proxre);
    guardar_reg(arch, inicio->regiones->proxre->proxre->proxre);
    guardar_reg(arch, inicio->regiones->proxre->proxre->proxre->proxre);
    inicio = inicio->prox;
  }

  guardar_puntos(arch, LISTA_PUNTOS);
  arch.close();
  color(hConsole, 11);
  cout << endl << "La partida se ha guardado correctamente." << endl << endl;
  Sleep(2000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TOP TEN
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION PARA PASAR EL CONTENIDO DEL ARCHIVO DEL TOP TEN A UNA STRING
string copiarCont(string direc) {
  string datos;

  ifstream arch;
  arch.open("TOPTEN.txt", ios::in); // TOPTEN.txt
  if (arch.fail()) {
    color(hConsole, 14);
    cout << endl << "OCURRIO UN ERROR. EL ARCHIVO DEL TOP TEN NO EXISTE";
    exit(-1);
  }
  getline(arch, datos);
  arch.close();
  return datos;
}

// FUNCION PARA ACTUALIZAR EL TOP TEN AL FINAL DEL JUEGO
void actualizarTop(string datos, PuntosFinales *listPuntos) {
  string nombre;
  int puntos;
  int i = 0;
  ofstream arch;
  arch.open("TOPTEN.txt", ios::out);
  istringstream ss(datos);
  ss >> nombre >> puntos;

  while (i < 10) {
    if (listPuntos != NULL &&
        (listPuntos->puntosjug > puntos || nombre == "null")) {
      arch << listPuntos->nomjug + " " + to_string(listPuntos->puntosjug);
      listPuntos = listPuntos->prox;
    } else {
      arch << nombre + " " + to_string(puntos);
      ss >> nombre >> puntos;
    }
    if (i < 9)
      arch << " ";
    i++;
  }

  arch.close();
}

// FUNCION PARA MOSTRAR EL TOP TEN DE LOS MEJORES JUGADORES
void mostrarTopTen(string datos) {
  int pos = 1;
  int puntos;
  string nombre;
  color(hConsole, 11);
  cout << endl
       << endl
       << endl
       << "Mostrando Top Ten de los mejores jugadores . . ." << endl
       << endl
       << endl;
  Sleep(1250);
  istringstream ss(datos);

  while (pos < 11 && ss) {
    ss >> nombre >> puntos;
    if (nombre == "null") {
      color(hConsole, 14);
      cout << endl
           << "Solo " << (pos - 1)
           << " usuarios han finalizado una partida. Faltan mas jugadores para "
              "completar el top ten"
           << endl;
      break;
    }
    color(hConsole, 10);
    cout << pos << ": ";
    color(hConsole, 15);
    cout << "Jugador ";
    color(hConsole, 11);
    cout << "'" << nombre << "'";
    color(hConsole, 15);
    cout << "con una puntuacion de: ";
    color(hConsole, 11);
    cout << puntos << endl;
    Sleep(750);
    pos++;
  }

  Sleep(750);
  color(hConsole, 15);
  cout << endl
       << endl
       << "Pulse cualquier boton para regresar al menu principal" << endl;
  getch();
  cout << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MENU PRINCIPAL DEL JUEGO
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCION QUE INDICA SI EL ARCHIVO EXISTE O NO

bool No_existe(string ruta) {
  ifstream arc;
  arc.open(ruta, ios::in);
  return arc.fail();
}
// MENU INICIO
void mostrar_menu_inicio() {
  color(hConsole, 10);
  cout << "O))))))))    O))        O))))      O))        O))     O))))      "
          "O)))))))     O))       O))))) "
       << endl;
  cout << "O))       O))   O))   O))    O))   O))        O))   O))    O))   "
          "O))    O))   O))       O))   O))"
       << endl;
  cout << "O))      O))        O))        O)) O))   O)   O)) O))        O)) "
          "O))    O))   O))       O))    O))"
       << endl;
  cout << "O))))))  O))        O))        O)) O))  O))   O)) O))        O)) O) "
          "O))       O))       O))    O)) "
       << endl;
  cout << "O))      O))        O))        O)) O)) O) O)) O)) O))        O)) "
          "O))  O))     O))       O))    O))"
       << endl;
  cout << "O))       O))   O))   O))     O))  O) O)    O))))   O))     O))  "
          "O))    O))   O))       O))   O))"
       << endl;
  cout << "O))))))))   O))))       O))))      O))        O))     O))))      "
          "O))      O)) O)))))))) O))))) "
       << endl
       << endl
       << endl;
  color(hConsole, 15);
  Sleep(1500);
  int input = selecOp(
      "Selecciona una opcion!", "Nuevo Juego.", "Cargar Partida.",
      "Ver top diez de los mejores jugadores.", "Salir :(", 4, NULL, NULL);

  switch (input) {

  case 1:
    if (!No_existe("PARTIDA.txt"))
      remove("PARTIDA.txt");

    Sleep(1000);
    CANT_JUGS = selecOp("Indique cantidad de jugadores (2-4)", "2", "3", "4",
                        "", 3, NULL, NULL);
    CANT_JUGS++;
    JUGADORES = iniciarJugs(CANT_JUGS);
    break;

  case 2:
    if (No_existe("PARTIDA.txt")) {
      color(hConsole, 14);
      cout << endl << "No se ha encontrado un archivo de guardado!" << endl;
      color(hConsole, 15);
      Sleep(1000);
      mostrar_menu_inicio();
      return;
    }
    Sleep(1000);
    cargar_partida();
    break;

  case 3:
    mostrarTopTen(copiarCont("TOPTEN.txt"));
    mostrar_menu_inicio();
    break;

  case 4:
    // UNICA FORMA DE SALIR DEL PROGRAMA (SI EL ARCHIVO TOP TEN SI EXISTE)
    cout << "ADIOS!" << endl;
    SALIR = true;
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
