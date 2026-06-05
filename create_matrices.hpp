#pragma once
#include "Eigen/Dense"


std::pair<Eigen::MatrixXd, Eigen::MatrixXd> create_RB(std::list<component>& lista_componenti, int nr, std::vector<std::vector<bool>>& maglie)
{
	Eigen::MatrixXd R(nr,nr);
	Eigen::MatrixXd B(nr, maglie.size());
	
	for (const auto& c : lista_componenti)
	{
		if (c.tipo == 'R') 
		{
			R(c.id -1, c.id -1) = c.valore;
			
			for (const auto& maglia : maglie) 
			{
				
			