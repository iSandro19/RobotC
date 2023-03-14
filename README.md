# RobotC EV3

Prácticas de la asignatura de Robótica (Q8 2022/2023) desarrolladas en RobotC para el robot EV3 de Lego.

Autores:

- Óscar Alejandro Manteiga Seoane
- Antonio Vila Leis

## Prácticas

### Reto 1

1. El robot avanzará en línea recta a una velocidad constante y se detendrá al
detectar un obstáculo a una distancia determinada (ej. 15 cm). Se utilizará el
sensor de ultrasonidos.
2. Se cambiará el color de la iluminación LED siempre que esté a menos del doble de
la distancia de la distancia de parada.
3. Una vez detenido, el robot girará hacia un lado predeterminado.
A. Realizar el giro por tiempo o indicando los grados de avance de cada rueda.
B. Hacer que el robot gire hasta que se llegue a unos grados de giro determinados
(controlado por giroscopio). Sería deseable detectar el caso en el que el robot no es
capaz de completar el giro pasado un tiempo porque se ha quedado atascado.
4. Tras el giro el robot volverá al paso 1 (avance en línea recta si está lejos de
obstáculo).
5. En simulación, escribir repetidamente en la ventana DebugStream la medida del
sensor de ultrasonidos (únicamente cuando varíe respecto al valor previo).

### Reto 2

1. Realizar un control proporcional del movimiento del robot, en función de la distancia a obstáculos:
2. Avance recto a velocidad constante.
3. Comience a reducir la velocidad proporcionalmente a partir de cierta distancia y se detenga a 10 cm aproximadamente.
4. Una vez detenido inicie un movimiento en retroceso aumentando la velocidad proporcionalmente a la distancia hasta que vuelva a detenerse y finalizar el programa.

## Luz

1. Objetivo: detectar bombilla emisora de luz presente en suelo, dirigir el
robot hacia ella y detenerse cerca.
2. Puede ser aconsejable hacer medidas previas de la intensidad de luz ambiente
para calibrar un nivel de intensidad medio del entorno que se tome como base
para distinguirlo de la intensidad de la fuente de luz objetivo.
3. Para dirigirse hacia la fuente de luz el robot debe estimar la dirección en que está
la luz (la dirección aproximada será dónde se detecte mayor nivel) y en caso de
ser necesario girar para tratar de orientarse mejor.
4. El giro para orientarse podría realizarse siguiendo un control proporcional o PID para un
mejor comportamiento, aunque si la luz se comienza a detectar a poca distancia (como
ocurrirá en este caso) el efecto positivo de dicho tipo de control se ve reducido.
5. No se puede implementar este comportamiento de forma realista en el
simulador, ya que no existen elementos que representen fuentes de luz. Se
puede utilizar la detección de color pero no nos permite emular los niveles
de intensidad de una fuente de luz.

## Paredes

1. Objetivo: implementar un control que permita al robot avanzar por el entorno
siguiendo paredes.
2. Si el entorno es cerrado y no tiene islas/columnas interiores, es posible recorrerlo por completo
siguiendo las paredes.
3. El robot comenzará a avanzar y cuando se encuentre cerca de una pared tratará de avanzar
siguiendo la pared, de modo que la pared quede siempre al mismo lado (izquierda o derecha).
4. Antes de estar en el estado “seguir pared”, tendría que dirigirse hacia una pared: avance recto, girando
hacia izquierda, giros en espiral creciente, etc. Se puede ignorar este paso previo, inicialmente.
5. El robot tratará de mantenerse a una distancia deseada de la pared.
6. Se utilizará el sensor de ultrasonidos para medir distancias.
7. Puede utilizarse el giroscopio para controlar los grados de giro.
8. EV3 únicamente dispone de un sensor de ultrasonidos, lo que dificulta la tarea. Es
necesario seguir alguna estrategia, realizando ciertos giros por ejemplo, para evaluar la distancia a la pared y corregir si es necesario.
9. Al acercarse a una esquina puede mejorarse el comportamiento actuando de forma
específica a la hora de realizar el giro necesario según los casos posibles.

## Practica

• La práctica consistirá en la implementación en RobotC de un comportamiento para resolver una
tarea, utilizando la aproximación de arquitecturas subsumidas.

• El comportamiento a implementar es similar al que se ve en teoría como ejemplo de arquitecturas
subsumidas: buscar y dirigirse hacia una luz en un entorno donde el robot no la ve inicialmente.

• No se pueden utilizar: mapas, rutas predefinidas, cualquier otro conocimiento a priori del entorno
que no esté explícitamente reflejado en el enunciado.

• La práctica únicamente se evaluará ejecutándose en el robot real.

• Se aconseja realizar una calibración del sensor de luz para las condiciones de la habitación en el
instante de ejecución (con el robot real).

• En el simulador no existen un elemento para representar una luz en el entorno. Puede
realizarse una detección de color que luego se reemplace por la detección de luz en el robot
real (aunque el comportamiento no será el mismo).

• Se permite reducir la iluminación artificial en el aula para tener un mayor contraste con la luz
que se usará como objetivo.

• Se sugiere implementar y probar aisladamente, y en este orden, los siguientes
comportamientos antes de utilizarlos conjuntamente para obtener el comportamiento
global:

• Acercarse a pared. Ir recto. Puede utilizarse otra estrategia más sofisticada.

• Dirigirse hacia luz. Se utilizará el sensor de color en modo luz ambiente.

• Escapar (salir de situaciones de colisión). Se pueden utilizar el sensor de
contacto, el giroscopio, el sensor de rotación del motor y el sensor de ultrasonidos.

• Seguir paredes. Utilizando el sensor de ultrasonidos, al acercarse a un objeto
seguir su contorno (tratar de mantenerse a una distancia objetiva de la pared). Esto
permite recorrer un entorno fácilmente siguiendo sus paredes, siempre y cuando no
haya columnas (y no las habrá).

• Se pueden seguir paredes dejándolas a la izquierda o a la derecha, pero
siempre al mismo lado.

• La práctica se desarrollará utilizando RobotC.

• Cada comportamiento deberá de estar implementado como una tarea (task) de RobotC.

• La sincronización entre tareas se realizará mediante variables globales y semáforos.

• Por coherencia con los principios de las arquitecturas subsumidas, usar una variable global
diferente para coordinar la ejecución de cada dos niveles de tareas adyacentes en la
arquitectura subsumida. De esta forma, una variable global dada solo podrá ser escrita por una
tarea (la que implemente el comportamiento del nivel n) y leída por otra (la que implemente el
comportamiento del nivel n+1).

• Una tarea tiene la capacidad de inhibir la ejecución de la de nivel inferior y tiene que
comprobar si su ejecución está inhibida por la superior.

• La inhibición se propaga a los niveles inferiores.

• No se utilizarán prioridades propias de RobotC.

• No se debe abusar del uso de la función wait/sleep.
