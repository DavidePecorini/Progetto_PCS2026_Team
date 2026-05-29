#pragma once
#include <fstream> 
#include <iostream>
#include "struct_class.hpp"
#include <list>
#include <string>

std::list<component> read_input(int argc, char **argv) 
{
	std::list<component> lista_componenti;
	
	if (argc < 2) {
		std::cerr << "ERRORE! È necessario inserire il nome del file!" << std::endl;  // errore
		return lista_componenti;
	}
	
	std::string filename = argv[1];
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		std::cerr << "ERRORE! Impossibile aprire il file: " << filename << std::endl;
		return lista_componenti;
	}
	
	char t;
	int n;
	double v;
	int n1;
	int n2;
	
	while (ifs >> t >> n >> v >> n1 >> n2)
	{
		component c;
		
		c.tipo = t;
		
		c.id = n;
		
		c.valore = v;
		c.nodo1 = n1;
		c.nodo2 = n2;
		
		lista_componenti.push_back(c);
	}
	return lista_componenti;
}

unidirected_graph<int> graph_builder(std::list<component>& lista_componenti)
{
	unidirected_graph<int> grafo_circuito;
	for (const auto& c : lista_componenti) 
	{
		unidirected_edge<int> arco(c.nodo1, c.nodo2, c.tipo, c.valore);
		grafo_circuito.add_edge(arco);
	}
	return grafo_circuito;
}

		