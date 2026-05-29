#include "create_graph.hpp"
#include "struct_class.hpp"
#include <list>
#include <iostream>
//questo è il main
int main(int argc, char **argv) 
{
	std::list<component> lista_componenti = read_input(argc, argv);
	
	for (component c : lista_componenti) 
	{
		std::cout << c.tipo << c.id << " " << c.valore << " " << c.nodo1 << " "  << c.nodo2 << std::endl;
	}
		unidirected_graph<int> grafo_circuito = graph_builder(lista_componenti);
		for (const unidirected_edge<int>& arco : grafo_circuito.all_edges())
		{
			std::cout << arco;
		}
		return 0;
	}