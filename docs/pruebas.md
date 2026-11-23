# Plan y registro de pruebas – Jarvis-son V1

## 1. Introducción

Este documento describe las pruebas funcionales y técnicas definidas para evaluar Jarvis-son V1.

El objetivo es verificar el funcionamiento individual de sus componentes y la integración del flujo completo de voz a voz en un dispositivo Android mediante Termux.

Las pruebas también permitirán identificar limitaciones de rendimiento, consumo de memoria y estabilidad.

## 2. Objetivos

- Verificar la captura de audio mediante Termux:API.
- Comprobar la conversión de audio mediante FFmpeg.
- Evaluar el reconocimiento de voz con Whisper.cpp.
- Comprobar la generación de respuestas mediante llama.cpp y Qwen.
- Validar la reproducción de respuestas mediante TTS.
- Verificar la integración de todos los componentes.
- Evaluar el funcionamiento sin conexión a Internet.
- Registrar errores, latencias y consumo de recursos.

## 3. Entorno de pruebas

El entorno de referencia utilizado durante el desarrollo es:

| Característica | Descripción |
|---|---|
| Sistema operativo | Android 17 |
| Dispositivo de pruebas | Samsung Galaxy A36 5G |
| Procesador | SM6475 |
| Arquitectura | ARM64 |
| Memoria RAM | 8 GB |
| Almacenamiento | 256 GB |
| Entorno de ejecución | Termux |
| Lenguaje | C++ |

**Consideración:** el dispositivo de pruebas dispone de más memoria que el objetivo de referencia del proyecto, establecido en 3 GB de RAM. Por tanto, las pruebas deberán repetirse en un dispositivo con recursos más limitados para validar la viabilidad de la arquitectura.

## 4. Componentes evaluados

| Componente | Herramienta | Aspecto evaluado |
|---|---|---|
| Captura | Termux:API | Grabación de audio |
| Conversión | FFmpeg | Formato y características del WAV |
| STT | Whisper.cpp Tiny | Calidad de transcripción |
| SLM | Qwen3.5-0.8B Q4_0 | Generación de respuestas |
| TTS | Termux TTS | Reproducción de voz |
| Orquestación | C++ | Integración y control de estados |

## 5. Pruebas funcionales

### 5.1. Prueba de captura de audio

**Objetivo:** comprobar que el sistema puede capturar audio mediante el micrófono del dispositivo.

Procedimiento:

1. Ejecutar Jarvis-son.
2. Conceder los permisos de micrófono cuando sean solicitados.
3. Iniciar una interacción.
4. Verificar que se genere el archivo de audio.
5. Comprobar que la grabación tenga una duración aproximada de cinco segundos.

Criterio de aceptación:

- El sistema debe generar correctamente el archivo de audio.
- La grabación debe contener la voz del usuario.
- No deben presentarse errores de permisos.

### 5.2. Prueba de conversión de audio

**Objetivo:** verificar que FFmpeg convierta correctamente el audio capturado.

Procedimiento:

1. Capturar un archivo de audio.
2. Ejecutar la función de conversión.
3. Verificar la existencia del archivo WAV.
4. Consultar sus características mediante FFprobe.

Comando de verificación:

```bash
ffprobe -v error -show_streams audio_entrada.wav
