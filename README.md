# Jarvis-son V1

**Asistente de voz local para dispositivos Android de recursos limitados.**

Jarvis-son es un proyecto de asistente de voz desarrollado en C++ para Android mediante Termux. Su objetivo es permitir la interacción por voz utilizando modelos de inteligencia artificial ejecutados localmente, reduciendo la dependencia de servicios externos y priorizando el uso eficiente de los recursos del dispositivo.

> **Estado del proyecto:** Prototipo funcional en desarrollo.

## 1. Descripción del proyecto

Jarvis-son implementa un flujo de interacción por turnos:

1. El usuario inicia una grabación de voz.
2. El sistema captura el audio mediante Termux:API.
3. FFmpeg convierte el audio al formato requerido.
4. Whisper.cpp transforma el audio en texto.
5. Un modelo de lenguaje local procesa la consulta y genera una respuesta.
6. Termux:API reproduce la respuesta mediante síntesis de voz.

El procesamiento se realiza de manera secuencial mediante un programa principal desarrollado en C++.

## 2. Objetivos

### Objetivo general

Desarrollar un asistente de voz local para Android, capaz de reconocer instrucciones habladas, procesarlas mediante un modelo de lenguaje pequeño y responder utilizando síntesis de voz.

### Objetivos específicos

- Implementar captura de audio mediante Termux:API.
- Integrar reconocimiento de voz con Whisper.cpp.
- Ejecutar un modelo de lenguaje local mediante llama.cpp.
- Incorporar síntesis de voz en español.
- Coordinar los procesos mediante un programa desarrollado en C++.
- Evaluar el funcionamiento y consumo de recursos en dispositivos móviles.

## 3. Arquitectura del sistema

La arquitectura actual utiliza los siguientes componentes:

| Componente | Tecnología | Función |
|---|---|---|
| Orquestador | C++ | Coordina el flujo de ejecución |
| Captura de audio | Termux:API | Graba la voz del usuario |
| Procesamiento de audio | FFmpeg | Convierte el audio a WAV |
| Reconocimiento de voz | Whisper.cpp | Convierte voz a texto |
| Modelo de lenguaje | llama.cpp | Ejecuta el modelo local |
| Modelo SLM | Qwen3.5-0.8B Q4_0 | Genera respuestas |
| Síntesis de voz | Termux TTS | Reproduce las respuestas |

### Flujo de procesamiento

```text
        USUARIO
           |
           v
    Captura de audio
      Termux:API
           |
           v
    Conversión de audio
         FFmpeg
           |
           v
    Reconocimiento de voz
       Whisper.cpp
           |
           v
     Texto transcrito
           |
           v
    Procesamiento local
        llama.cpp
           |
           v
      Qwen3.5-0.8B
           |
           v
     Respuesta generada
           |
           v
     Síntesis de voz
        Termux TTS
           |
           v
        USUARIO
