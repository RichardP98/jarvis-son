from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
import subprocess
import json
import threading

BASE_DIR = Path.home() / "jarvis-son"
FRONTEND_DIR = BASE_DIR / "frontend"

ejecutando = False
bloqueo = threading.Lock()


class JarvisHandler(SimpleHTTPRequestHandler):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(FRONTEND_DIR), **kwargs)

    def responder_json(self, codigo, datos):
        contenido = json.dumps(datos).encode("utf-8")

        self.send_response(codigo)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(contenido)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(contenido)

    def do_POST(self):
        global ejecutando

        if self.path != "/api/interact":
            self.responder_json(404, {"error": "Ruta no encontrada"})
            return

        with bloqueo:
            if ejecutando:
                self.responder_json(
                    409,
                    {"error": "Jarvis-son ya esta procesando una solicitud"}
                )
                return

            ejecutando = True

        try:
            proceso = subprocess.run(
                [str(BASE_DIR / "jarvis")],
                cwd=str(BASE_DIR),
                capture_output=True,
                text=True,
                timeout=180
            )

            transcripcion = BASE_DIR / "transcripcion.txt"
            respuesta = BASE_DIR / "respuesta.txt"

            texto_usuario = (
                transcripcion.read_text(encoding="utf-8").strip()
                if transcripcion.exists() else ""
            )

            texto_respuesta = (
                respuesta.read_text(encoding="utf-8").strip()
                if respuesta.exists() else ""
            )

            if proceso.returncode != 0 or not texto_respuesta:
                self.responder_json(500, {
                    "error": "No se pudo completar la interaccion",
                    "detalle": proceso.stderr[-1500:]
                })
                return

            self.responder_json(200, {
                "usuario": texto_usuario,
                "respuesta": texto_respuesta,
                "estado": "LISTO"
            })

        except subprocess.TimeoutExpired:
            self.responder_json(504, {
                "error": "La solicitud supero el tiempo limite"
            })

        except Exception as error:
            self.responder_json(500, {
                "error": str(error)
            })

        finally:
            ejecutando = False


if __name__ == "__main__":
    servidor = ThreadingHTTPServer(
        ("127.0.0.1", 8000),
        JarvisHandler
    )

    print("================================")
    print("       JARVIS-SON WEB")
    print("================================")
    print("Servidor disponible en:")
    print("http://127.0.0.1:8000")
    print("Presiona CTRL + C para detenerlo")

    servidor.serve_forever()
