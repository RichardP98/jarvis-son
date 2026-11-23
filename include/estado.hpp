#ifndef ESTADO_HPP
#define ESTADO_HPP

#include <string>

enum class Estado {
    LISTO,
    GRABANDO,
    TRANSCRIBIENDO,
    GENERANDO,
    HABLANDO,
    ERROR
};

std::string obtenerNombreEstado(Estado estado);

void cambiarEstado(Estado& estado, Estado nuevoEstado);
bool esEstado(Estado actual, Estado esperado);
#endif
