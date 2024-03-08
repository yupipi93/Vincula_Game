# Vincula
[ABPCE19] Proyecto de Videojuego del grupo SixpiDev del ABP 2019/20

En la carpeta raíz encontramos el ejecutable compilado del juego y en la carpeta Vincula_WRelease la versión para Windows.

Si desea recompilar el proyecto, el archivo MAKEFILE incluye las siguientes instrucciones:<br />
	- Make:
		Compila el proyecto en modo debug<br />
	- Make clean:
		Borra todos el directorio obj, lo que permite recompilar el proyecto por completo<br />
	- Make cleanall:
		Realiza un Make clean y elimina también el archivo ejecutable<br />
	- Make all:
		Realiza un Make all y luego un Make<br />
	- Make RELEASE=1:
		Compila el proyecto completo con optimizaciones. Se recomienda hacer primero un Make cleanall, para asegurar que se compilan todos los archivos con optimizaciones, y no en modo debug como está configurado por defecto.<br />
	- Make WIN=1:
		Compila el proyecto completo para Windows con optimizaciones. Se recomienda hacer primero un Make cleanall, para asegurar que se compilan todos los archivos con optimizaciones, y no en modo debug como está configurado por defecto.<br />

El ejecutable consta de 4 parámetros:<br />
	- Modo debug / release<br />
	- Ancho pantalla<br />
	- Alto pantalla<br />
	- Modo windows / fullscreen<br />

Por defecto el juego se ejecutará en modo release, a 1920x1080 y en pantalla completa.