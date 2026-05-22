//Creo il file di lettura dell'input, creo per prima cosa la struttura che uso per memorizzare la componente 
#pragma once

struct component
{    //uso una struttura perchè voglio avere tutto pubblico
	char tipo = ' ';
	int id = 0;
	double valore = 0.0;
	int nodo1 = -1;
	int nodo2 = -1;
};