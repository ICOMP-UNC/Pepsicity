# Guía de Instalación para Control de Cinta Transportadora con LPC1769

Este proyecto controla una cinta transportadora utilizando la placa **LPC1769**. El código está escrito en **C** y debe ser compilado usando la **IDE de MCUXpresso**. Una vez compilado, el firmware se carga a la placa para comenzar a controlar la cinta transportadora.

## Requisitos previos

Antes de comenzar con la instalación, asegúrate de tener los siguientes requisitos:

- **Putty** (para enviar comandos al sistema)
- **MCUXpresso IDE** (para compilar y cargar el firmware a la placa)
- **LPC1769** (Placa de desarrollo)
- **Conexión USB** (para cargar el firmware en la placa)
- **Cable de programación** (si es necesario para depuración o programación directa)
- **Librerías y Drivers**: Asegúrate de tener instalados los drivers para la placa LPC1769 y las librerías necesarias para la plataforma.

### Instalación de MCUXpresso IDE

1. **Descargar MCUXpresso IDE**:
   - Ve al sitio oficial de [MCUXpresso IDE](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools/mcuxpresso-ide:MCUXpresso-IDE) y descarga la versión compatible con tu sistema operativo (Windows, macOS o Linux).
   
2. **Instalar MCUXpresso IDE**:
   - Sigue las instrucciones de instalación para tu sistema operativo. Una vez instalado, abre la IDE.

### Instalación de Drivers (si es necesario)

Si no tienes los drivers de la placa LPC1769, descárgalos desde el sitio oficial de NXP o utiliza el software de NXP que permite la instalación automática de drivers.

## Pasos para Compilar y Cargar el Proyecto

### 1. Clonar el repositorio

Clona el repositorio que contiene el código fuente del proyecto a tu máquina local:

```bash
git clone https://github.com/ICOMP-UNC/Pepsicity

```
### 2. Cargar en el MCUXpresso-IDE

Para cargar el software en la placa seleccionar en el IDE: File -> OpenProject from File System. Luego debe compilar el proyecto y debuggearlo .

### 3. Abrir Putty

Configurar el puerto para abrir la comunicación y comunicarse con la placa.

