#include "audio.hpp"
#include "procesos.hpp"

bool grabarAudio() {
    const std::string comando =
        "rm -f \"$HOME/jarvis-son/audio_entrada.m4a\" "
        "\"$HOME/jarvis-son/audio_entrada.wav\" && "
        "termux-microphone-record "
        "-f \"$HOME/jarvis-son/audio_entrada.m4a\" "
        "-l 5 -r 16000 -c 1 -e aac -b 64";

    if (!ejecutarProceso(comando)) {
        return false;
    }

    const std::string espera =
        "for intento in 1 2 3 4 5 6 7 8 9 10 11 12; do "
        "estado=$(termux-microphone-record -i 2>/dev/null); "
        "echo \"$estado\" | grep -q '\"isRecording\": false' "
        "&& exit 0; "
        "sleep 1; "
        "done; "
        "termux-microphone-record -q >/dev/null 2>&1; "
        "exit 1";

    return ejecutarProceso(espera);
}
bool convertirAudio() {
    const std::string comando =
        "ffmpeg -y "
        "-i \"$HOME/jarvis-son/audio_entrada.m4a\" "
        "-ac 1 -ar 16000 -c:a pcm_s16le "
        "\"$HOME/jarvis-son/audio_entrada.wav\"";

    return ejecutarProceso(comando);
}
