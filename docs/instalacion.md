Guía de instalación – Jarvis-son V1

1. Introducción

Esta guía describe los requisitos y procedimientos necesarios para instalar, configurar, compilar y ejecutar Jarvis-son V1, un asistente de voz local desarrollado en C++ para dispositivos Android mediante Termux.

El sistema permite realizar interacciones por turnos mediante voz. Su funcionamiento integra reconocimiento automático del habla (STT), generación de respuestas mediante un modelo de lenguaje pequeño (SLM) y síntesis de voz (TTS).

La arquitectura está diseñada para ejecutar los componentes localmente, sin depender de servicios externos durante el procesamiento. Sin embargo, la disponibilidad de funcionamiento completamente sin conexión debe comprobarse mediante pruebas en modo avión.

2. Requisitos del sistema

2.1. Requisitos de hardware

ComponenteRequisitoDispositivoTeléfono AndroidArquitecturaARM64AndroidAndroid 13 o superiorMemoria RAM3 GB como mínimo previstoAlmacenamientoEspacio suficiente para modelos, dependencias y compilacionesMicrófonoIntegrado en el dispositivoSalida de audioAltavoz o auriculares

Dispositivo utilizado durante el desarrollo y las pruebas:

Modelo: Samsung Galaxy A36 5G.

Procesador: Qualcomm SM6475.

Memoria RAM: 8 GB.

Almacenamiento: 256 GB.

Sistema operativo: Android 17.

Las pruebas realizadas en este dispositivo no representan necesariamente el rendimiento que se obtendrá en teléfonos con 3 GB de RAM.

2.2. Requisitos de software

Android.

Termux.

Termux:API.

Clang.

CMake.

Git.

Make.

FFmpeg.

whisper.cpp.

llama.cpp.

3. Instalación del entorno Termux

3.1. Instalar Termux

Instalar Termux desde una fuente oficial, preferiblemente F-Droid o el repositorio oficial del proyecto.

Repositorio:

https://github.com/termux/termux-app

Se recomienda instalar Termux:API desde la misma fuente de distribución que Termux.

3.2. Actualizar los paquetes

Abrir Termux y ejecutar:

pkg update && pkg upgrade -y

3.3. Instalar las dependencias

Ejecutar:

pkg install -y clang cmake git make pkg-config ffmpeg termux-api

Estas herramientas cumplen las siguientes funciones:

HerramientaFunciónClangCompilación del código C++CMakeConfiguración de compilacionesGitControl de versionesMakeAutomatización de compilacionespkg-configGestión de información de bibliotecasFFmpegConversión y procesamiento de audioTermux:APIAcceso a funciones del dispositivo Android

3.4. Verificar las herramientas

Comprobar las versiones instaladas:

clang++ --version cmake --version git --version ffmpeg -version

También se debe comprobar que Termux:API esté disponible:

termux-microphone-record -h

Es necesario conceder los permisos de micrófono correspondientes desde Android.

4. Obtener el proyecto

Clonar el repositorio de GitHub:

git clone https://github.com/RichardP98/jarvis-son.git

Ingresar al directorio:

cd jarvis-son

Comprobar la estructura del proyecto:

ls -la

La estructura principal es:

jarvis-son/ ├── docs/ │ ├── arquitectura.md │ ├── instalacion.md │ └── pruebas.md ├── include/ │ ├── audio.hpp │ ├── estado.hpp │ └── procesos.hpp ├── models/ ├── scripts/ ├── src/ │ ├── audio.cpp │ ├── estado.cpp │ ├── main.cpp │ └── procesos.cpp ├── tests/ ├── .gitignore └── README.md

5. Instalación de los motores de procesamiento

Jarvis-son utiliza dos motores principales: whisper.cpp para reconocimiento de voz y llama.cpp para generación de respuestas.

5.1. Instalar whisper.cpp

Clonar el repositorio:

cd ~ git clone https://github.com/ggml-org/whisper.cpp.git

Ingresar al directorio:

cd ~/whisper.cpp

Crear el directorio de compilación:

cmake -B build

Compilar:

cmake --build build --config Release -j2

El ejecutable utilizado por Jarvis-son es:

~/whisper.cpp/build/bin/whisper-cli

5.2. Descargar el modelo de reconocimiento de voz

Jarvis-son utiliza el modelo Whisper Tiny multilingüe.

El archivo utilizado es:

ggml-tiny.bin

Ubicación:

~/whisper.cpp/models/ggml-tiny.bin

El modelo debe obtenerse desde las fuentes oficiales de whisper.cpp y estar disponible antes de ejecutar el asistente.

5.3. Instalar llama.cpp

Clonar el repositorio:

cd ~ git clone https://github.com/ggml-org/llama.cpp.git

Ingresar al directorio:

cd ~/llama.cpp

Configurar la compilación:

cmake -B build

Compilar:

cmake --build build --config Release -j2

El ejecutable utilizado por Jarvis-son es:

~/llama.cpp/build/bin/llama-cli

5.4. Descargar el modelo de lenguaje

El modelo utilizado es:

CaracterísticaConfiguraciónModeloQwen3.5-0.8BFormatoGGUFCuantizaciónQ4_0ArchivoQwen3.5-0.8B-Q4_0.ggufContexto configurado1024 tokensMáxima salida configurada96 tokensHilos utilizados3

Ubicación del archivo:

~/modelos/Qwen3.5-0.8B-Q4_0.gguf

Crear el directorio si no existe:

mkdir -p ~/modelos

El archivo del modelo debe descargarse desde una fuente que distribuya la versión GGUF correspondiente. Antes de utilizarlo, se debe comprobar su licencia y procedencia.

Importante: los modelos no se almacenan directamente en el repositorio de GitHub debido a su tamaño. Se mantienen localmente en el dispositivo.

6. Configuración de las rutas

Jarvis-son utiliza las siguientes rutas locales:

RecursoRutaProyecto~/jarvis-sonModelo Whisper~/whisper.cpp/models/ggml-tiny.binEjecutable Whisper~/whisper.cpp/build/bin/whisper-cliModelo Qwen~/modelos/Qwen3.5-0.8B-Q4_0.ggufEjecutable llama.cpp~/llama.cpp/build/bin/llama-cli

Es importante mantener estas rutas o adaptar las referencias en el código fuente si se modifica la ubicación de los archivos.

7. Compilación de Jarvis-son

Ingresar al directorio del proyecto:

cd ~/jarvis-son

Ejecutar el siguiente comando para compilar los archivos fuente:

clang++ -std=c++17 -Wall -Wextra -O2 \ -I ~/jarvis-son/include \ ~/jarvis-son/src/main.cpp \ ~/jarvis-son/src/procesos.cpp \ ~/jarvis-son/src/estado.cpp \ ~/jarvis-son/src/audio.cpp \ -o ~/jarvis-son/jarvis

7.1. Descripción de los parámetros

ParámetroDescripción-std=c++17Utiliza el estándar C++17-WallActiva advertencias comunes-WextraActiva advertencias adicionales-O2Aplica optimizaciones de compilación-IIndica la ubicación de los archivos de cabecera-oDefine el nombre y ubicación del ejecutable

Si la compilación finaliza correctamente, se genera el ejecutable:

~/jarvis-son/jarvis

8. Permisos y configuración de Android

Para utilizar las funciones de audio es necesario:

Tener instalada la aplicación Termux:API.

Conceder permisos de micrófono a Termux.

Permitir el acceso al micrófono desde Android.

Comprobar que el dispositivo tenga un motor de síntesis de voz disponible.

Verificar que el volumen multimedia esté habilitado.

Para comprobar la síntesis de voz:

termux-tts-speak -l es "Hola, soy Jarvis-son"

La síntesis utilizada actualmente por el proyecto se realiza mediante Termux TTS.

La disponibilidad de un motor de voz y su funcionamiento sin conexión dependen de la configuración del dispositivo Android.

9. Ejecución del asistente

Una vez instaladas las dependencias, descargados los modelos y compilado el proyecto, ingresar al directorio:

cd ~/jarvis-son

Ejecutar:

./jarvis

El asistente inicia el proceso de interacción por turnos.

9.1. Flujo de ejecución

El funcionamiento general sigue esta secuencia:

LISTO: el asistente se encuentra preparado para recibir una interacción.

GRABANDO: captura el audio del usuario mediante Termux:API.

TRANSCRIBIENDO: convierte el audio a texto utilizando whisper.cpp.

GENERANDO: procesa la transcripción con el modelo Qwen mediante llama.cpp.

HABLANDO: reproduce la respuesta mediante síntesis de voz.

LISTO: finaliza el turno y queda preparado para una nueva interacción.

Si ocurre un error durante alguna etapa, el sistema utiliza el estado ERROR.

10. Archivos temporales

Durante el procesamiento se generan archivos temporales que permiten intercambiar información entre los componentes.

Entre ellos se encuentran:

ArchivoFunciónaudio_entrada.wavAudio convertido a WAVtranscripcion.txtTexto reconocido por Whisperprompt.txtInstrucciones enviadas al modelorespuesta.txtRespuesta procesada del modelorespuesta_completa.txtSalida completa de llama.cpp

Estos archivos se generan durante la ejecución y no deben publicarse en el repositorio.

El proyecto utiliza reglas en .gitignore para excluir archivos temporales, grabaciones y modelos.

11. Consideraciones de funcionamiento

Jarvis-son funciona mediante interacciones iniciadas por el usuario.

La versión actual no implementa una palabra de activación permanente.

No mantiene escucha continua.

No permite interrumpir una respuesta mientras se reproduce.

No ejecuta acciones sensibles del sistema operativo.

La calidad del reconocimiento depende del ruido ambiental y del micrófono.

La velocidad de generación depende del procesador, la memoria disponible y la cuantización del modelo.

El funcionamiento completamente sin conexión debe verificarse mediante pruebas específicas en modo avión.

La síntesis de voz de Android puede depender de un motor instalado y de sus recursos disponibles.

12. Solución de problemas

12.1. Error al capturar audio

Comprobar que Termux:API esté instalada y que Termux tenga permiso para utilizar el micrófono.

12.2. No se encuentra whisper-cli

Comprobar que el ejecutable exista:

ls -lh ~/whisper.cpp/build/bin/whisper-cli

12.3. No se encuentra llama-cli

Comprobar:

ls -lh ~/llama.cpp/build/bin/llama-cli

12.4. No se encuentra el modelo de Whisper

Comprobar:

ls -lh ~/whisper.cpp/models/ggml-tiny.bin

12.5. No se encuentra el modelo Qwen

Comprobar:

ls -lh ~/modelos/Qwen3.5-0.8B-Q4_0.gguf

12.6. No se reproduce la respuesta

Probar el motor de voz de Android:

termux-tts-speak -l es "Prueba de audio"

Comprobar el volumen multimedia y la configuración del motor de síntesis de voz.

12.7. Error durante la compilación

Verificar que las dependencias estén instaladas, que los archivos fuente existan y que se esté ejecutando el comando desde una instalación completa del proyecto.

13. Privacidad y seguridad

Jarvis-son está diseñado para procesar las interacciones localmente.

Se deben tener en cuenta las siguientes recomendaciones:

No subir modelos de lenguaje ni archivos binarios pesados a GitHub.

No publicar grabaciones de voz ni transcripciones personales.

No almacenar contraseñas, tokens o credenciales en el código fuente.

No ejecutar comandos del sistema construidos directamente a partir de transcripciones del usuario.

Eliminar los archivos temporales cuando ya no sean necesarios.

Verificar la procedencia y licencia de los modelos utilizados.

14. Repositorio del proyecto

El código fuente y la documentación se encuentran disponibles en GitHub:

Repositorio:
https://github.com/RichardP98/jarvis-son

La documentación complementaria incluye:

README.md: descripción general del proyecto.

docs/arquitectura.md: arquitectura y funcionamiento interno.

docs/instalacion.md: guía de instalación y ejecución.

docs/pruebas.md: plan y registro de pruebas.

15. Estado de la documentación

Esta guía describe el entorno y los procedimientos utilizados durante el desarrollo de Jarvis-son V1.

La instalación debe validarse en otros dispositivos Android para determinar compatibilidad, consumo de memoria, rendimiento y disponibilidad de los motores de voz.

Los resultados de las pruebas deben registrarse en docs/pruebas.md, diferenciando las funcionalidades verificadas de aquellas que todavía se encuentran pendientes de evaluación formal.

