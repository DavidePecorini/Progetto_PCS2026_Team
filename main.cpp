#include "create_graph.hpp"
#include "struct_class.hpp"
#include "depina.hpp"
#include <list>
#include <iostream>

int main(int argc, char **argv) 
{
	auto  [lista_componenti, nr] = read_input(argc, argv);
	
	for (component c : lista_componenti) 
	{
		std::cout << c.tipo << c.id << " " << c.valore << " " << c.nodo1 << " "  << c.nodo2 << std::endl;
	}
	unidirected_graph<int> grafo_circuito = graph_builder(lista_componenti);
	for (const unidirected_edge<int>& arco : grafo_circuito.all_edges())
	{
		std::cout << arco;
	}
	std::vector<std::vector<bool>> maglie = de_pina(grafo_circuito);
	for (const auto& maglia : maglie)
	{
		std::cout << "(";
		for (size_t k = 0; k < maglia.size()-1; k++)
		{
			std::cout << maglia[k] << ", ";
		}
		std::cout << maglia[maglia.size()-1] << ")" << std::endl;
	}
	return 0;
	}