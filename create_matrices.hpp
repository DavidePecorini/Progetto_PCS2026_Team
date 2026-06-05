#pragma once
#include "Eigen/Dense"


std::pair<Eigen::MatrixXd, Eigen::MatrixXd> create_RB(const std::list<component>& lista_componenti, int nr, const std::vector<std::vector<bool>>& maglie, const unidirected_graph<int>& g)
{
	Eigen::MatrixXd R = Eigen::MatrixXd::Zero(nr,nr);
	Eigen::MatrixXd B = Eigen::MatrixXd::Zero(nr, maglie.size());
	
	for (const auto& c : lista_componenti)
	{
		// Controllo che la componente in analisi sia un resistore
		if (c.tipo == 'R') 
		{
			// Creo la matrice diagonale R, è necessario inserire il -1 poichè i resistori sono numerati partendo da 1
			// tuttavia le matrici partono da indice 0
			R(c.id -1, c.id -1) = c.valore;
			
			for (size_t k = 0 , k < maglie.size(), k++) 
			{
				std::vector<bool> maglia = maglie[k];
				
				if (maglia[g.edge_number((c.nodo1,c.nodo2))])
				{
					\
				
			