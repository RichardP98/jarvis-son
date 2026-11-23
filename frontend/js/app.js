
const micButton = document.getElementById("micButton");
const micLabel = document.getElementById("micLabel");
const statusText = document.getElementById("status");
const orb = document.getElementById("orb");
const chat = document.getElementById("chat");
const messageCount = document.getElementById("messageCount");

let totalMensajes = 0;
let procesando = false;

// Actualizar el estado visual
function cambiarEstado(estado, mensaje) {
    statusText.textContent = mensaje;
    micLabel.textContent = estado;

    if (procesando) {
        orb.classList.add("processing");
    } else {
        orb.classList.remove("processing");
    }
}

// Agregar un mensaje al historial
function agregarMensaje(tipo, autor, contenido) {
    const bienvenida = chat.querySelector(".welcome");

    if (bienvenida) {
        bienvenida.remove();
    }

    const mensaje = document.createElement("div");
    mensaje.className = `message ${tipo}`;

    const nombre = document.createElement("strong");
    nombre.textContent = autor;

    const texto = document.createElement("p");
    texto.textContent = contenido;

    mensaje.appendChild(nombre);
    mensaje.appendChild(texto);

    chat.appendChild(mensaje);
    chat.scrollTop = chat.scrollHeight;

    totalMensajes++;
    messageCount.textContent =
        `${totalMensajes} mensajes`;
}

// Iniciar interacción con Jarvis-son
async function iniciarInteraccion() {
    if (procesando) return;

    procesando = true;
    micButton.disabled = true;
    micButton.textContent = "⏳";

    cambiarEstado("PROCESANDO", "Iniciando captura de audio...");

    try {
        const respuesta = await fetch("/api/interact", {
            method: "POST"
        });

        const datos = await respuesta.json();

        if (!respuesta.ok) {
            throw new Error(datos.error || "Error de comunicación");
        }

        if (datos.usuario) {
            agregarMensaje(
                "user",
                "TÚ",
                datos.usuario
            );
        }

        if (datos.respuesta) {
            agregarMensaje(
                "jarvis",
                "JARVIS-SON",
                datos.respuesta
            );
        }

        cambiarEstado("LISTO", "Estoy listo para escucharte.");

    } catch (error) {
        agregarMensaje(
            "jarvis",
            "SISTEMA",
            error.message
        );

        cambiarEstado("ERROR", "Ocurrió un problema.");
    } finally {
        procesando = false;
        micButton.disabled = false;
        micButton.textContent = "🎙";
        orb.classList.remove("processing");
        micLabel.textContent = "PRESIONA PARA HABLAR";
    }
}

// Evento del botón
micButton.addEventListener("click", iniciarInteraccion);
