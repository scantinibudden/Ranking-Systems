En la consola dentro de la carpeta se puede utilizar la siguiente funcion:
	./tp1 [Direccion de parametro de entrada] [Direccion del parametro de salida] [Modo de utilizacion]

Modos de utilizacion:
-Colley Matrix Method(CMM) = 0
-Winning Percentage(WP) = 1
-Massey Method(MM) = 2

Ejemplo de de escritura:
	./tp1 carpeta_ejemplo_1/archivo_ejemplo.in carpeta_ejemplo_2/nombre_deseado.out 0

Esto rapidamente generara un nuevo archivo con el nombre de nombre_deseado.out en carpeta_ejemplo_2.
Los datos seran obtenidos de carpeta_ejemplo_2 y seran los que estan en archivo_ejemplo.in.
El metodo a utilizar sera el 0; CMM.

Ejemplo real:
	./tp1 data/nba_2016_scores.dat result/nba_mm.out 2

Esta es la funcion que utilizamos para generar el ranking basado en el metodo 2 (MM) usando los datos de la NBA.