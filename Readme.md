# NEAT en modern C++

El código que se está presentando representa una librería de NEAT [1] (Neuro Evolution of Augmenting Topologies), la cual puede ser instalada para su sencillo uso en experimentos personales.

Este algoritmo está explicado en extensión por su autor [1] y es la base necesaria para quienes deseen entender el trasfondo, aunque para usar NEAT no es necesario su completa comprensión.

## Instalación y uso.

Se encuentra en la carpeta el archivo GuiaUsuario.pdf en el cual se explica con más detalles tanto la instalación como el uso del software con ejemplos.

Con menos detalle y a modo de resumen:

### Instalación [Sólo linux, pronto a través de cmake (y por lo tanto para toda plataforma)]

Dirijase con su terminal a la carpeta que contiene este archivo Readme.md, luego ejecuta los siguientes comandos (comandos típicos de Makefile)

> make

> sudo make install

Luego de esto usted tendrá instalada la librería de NEAT, para agregar los headers a su propio programa usted debe agregar '''#include \<NEATSpikes\>''' en su header y para linkear los objetos en su solución usted debe agregar -lneatspikes en las opciones de compilación. 

En los experimentos de muestra usted puede encontrar archivos Makefile que realizan toda esta operación y los cuales usted puede usar como template.

Recordar: Una vez instalado usted puede crear sus propios códigos que usen esta librería en cualquier sitio de su computadora lo que lo hace más simple de usar.

### Uso

El usuario desea que una red neuronal le solucione un problema, para ello debe confeccionar un experimento en el cual sea evaluada cada red neuronal. El usuario sólo tiene que ser capaz de calificar qué tan bien se está resolviendo su experimento con las redes neuronales que van siendo producidas por el algoritmo (nosotros nos referimos a esto como otorgarle un fitness), para esto el usuario debe ser capaz de crear un experimento en el cual, usando la red neuronal, se evalua el desempeño de la red neuronal.

Refierase a GuiaUsuario.pdf.


## Ejemplos

En la carpeta experimentSamples se encuentran 2 ejemplos de experimentos que pueden ser probados y usados como base, de todas formas se recomienda que sean manejados después de leer el documento GuiaUsuario.pdf


## Características mejoradas en esta versión de NEAT.

Existen varias diferencias con respecto al algoritmo original, pero siempre manteniendo el principio de lo que el autor explica en [1], para ver las diferencias en cavalidad se presenta el archivo diferencias.pdf.

Para entender mejor las características añadidas importantes lea CaracteristicasMejoradas.pdf en el cual se explicarán esencialmente 2 cambios los cuales el primero tiene que ver con la estructura orientada a objetos utilizada la cual agrega posibilidades de extensión del algoritmo, y en segundo lugar un modelo abstracto de NEAT que simplifica su uso en cualquier tipo de experimento.

Además de todo lo anterior, y no menos, se hizo el esfuerzo de modernizar NEAT con modern C++ con tal de asegurar un uso eficiente de la memoria y resultado eficiente en termino de CPU también.

## Explicación de cada carpeta dentro.

Las siguientes carpetas serán encontradas en la raiz de está libreria:

1. Documents: Aqui se encuentran las fuentes de todos los archivos pdf que son usados para explicar.
2. Tools: Todas las herramientas externas que son usadas para, por ejemplo, realizar gráficos de los resultados obtenidos u cualquier labor que sea considerada externa a NEAT.
3. experimentSamples: Carpeta que contiene todos los experimentos de muestra, los cuales pueden ser usados como punto de partida en la creación de otros experimentos.
3. include: Se encuentran todas los headers de todas las clases usadas.
4. src: Se encuentran todas las definiciones de las clases usadas.
5. test: carpeta que contiene las pruebas a las que son sometidas todas las clases con el objetivo de garantizar que no se posean errores en el código.


# Referencias:

[1] (2002) Kenneth S. "Evolving Neural Networks through Augmenting Topologies", The MIT Press Journal. 