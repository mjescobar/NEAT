NEAT Spikes escrito en modern c++.



Cambios generales al algoritmo:

-Se argega el concepto de especie y el de nicho se cambia a raza.
-Marcas historicas son locales (de la especie).
-Ya no existirá cruzamiento entre diferentes especies.
-La distancia entre organismos solo se define para una misma especie.	




Consideracion de implementacion:
-------------------------------

Si deseas ver la implementacion te aconsejo leas algunas consideraciones de implementación que han sido hechas.


[Relativo a la creacion de una nueva ANN ó clonancion ó cruzamiento]: Al crear una nueva ANN se clonan todas las neuronas primero (capa por capa) y luego se crean las conexiones sinapticas y se le agregan a las neuronas, es por esto que al clonar una neurona no se clonan las conexiones sinapticas que poseia. MOTIVO: suponiendo que las neuronas creasen las conexiones synapticas tanto la neurona que empieza como la que termina en la conexion tienen ciertas necesidades con tal conexion synaptica pero esta no debe ser creada dos veces y ademas la que fuera que la crease no tiene como saber como avisarle a la otra neurona que creo tal conexion... por simplicidad se encarga de eso ANN y es a través del algoritmo explicado al comienzo de este parrafo.