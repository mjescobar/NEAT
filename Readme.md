# NEAT en modern C++

El código que se está presentando representa una librería de NEAT [1] (Neuro Evolution of Augmenting Topologies), la cual puede ser instalada para su sencillo uso en experimentos personales.

Este algoritmo está explicado en extensión por su autor [1] y es la base necesaria para quienes deseen entender el trasfondo, aunque para usar NEAT no es necesario su completa comprensión.

## Instalación y uso.

Se encuentra en la carpeta el archivo GuiaUsuario.pdf en el cual se explica con más detalles tanto la instalación como el uso del software con ejemplos.

Con menos detalle y a modo de resumen:

### Instalación [Sólo linux, pronto a través de cmake (y para toda plataforma por lo tanto)]

En esta carpeta:

> make
> sudo make install

Luego de esto usted tendrá instalada la librería de NEAT, para agregar los headers a su propio programa usted debe agregar '''#include <NEATSpikes>''' en su header y para linkear los objetos en su solución usted debe agregar -lneatspikes en las opciones de compilación. 

En los experimentos de muestra usted puede encontrar archivos Makefile que realizan toda esta operación y los cuales usted puede usar como template.

Recordar: Una vez instalado usted puede crear sus propios códigos que usen esta librería en cualquier sitio de su computadora lo que lo hace más simple de usar.

### Uso

Refierase a GuiaUsuario.pdf.


## Ejemplos

En la carpeta experimentSamples se encuentran 2 ejemplos de experimentos que pueden ser probados y usados como base, de todas formas se recomienda que sean manejados después de leer el documento GuiaUsuario.pdf


## Características mejoradas en esta versión de NEAT.

Existen varias diferencias con respecto al algoritmo original, pero siempre manteniendo el principio de lo que el autor explica en [1], para ver las diferencias en cavalidad se presenta el archivo diferencias.pdf.

Para entender mejor las características añadidas importantes lea CaracteristicasMejoradas.pdf en el cual se explicarán esencialmente 2 cambios los cuales el primero tiene que ver con la estructura orientada a objetos utilizada la cual agrega posibilidades de extensión del algoritmo y además un modelo abstracto de uso de NEAT que simplifica su uso en cualquier tipo de experimento.

# Referencias:

[1] (2002) Kenneth S. "Evolving Neural Networks through Augmenting Topologies", The MIT Press Journal. 