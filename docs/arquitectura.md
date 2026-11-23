# Arquitectura técnica – Jarvis-son V1

## 1. Introducción

Jarvis-son V1 es un asistente de voz local desarrollado en C++ para dispositivos Android mediante Termux.

Su arquitectura está diseñada para integrar diferentes herramientas de inteligencia artificial en un flujo secuencial de procesamiento de voz. El sistema recibe una instrucción hablada, la convierte en texto, procesa la consulta mediante un modelo de lenguaje pequeño (SLM) y genera una respuesta hablada.

La arquitectura prioriza el procesamiento local, la modularidad y el uso controlado de recursos del dispositivo.

## 2. Principios de diseño

La arquitectura sigue los siguientes principios:

- **Procesamiento local:** utilizar modelos ejecutados directamente en el dispositivo.
- **Ejecución secuencial:** completar cada etapa antes de iniciar la siguiente.
- **Modularidad:** separar captura, procesamiento de audio, reconocimiento, generación y reproducción.
- **Optimización:** utilizar modelos cuantizados y limitar el contexto y la generación de tokens.
- **Control de errores:** verificar el resultado de los procesos externos.
- **Privacidad:** evitar almacenar conversaciones personales de manera permanente.

## 3. Arquitectura general

El sistema está compuesto por cinco bloques principales:

| Bloque | Tecnología | Responsabilidad |
|---|---|---|
| Captura | Termux:API | Obtener el audio del usuario |
| Normalización | FFmpeg | Convertir el audio al formato requerido |
| Reconocimiento (STT) | Whisper.cpp | Convertir voz en texto |
| Generación (SLM) | llama.cpp + Qwen | Procesar la consulta y generar una respuesta |
| Síntesis (TTS) | Termux TTS | Convertir la respuesta en voz |

La coordinación de estos componentes se realiza mediante un programa principal desarrollado en C++.

## 4. Diagrama de arquitectura

```text
+---------------------------+
|          USUARIO          |
|       Entrada de voz      |
+-------------+-------------+
              |
              v
+---------------------------+
|       TERMUX:API          |
|      Captura de audio     |
+-------------+-------------+
              |
              v
+---------------------------+
|          FFMPEG           |
|   Normalización de audio  |
|   WAV - PCM16 - 16 kHz    |
|           Mono            |
+-------------+-------------+
              |
              v
+---------------------------+
|        WHISPER.CPP        |
|   Reconocimiento de voz   |
|       Modelo Tiny         |
+-------------+-------------+
              |
              v
+---------------------------+
|      TRANSCRIPCION        |
|       Texto en español    |
+-------------+-------------+
              |
              v
+---------------------------+
|       ORQUESTADOR C++     |
|   Construcción del prompt |
+-------------+-------------+
              |
              v
+---------------------------+
|         LLAMA.CPP         |
|    Inferencia del modelo  |
|       Qwen3.5 0.8B        |
+-------------+-------------+
              |
              v
+---------------------------+
|     RESPUESTA EN TEXTO    |
+-------------+-------------+
              |
              v
+---------------------------+
|        TERMUX TTS         |
|     Síntesis de voz       |
+-------------+-------------+
              |
              v
+---------------------------+
|          USUARIO          |
|     Respuesta hablada     |
+---------------------------+
