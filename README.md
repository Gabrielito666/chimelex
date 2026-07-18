# Idea a futuro

```xml
<serie>
	<sound duration="2"/>
		<wave-fn name="chimelex.sin" amp="0.3" fq="440.0"/>
	</sound>
	<sound duration="3">
		<wave-fn name="chimelex.sqar" amp="0.3" fq="460.0"/>
	</sound>
</serie>
```
o
```xml
<serie>
	<sound duration="2" wave-fn="chimelex.sin" amp="0.3" fq="440.0"/>
	<sound duration="3" wave-fn="chimelex.sqar" amp="0.3" fq="460.0"/>
</serie>
```
un parser que convierta un xml como ese o un json en el arbol de estructuras en c y lo compile o reproduzca

```bash
chimelex -f mi-obra.chml -o mi-obra.wav
```

y que se puedan poner plugins de sonidos o efectos en una carpeta `chimelex-modules`

```fs
mi-obra.chml
chimelex-modules/
	super-sounds.elephant.c
	super-sounds.elephant.h
```

y luego 
```xml
<serie>
	<sound duration="2" wave-fn="super-sounds.elephant" happiness="0.2" age="14"/>
</serie>

```

Algo de esa naturaleza

# yo desde el futuro

pero que tambien se puedea con funciones en el xml:

```xml
<chml>
<functions>
	wave-fn elephant(t, { happiness:=0.5, age:=10 }) = sin(t * happiness - age);

	wave-fn conditional-sin(t) = {
		if(t > 5) return sin(t * 100);
		return  sin(t * 200)
	}

	f(t) = 3x;

	#includes ./src/my-wave-functions/dog.chs //de chimescript o algo así
</functions>
<serie>
	<sound duration="2" wave-fn="elephant" happiness="0.2" age="14"/>
	<sound duration="3" wave-fn="conditional-sin"/>
</serie>
</chml>
```

Los params de este lenguage de scripting debiesen tener valores numericos, booleans, strings o fn.

Una fn es un tipo de dato matemático con 2 posibles parametros. t obligatorio (tiempo u eje horizontal) y extraParams (un objeto de parametros), y retornando siempre un numero. pero no todos son wave-fns si a happiness le pasamos el nombre de una fn cada iteración se le pasaría el resultado segun t y los parametros que se le pasen.

```xml
	<sound duration="2" wave-fn="elephant" happiness="fn:f"/>
```

Con esto los parametros pueden ser mucho más poderosos.

## effects

```xml
	<functions>
		effect overdrive(fq, t) = fq * 100;
		effect chorus(fq, t, { current-wave-fn }) = ((current-wave-fn(t+1) +1) + fq + (current-wave-fn(t-1) -1) / 3;
	</functions>
	<sound duration="2" wave-fn="chimelex.sin" fq="440" amp="0.5" effect="overdrive"/>
	<serie effect="[overdrive, chorus]">
		<sound wave-fn="sin" fq="440" amp="0.5"/>
		<sound wave-fn="sin" fq="404" amp="0.2"/>
	</serie>
```

## sample

Chimelex debe traer una wave-fn propia y accecible sample o import..

```xml
	<sound duration="*" wave-fn="sample" src="./my-sample.wav"/>
```
