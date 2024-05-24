#include "ObtRandomNum.cpp"
#include <string>
using namespace std;
struct Evento {
  string NombreEvento;
  int EventoID;
  string Respuesta;
  Evento *prox;
};

void CrearNodoEvento(Evento **inicio, string nombre, int ID, string respuesta) {
  if (*inicio == NULL) {
    Evento *aux = new Evento;
    aux->NombreEvento = nombre;
    aux->EventoID = ID;
    aux->Respuesta = respuesta;
    *inicio = aux;
    (*inicio)->prox = NULL;
  } else {
    Evento *aux = new Evento;
    aux->NombreEvento = nombre;
    aux->EventoID = ID;
    aux->Respuesta = respuesta;
    aux->prox = *inicio;
    *inicio = aux;
  }
}

Evento *LlenarEventos() { // *Cambiar para cuando se implemente lectura y
                          // escritura de archivos
  Evento *aux = NULL;
  CrearNodoEvento(&aux, "Terremoto", 15,
                  "Construir estructuras resistentes a los terremotos y tener "
                  "un plan de emergencia.");
  CrearNodoEvento(&aux, "Inundacion", 14,
                  "Construir barreras como diques y muros de contencion, y "
                  "reubicar a las personas en zonas seguras.");
  CrearNodoEvento(&aux, "Sequia", 13,
                  "Implementar tecnicas de conservacion del agua y desarrollar "
                  "cultivos resistentes a la sequia.");
  CrearNodoEvento(&aux, "Tormenta de nieve", 12,
                  "Limpiar la nieve de las vias");
  CrearNodoEvento(&aux, "Huracan", 11,
                  "Refugiar a las personas en estructuras resistentes y/o "
                  "debajo de la tierra");
  CrearNodoEvento(&aux, "Desechos Toxicos", 10,
                  "Sanitizar toda la zona y recoger los desechos");
  CrearNodoEvento(&aux, "Tsunami", 9,
                  "Evacuar a las personas a un lugar seguro y rescatar a las "
                  "que se hayan quedado atrapadas por el agua");
  CrearNodoEvento(
      &aux, "Erupcion volcanica", 8,
      "Evacuar a las personas del area de erupcion y del area de cenizas");
  CrearNodoEvento(
      &aux, "Deslizamiento de tierra", 7,
      "Estabilizar las laderas y reubicar a las personas en zonas seguras.");
  CrearNodoEvento(&aux, "Calentamiento global", 6,
                  "Reducir las emisiones de gases de efecto invernadero y "
                  "promover la energia renovable.");
  CrearNodoEvento(&aux, "Perdida de biodiversidad", 5,
                  "Proteger los habitats naturales y promover la conservacion "
                  "de la biodiversidad.");
  CrearNodoEvento(&aux, "Contaminacion del aire", 4,
                  "Reducir las emisiones de contaminantes y promover la "
                  "calidad del aire.");
  CrearNodoEvento(
      &aux, "Contaminacion del agua", 3,
      "Tratar las aguas residuales y promover la conservacion del agua.");
  CrearNodoEvento(
      &aux, "Deforestacion", 2,
      "Plantar arboles y promover la gestion sostenible de los bosques.");
  CrearNodoEvento(&aux, "Apagon", 1, "Proporcionar velas y baterias");
  return aux;
}

Evento *Lista_Eventos = LlenarEventos();

void freeEventList(Evento **inicio) {
  while (*inicio != NULL) {
    Evento *aux = *inicio;
    *inicio = (*inicio)->prox;
    delete aux;
  }
}

Evento *ObtEvent(Evento *inicio, int ID) {
  while (inicio->EventoID != ID) {
    inicio = inicio->prox;
  }
  return inicio;
}

Evento *CrearListaRespuestas(Evento *Correcta, int CorrectaPos) {
  Evento *aux = NULL, *aux2;
  for (int i = 4; i > 0; i--) {
    if (i == CorrectaPos) {
      CrearNodoEvento(&aux, Correcta->NombreEvento, i, Correcta->Respuesta);
    } else {
      aux2 = ObtEvent(Lista_Eventos, aleat(1, 15));
      CrearNodoEvento(&aux, aux2->NombreEvento, i, aux2->Respuesta);
      delete aux2;
    }
  }
  return aux;
}

bool RespuestaAcertada(int RespBuena, int RespSeleccionada) {
  return RespBuena == RespSeleccionada;
}
