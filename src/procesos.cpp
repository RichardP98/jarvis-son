#include "procesos.hpp"
#include <cstdlib>
#include <sys/wait.h>

bool ejecutarProceso(const std::string& comando) {
    int resultado = system(comando.c_str());

    if (resultado == -1) {
        return false;
    }

    if (WIFEXITED(resultado)) {
        return WEXITSTATUS(resultado) == 0;
    }

    return false;
}
