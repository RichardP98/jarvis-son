
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
#include <cctype>

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

// Identificar preguntas que puede responder localmente
bool obtenerRespuestaLocal(
    const string& texto,
    string& respuesta
) {
    string pregunta = texto;

    transform(
        pregunta.begin(),
        pregunta.end(),
        pregunta.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        }
    );

    bool preguntaTecnologia =
        pregunta.find("tecnolog") != string::npos ||
        (
            pregunta.find("escuchar") != string::npos &&
            pregunta.find("responder") != string::npos
        ) ||
        pregunta.find("como procesas mi voz") != string::npos;

    if (preguntaTecnologia) {
        respuesta =
            "Utilizo Whisper.cpp con el modelo Whisper Tiny "
            "para convertir tu audio en texto. "
            "Qwen 3.5 de 0.8B, mediante llama.cpp, "
            "genera mis respuestas. "
            "Termux TTS convierte esas respuestas en voz "
            "en espanol y FFmpeg prepara el audio "
            "antes de la transcripcion.";

        return true;
    }

    return false;
}

// Guardar y reproducir una respuesta
bool guardarYReproducir(const string& respuesta) {
    string ruta = string(getenv("HOME")) +
                  "/jarvis-son/respuesta.txt";

    ofstream archivo(ruta);

    if (!archivo.is_open()) {
        return false;
    }

    archivo << respuesta << endl;
    archivo.close();

    const string comando =
        "termux-tts-speak -l es "
        "\"$(cat $HOME/jarvis-son/respuesta.txt)\"";

    return ejecutarProceso(comando);
}

// Generar respuesta con Qwen y reproducirla
bool generarRespuesta(const string& texto) {

    // Primero comprobar si existe una respuesta local
    string respuestaLocal;

    if (obtenerRespuestaLocal(texto, respuestaLocal)) {
        cout << "\nRespuesta local de Jarvis-son:"
             << endl;

        cout << respuestaLocal << endl;

        return guardarYReproducir(respuestaLocal);
    }

    // Si no existe una respuesta local, utilizar Qwen
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

    archivo << "Eres Jarvis-son, un asistente virtual de voz "
            << "que funciona localmente en Android.\n"
            << "Responde en espanol latinoamericano.\n"
            << "Instrucciones:\n"
            << "1. Contesta directamente la pregunta del usuario.\n"
            << "2. No saludes ni repitas la pregunta.\n"
            << "3. Usa respuestas breves, maximo tres oraciones.\n"
            << "4. Tu arquitectura utiliza whisper.cpp con "
            << "Whisper Tiny para transcribir audio, "
            << "Qwen3.5-0.8B mediante llama.cpp para generar "
            << "respuestas y Termux TTS para producir voz.\n"
            << "5. No confundas las funciones de estas tecnologias.\n"
            << "6. No inventes funciones ni tecnologias.\n"
            << "7. Si no sabes algo, dilo claramente.\n"
            << "Fecha y hora del dispositivo: "
            << fechaHora << ".\n"
            << "Usa estos datos cuando corresponda. "
            << "No inventes fechas ni horas.\n\n"
            << "Pregunta del usuario: " << texto << "\n"
            << "Respuesta:";

    archivo.close();

    const string comando =
        "rm -f $HOME/jarvis-son/respuesta.txt "
        "$HOME/jarvis-son/respuesta_completa.txt && "

        "$HOME/llama.cpp/build/bin/llama-cli "
        "-m $HOME/modelos/Qwen3.5-0.8B-Q4_0.gguf "
        "-f $HOME/jarvis-son/prompt.txt "
        "-n 60 -c 1024 -t 3 "
        "--reasoning off "
        "--single-turn --simple-io "
        "--no-display-prompt "
        "-o $HOME/jarvis-son/respuesta_completa.txt "
        "&& "

        "sed -n '/^Assistant:/,$p' "
        "$HOME/jarvis-son/respuesta_completa.txt | "
        "sed '1d' "
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

                    cout << "\nGenerando respuesta..."
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
