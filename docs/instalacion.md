# Guía de instalación – Jarvis-son V1

## 1. Introducción

Esta guía describe los requisitos y procedimientos necesarios para preparar el entorno de ejecución de Jarvis-son V1 en un dispositivo Android utilizando Termux.

El proyecto utiliza herramientas de procesamiento de audio, reconocimiento de voz, inferencia de lenguaje y síntesis de voz.

## 2. Requisitos del sistema

### 2.1. Hardware

| Recurso | Requisito |
|---|---|
| Sistema operativo | Android 13 o superior |
| Arquitectura | ARM64 |
| Memoria RAM objetivo | 3 GB |
| Almacenamiento | Espacio suficiente para herramientas y modelos |
| Micrófono | Requerido |

La arquitectura se puede comprobar desde Termux con:

```bash
uname -m
