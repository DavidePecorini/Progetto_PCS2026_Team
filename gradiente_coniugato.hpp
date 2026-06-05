#include <Eigen/Dense>


std::pair<Eigen::VectorXd, unsigned int> gradiente_coniugato(const Eigen::MatrixXd& A, 
															 const Eigen::VectorXd& b, 
															 Eigen::VectorXd& x,
															 const double tol) 
{
	// Calcolo i dati iniziali 
	Eigen::VectorXd res = b - A * x;
	Eigen::VectorXd p = res;
	// Il numero massimo di iterazioni dipende dalla dimensione del sistema
	const unsigned int it_max = 10 * b.size();
	
	// Inizializzo tolleranza relativa e contatore
	// Al posto di calcolare res.norm() / res_norm_0 > tol faccio res.norm() > tol * res_norm_0
	// in quanto la moltiplicazione è più veloce della divisione, in più il termine di destra lo 
	// calcolo una volta sola
	double tol_rel = tol * res.norm();
	unsigned int it = 0;
	
	
	while (it < it_max && res.norm() > tol_rel) 
	{
		// Ottimizzo e calcolo A * p una volta sola invece che due
		Eigen::VectorXd Ap = A * p;
		
		// Utilizzo .dot() per calcolare il prodotto scalare di 
		// p * A * p per non calcolarlo due volte
		double p_scalare_Ap = p.dot(Ap);
		
		// Continuo ad usare il prodotto scalare invece del prodotto tra vettori dello pseudocodice
		// poichè il prodotto tra vettori restituisce una matrice 1x1 che rende necessario usare 
		// .value() per estarre il valore mentre .dot() restituisce già uno scalare
		
		double alpha = p.dot(res) / p_scalare_Ap;
		
		// Aggiorno il valore di x e di res
		x += alpha * p;
		
		// Sostituendo la def di x nella formula del residuo otteniamo una formulazione ottimizzata
		// infatti res = b - A * x = b - A * (x + alpha * p) = r - alpha * Ap
		res -= alpha * Ap;
		
		
		// Calcolo beta e aggiorno la direzione di discesa 
		// Sfrutto l'aver già calcolato A * p e la commutatività del prodotto scalare
		double beta = res.dot(Ap) / p_scalare_Ap;
		p = res - beta * p;
		
		// Aggiorno il contatore
		it++;
	}
	
	return {x,it};
}
