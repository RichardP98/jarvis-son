#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "procesos.hpp"
#include "estado.hpp"
#include "audio.hpp"

using namespace std;

// Leer transcripcion generada por Whisper
bool leerTranscripcion(string& texto) {
    string ruta = string(getenv("HOME")) +
                  "/jarvis-son/transcripcion.txt";

    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        return false;
    }

    getline(archivo, texto);

    return !texto.empty();
}

// Obtener fecha y hora del dispositivo Android
string obtenerFechaHora() {
    string resultado;
    char buffer[128];

    FILE* proceso = popen(
        "date '+%A %d de %B de %Y, %H:%M'",
        "r"
    );

    if (proceso == nullptr) {
        return "";
    }

    while (fgets(buffer, sizeof(buffer), proceso) != nullptr) {
        resultado += buffer;
    }

    pclose(proceso);

    return resultado;
}

// Generar respuesta con Qwen y reproducirla
bool generarRespuesta(const string& texto) {
    string fechaHora = obtenerFechaHora();

    if (fechaHora.empty()) {
        return false;
    }

    string ruta = string(getenv("HOME")) +
                  "/jarvis-son/prompt.txt";

    ofstream archivo(ruta);

    if (!archivo.is_open()) {
        return false;
    }

    archivo << "Eres Jarvis-son, un asistente de voz. "
            << "Responde en espanol latinoamericano, "
            << "de forma breve y clara.\n"
            << "La fecha y hora actual del dispositivo es: "
            << fechaHora << ".\n"
            << "Utiliza esta informacion para responder "
            << "preguntas sobre la fecha y hora. "
            << "No inventes fechas ni horas.\n\n"
            << "Usuario: " << texto << "\n"
            << "Jarvis-son:";

    archivo.close();

    const string comando =
        "$HOME/llama.cpp/build/bin/llama-cli "
        "-m $HOME/modelos/Qwen3.5-0.8B-Q4_0.gguf "
        "-f $HOME/jarvis-son/prompt.txt "
        "-n 96 -c 1024 -t 3 --reasoning off "
        "--single-turn --simple-io "
        "2>&1 | tee $HOME/jarvis-son/respuesta_completa.txt && "
        "awk '/Jarvis-son:/{captura=1; next} "
        "/^\\[ Prompt:/{captura=0} "
        "captura {print}' "
        "$HOME/jarvis-son/respuesta_completa.txt "
        "> $HOME/jarvis-son/respuesta.txt && "
        "test -s $HOME/jarvis-son/respuesta.txt && "
        "termux-tts-speak -l es "
        "\"$(cat $HOME/jarvis-son/respuesta.txt)\"";

    return ejecutarProceso(comando);
}

// Programa principal
int main() {
    Estado estado = Estado::LISTO;

    cout << "\n================================" << endl;
    cout << "       INICIANDO JARVIS-SON" << endl;
    cout << "================================" << endl;

    cout << "\nIniciando captura de audio..." << endl;

    cambiarEstado(estado, Estado::GRABANDO);

    if (grabarAudio()) {
        cout << "Grabacion completada." << endl;

        cout << "\nConvirtiendo audio a WAV..." << endl;

        if (convertirAudio()) {
            cout << "Audio convertido correctamente." << endl;

            cout << "\nTranscribiendo audio con Whisper..."
                 << endl;

            cambiarEstado(estado, Estado::TRANSCRIBIENDO);

            string rutaTranscripcion =
                string(getenv("HOME")) +
                "/jarvis-son/transcripcion.txt";

            remove(rutaTranscripcion.c_str());

            bool transcripcion = ejecutarProceso(
                "$HOME/whisper.cpp/build/bin/whisper-cli "
                "-m $HOME/whisper.cpp/models/ggml-tiny.bin "
                "-f $HOME/jarvis-son/audio_entrada.wav "
                "-l es -t 3 -nt -otxt "
                "-of $HOME/jarvis-son/transcripcion"
            );

            if (transcripcion) {
                cout << "\nTranscripcion completada." << endl;

                string texto;

                if (leerTranscripcion(texto)) {
                    cout << "Texto reconocido: "
                         << texto << endl;

                    cout << "\nGenerando respuesta con Qwen..."
                         << endl;

                    cambiarEstado(estado, Estado::GENERANDO);

                    if (generarRespuesta(texto)) {
                        cout << "\nRespuesta generada y reproducida."
                             << endl;

                        cambiarEstado(estado, Estado::HABLANDO);

                    } else {
                        cout << "\nError al generar o reproducir "
                             << "la respuesta." << endl;

                        cambiarEstado(estado, Estado::ERROR);
                    }

                } else {
                    cout << "No se pudo leer la transcripcion."
                         << endl;

                    cambiarEstado(estado, Estado::ERROR);
                }

            } else {
                cout << "Error al transcribir el audio." << endl;

                cambiarEstado(estado, Estado::ERROR);
            }

        } else {
            cout << "Error al convertir el audio." << endl;

            cambiarEstado(estado, Estado::ERROR);
        }

    } else {
        cout << "Error durante la grabacion." << endl;

        cambiarEstado(estado, Estado::ERROR);
    }

    if (!esEstado(estado, Estado::ERROR)) {
        cambiarEstado(estado, Estado::LISTO);
    }

    cout << "\n================================" << endl;
    cout << "          JARVIS-SON V1" << endl;
    cout << "================================" << endl;

    cout << "Estado final: "
         << obtenerNombreEstado(estado) << endl;

    return 0;
}
