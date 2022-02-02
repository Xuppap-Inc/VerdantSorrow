# VERDANT SORROW

***Documento de diseño de videojuego***

**Nombre de la empresa**: *Xuppap Inc.*

**Integrantes y correos**: *Daniel Martín Gómez (damart24@ucm.es), Miriam Martín Sánchez (mirima10@ucm.es), Javier Callejo Herrero (jacall02@ucm.es), Laura Gómez Bodego (lgomez25@ucm.es),  Nacho Del Castillo (igdelcas@ucm.es), Javier Muñoz García (javimuno@ucm.es), José María Gómez Pulido (jgomez18@ucm.es),Paula Morillas Alonso (pamorill@ucm.es), Rodrigo Sánchez Torres (rodsan05@ucm.es), Sergio Baña Marchante (sebana@ucm.es).  
Versión 1 - 30 de enero de 2022*

| Resumen||
:--: | :--: 
Géneros | Souls-Like, Acción, Aventuras, Boss-rush 
Modo | Single-player
Público Objetivo | Edad: 15-30 años
||Sexo: todos
||Idioma: español
Plataformas|Windows
---

**Tabla de contenidos**

1. [Aspectos generales](#aspectos-generales)   
    1.1. [Relato breve y parcial de una partida típica](#relato-breve)
2. [Jugabilidad](#jugabilidad)  
    2.1. [Mecánica](#mecanica)  
        2.1.1. [Mecánicas del personaje](#mecanicas-del-personaje)  
        2.1.2. [Mecánicas de enemigos](#mecanicas-de-enemigos)    
    2.2. [Controles](#controles)  
    2.3. [Cámara](#camara)  
    2.4. [Dinámica](#dinamica)  
    2.5. [Estética](#estetica)  
3. [Menús y modos de juego](#menus-y-modos-de-juego)  
    3.1. [Configuración](#configuracion)  
    3.2. [Interfaz y control](#interfaz-y-control)
4. [Breve descripción del sistema y plataforma de gestión](#descripion-del-sistema)  
5. [Contenido](#contenido)  
    5.1. [Música](#musica)  
    5.2. [Sonidos](#sonidos)  
    5.3. [Imágenes](#imagenes)  
    5.4. [Historia](#historia)  
    5.5. [Niveles](#niveles)   
6. [Referencias](#referencias)
___


## <a name = "aspectos-generales"></a>1. Aspectos Generales

Vista General |
:--:|
![Vista General]()|

### <a name = "relato-breve"></a> 1.1. Relato breve y parrcial de una partida típica   
El jugador realiza el tutorial. Tras eso sale al hub. Allí tiene que conseguir 3 almas de jefes para abrir una puerta. 

Cada alma la consigue derrotando al jefe correspondiente.  

---  
## <a name = "jugabilidad"></a> 2. Jugabilidad  
### <a name="mecanica"></a> 2.1. Mecánica 
### <a name="mecanicas-del-personaje"></a> 2.1.1. Mecánicas del personaje  

-Movimiento: El jugador podrá moverse hacia la izquierda o la derecha con un movimiento lateral a velocidad constante. Además podrá saltar.  

-Esquivar: El jugador podrá esquivar ataques, consiguiendo frames de invulnerabilidad durante la animación de esquivar.  

-Ataque: El personaje dará un barrido hacia delante con la espada, haciendo un daño. Te quedas quieto cuando estés atacando.  

-Vida: El jugador dispondrá de tres vidas, perdiendo una por cada golpe que reciba de un enemigo. Al recibir un golpe el jugador será impulsado hacia atrás y brillará en blanco.

### <a name="mecanicas-de-enemigos"></a> 2.1.2. Mecánicas de enemigos  

Comportamiento de los jefes: 
  - Jefe de la rana
      - Visual: Una rana quiere comer moscas con la lengua.
      - Primera fase:  
          - La rana va saltando de un lado a otro de la pantalla. Empieza saltando hacia la izquierda, calculando la distancia del salto aleatoriamente dentro de un intervalo.
          - Si el salto hace que la rana salga de la pantalla, salta en la otra dirección.
          - Ataque de la lengua: Cada X saltos(decidido aleatoriamente entre un intervalo), hace este ataque:
            - Aparece una mosca cerca del jugador, a una distancia determinada de la rana siempre a la misma distancia del suelo. Si la posición se sale de la pantalla, aparece en la otra dirección. La mosca aparece siempre a la altura del jugador.
            - X segundos después, la rana lanza la lengua hasta la posición x de la mosca y hace daño, y vuelve rápidamente. La lengua sale de forma horizontal.
            - Si matas a la mosca se enfada y realiza un ataque, un salto sobre sí mismo realizando una onda expansiva al caer. Tras este ataque se queda vulnerable durante un tiempo.
            - La onda expansiva se define como un proyectil que se mueve de forma horizontal desde la posición del boss hasta uno de los extremos de la pantalla, pegado al suelo a velocidad constante, de tamaño lo suficientemente pequeño para que pueda ser saltado por el jugador.
        - Segunda fase:
              - Mismo comportamiento que en la primera fase.
              - La rana se enfada y cambia de color (por ejemplo), ahora ataca al jugador.
              - Añade un nuevo ataque en que los saltos son el doble de alto y cada uno causa una onda expansiva.
              - En cada salto la rana tiene un 70% de probabilidad de hacer saltos pequeños y un 30% de hacer saltos grandes.
              - El ataque de la lengua cambiaria. Ahora la mosca no aparece antes de tirar la lengua, si no que siempre ataca siempre hacia el jugador de forma horizontal.
    - Árbol humanoide:
      - Visual: Groot con una lámpara.
      - Primera fase: 
        - Se va acercando hacia la posición x del jugador, y cuando está lo suficientemente cerca, manotazo a melee, se queda quieto un instante.
        - Ola de raíces: Tras un tiempo aleatorio en un intervalo, salen raíces del suelo progresivamente izquierda a derecha, separadas a la misma distancia cada una con un pequeño delay entre que sale una rama y otra. Cubriendo todo el alto de la pantalla. Antes de que salga una raíz, hay un indicador de una raíz pequeñita en la posición en la que va a salir la raíz.
        - Ataque raíces con auto-aim: Cada X segundos, elegidos aleatoriamente dentro de un intervalo. El boss se sale de pantalla, la lámpara se queda en mitad del ancho de la pantalla, y lo suficientemente alta para que el jugador no llegue. Las raíces del ataque anterior van saliendo en la posición del jugador cada X segundos. Si una de las raíces alcanza a la lámpara, se le hace mucho daño y se cambia a otro patrón de ataque.
      - Segunda fase: 
        - El boss ya no lleva la lámpara con él. La lámpara aparece en un lado de la pantalla.
        - Cuando recibe 3 golpes, cambia al otro lado de la pantalla. De forma que el boss siempre está al principio entre la lámpara y el jugador. Esto se repite 3-4 veces.
        - Al igual que en la primera fase, el boss persigue al jugador, y cuando lo tiene a rango, le pega un manotazo.
        - El ataque de raíces con auto-aim se realiza constantemente. Si la raíz sale cerca de la lámpara se cancela (el jefe no le pega a su lámpara).
    - Jefe de las manos: 
      - Visual: Un cuerpo principal (ojo, cabeza, mago…) que ataca con 2 manos gigantes.
      - Hay que subirse a las manos para atacar al cuerpo principal.
      - Las manos vuelven a su posición original tras cada ataque.
      - Tres ataques:
        - Clap: junta ambas manos hacia el centro de la pantalla rápidamente a ras de suelo.
        - Puñetazo: la mano se echa hacia atrás y mete un puñetazo rápido.
        - Martillazo: la mano choca contra el suelo.
      
      - Las manos hacen daño sólo durante el ataque. (Idea opcional: nunca hacen daño por la parte superior).
      - Segunda fase:
        - El boss se desencadena y comienza a moverse por la pantalla.
        - El resto de ataques siguen igual, solo que las manos ahora hacen daño por contacto siempre (se prenden).
        - El movimiento del cuerpo del boss es el siguiente: <a ref="https://www.youtube.com/watch?v=5mGuCdlCcNMMovimiento">Movimiento</a>
        - Cuando choca contra el suelo lanza una bola de fuego a cada lado y se queda inmóvil unos segundos.
        - Cada X tiempo lanza una bola de fuego hacia el jugador, que desaparece al chocar con un lateral de la pantalla o el suelo.
      - Tercera fase:
        Cámara scroll lateral hacia la derecha. El boss te persigue desde la izquierda y el jugador tendrá que correr hacia la derecha para huir.

### <a name="controles"></a> 2.2. Controles  
El jugador se controla con las teclas WASD del teclado (W se utiliza para saltar, al igual que la tecla Espacio, A y D para el movimiento lateral). Se atacará con la tecla J, y se esquivará con el Shift izquierdo.
<p align="center">
  <img src="./images/masalladelbosque.jpg" width="400"/>

### <a name="camara"></a> 2.3. Cámara  
La cámara estará posicionada de modo que se muestre una vista lateral del personaje y del escenario. Según el jugador vaya avanzando, la cámara lo seguirá con un cierto delay haciendo scroll, con el jugador siempre centrado en la cámara. En las peleas con los jefes la cámara está fija en el centro porque las zonas de combate ocupan una pantalla únicamente.

### <a name="dinamica"></a> 2.4. Dinámica  
- Objetivo: Avanzar a través de las diferentes zonas venciendo a los jefes que se encuentren por el camino.  

- Ganar: Acabar con el jefe final.  

- Perder: Morir en combate contra los enemigos.  

- Castigo: Volver a empezar desde el checkpoint más cercano (los checkpoints estarán en pequeños pasillos entre las zonas).  


### <a name="estetica"></a> 2.5. Estética
<p align="center">
  <img src="./images/masalladelbosque.jpg" width="400"/>
  <p align="center">
  <img src="./images/fondo1.jpg" width="400"/>

Se busca una estética inspirada principalmente en “Más allá del jardín”. Dibujos infantiles pero con una tonalidad oscura que nos puede recordar también a la estética de Tim Burton. Los personajes, enemigos y objetos con los que interactúa el jugador tendrán tonos con más saturación para poder distinguirlos del fondo. Todo el arte tendrá lineart negro, exceptuando zonas muy alejadas del fondo. En ciertos momentos se usará una composición uniforme con un elemento contrastado para captar la atención del jugador (por ejemplo cuando el jefe de la linterna sale de la pantalla la linterna contrasta con todo lo demás).

Esta será la paleta de colores que se utilizará.
 <p align="center">
  <img src="./images/paleta.jpg" width="400"/>

---

## <a name="menus-y-modos-de-juego"></a> 3. Menús y modos de juego
### <a name="configuracion"></a> 3.1. Configuración

- Menú principal: Cuenta con los siguientes botones:
    - Nueva partida: Comienza una nueva partida.
    - Continuar: Continúa el juego en el último punto de guardado.
   -  Zonas: Te permite seleccionar un jefe contra el que luchar.
   -  Ajustes: Permite regular el volumen de salida de audio.
   -  Controles: Muestra una pantalla con los controles del juego.
   -  Salir: Permite salir del juego.
 
 - Menú de pausa: Cuenta con los siguientes botones:
    - Reanudar nivel: Permite reanudar la partida.
    - Ajustes: Permite regular el volumen de salida de audio.
    - Salir al menú principal: Permite volver al menú principal.
    - Salir : Permite cerrar el juego sin tener que salir al menú principal.

 Al utilizar las opciones de salir, el juego guardará el estado desde el último punto de guardado.

 ### <a name="interfaz-y-control"></a> 3.2. Interfaz y control  
 La interfaz será una interfaz sencilla; contendrá 3 corazones que representan la vida del jugador. Cada enemigo tendrá en la cabeza una barra que representa su vida. 

 <p align="center">
  <img src="./images/paleta.jpg" width="400"/>

---

 ## <a name="descripion-del-sistema"></a> 4. Breve descripción del sistema y plataforma de gestión  
 Utilizaremos GitHub Issues como sistema de gestión, cada historia de usuario se escribirá como una issue y los responsables se la asignarán.  

 ### 4.1. Breve descripción del sistema y plataforma de comunicación 
 Utilizaremos medios como Discord y/o WhatsApp para la comunicación efectiva entre todo el equipo y para acordar fechas y horas de reunión para aclarar dudas y posibles conflictos.  

---

  ## <a name="contenido"></a> 5. Contenido  
  ### <a name="musica"></a> 5.1. Música  
  - Musica de Boss.  
  - Musica de combate normal.  
  - Musica menú.  

  ### <a name="sonidos"></a> 5.2. Sonidos
  - Jugador
    - Espadazo (suena cuando el jugador ataca)
    - Salto
    - Rodar 
  
  - General
    - GameOver
    - Derrotar Boss
  
  - Enemigos
  - Bosses

  ### <a name="imagenes"></a> 5.3. Imágenes


  ### <a name="historia"></a> 5.4. Historia  
  Durante los últimos milenios, el mundo de Arven vio cómo los poderes naturales que daban la noción de equilibrio al propio planeta y ayudaban a sus habitantes a prosperar y estar en armonía con el mundo mismo, iban pervirtiendose y transformándose en una mágica distinta, más egoísta, impersonal y codiciosa.  

  Las tres esencias primordiales del mundo y sus custodios se refugiaron en un bosque ancestral, un bosque sin nombre, donde los habitantes de este planeta vieron su luz por primera vez.  

  Terria, la esencia de toda la materia.  

  Lumine, la esencia de la luz y la vida.  

  Etheria, la esencia que constituye la voluntad y la mente.  

  El uso incontrolado de esta magia acabó con la paz que imperaba en el mundo, resultando en guerras por el control de sus terrenos y en rituales para succionar las energías primordiales y transformarlas en esta nueva magia arcana, si bien más poderosa, también destructiva en su descontrol.  

  Así pasó que, cuando desde diversos puntos del mundo la succión de la esencia del mundo fue desmedida e incontrolada, la magia que otrora sirviera a los propósitos de sus habitantes implosionó de manera global, borrando de la superficie todo cuando encontró a su paso, nada que hubiera tocado la magia sobrevivió.  

  Con este devastador suceso, tan solo los custodios de las esencias sobrevivieron, más no como ellos esperaban, sus nombres se olvidaron y su apariencia cambió, pasando a ser criaturas grotescas y furibundas cuyo único propósito es ahora ser las carceleras de las fuerzas primordiales.  

  El custodio de Terria se transformó en una gigantesca rana que consume todo cuanto está a su alcance.  

  El de Lumine se convirtió en un árbol viviente que encarceló la esencia de la luz en una lámpara cerrada.  

  Por último, el de Etheria acabó por transformarse en un ser informe sin mente, una masa de materia en continua alteración, salvo por su único ojo y sus dos gigantescas manos que arremeten contra todo aquello que ve.  

  Para traer de vuelta el equilibrio, el propio mundo ha canalizado sus fuerzas y dado vida a su avatar, Kyna con la esperanza de que este reúna las 3 esencias primordiales.  

 ### <a name="niveles"></a> 5.5. Niveles  
---

## <a name="referencias"></a> 6. Referencias  

- Estética basada en “Más allá del jardín”, Pat McHale.  

- Dark Souls, FromSoftware, BANDAI NAMCO.  

- Ender lilies, Adglobe, Live Wire,Binary Haze Interactive.  

- Hollow Knight, Team Cherry Games.  

- Morbid, Still Running, Merge Games  

- Cuphead, Studio MDHR Entertainment Inc.  

- Titan Souls, Acid Nerve, Devolver Digital.

- Dead cells, Motion Twin.  

- Blasphemous, The Game Kitchen, Team17.  

- Death Door, Acid Nerve, Devolver Digital.  

- Salt and Sanctuary, Ska Studios.

---










https://github.com/jgomez18UCM/Proyectos2








