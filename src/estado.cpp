#include "estado.hpp"

std::string obtenerNombreEstado(Estado estado) {
    switch (estado) {
        case Estado::LISTO: return "LISTO";
        case Estado::GRABANDO: return "GRABANDO";
        case Estado::TRANSCRIBIENDO: return "TRANSCRIBIENDO";
        case Estado::GENERANDO: return "GENERANDO";
        case Estado::HABLANDO: return "HABLANDO";
        case Estado::ERROR: return "ERROR";
        default: return "DESCONOCIDO";
    }
}

void cambiarEstado(Estado& estado, Estado nuevoEstado) {
    estado = nuevoEstado;
}
bool esEstado(Estado actual, Estado esperado) {
    return actual == esperado;
}
