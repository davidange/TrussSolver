#include "Truss.h"
#include <iostream>
#include<vector>
#include <map>
#include <Eigen/Dense>

Eigen::MatrixXd calculateGlobalMatrix(std::vector<Truss>& TrussVector,int numberOfDOFs) 
{
	Eigen::MatrixXd globalMatrix=Eigen::MatrixXd::Zero(numberOfDOFs, numberOfDOFs);
	int EFT[4];

	for (Truss truss : TrussVector) {
		//TOOD modify for generalized id Count of Nodes (if there are gaps in counting of ID) 
		EFT[0] = (truss.getNode1()->getId() - 1) * 2;
		EFT[1] = (truss.getNode1()->getId() - 1) * 2+1;
		EFT[2] = (truss.getNode2()->getId() - 1) * 2;
		EFT[3] = (truss.getNode2()->getId() - 1) * 2 + 1;
		Eigen::Matrix4d trussStiffnessMatrix = truss.getGlobalMatrix();
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				//std::cerr<< globalMatrix(EFT[row], EFT[column]) <<"\n";
				globalMatrix(EFT[row], EFT[column]) = globalMatrix(EFT[row], EFT[column])+ trussStiffnessMatrix(row, column);
			}
		}
	}
	//round Matrix  zeros
	for (int row = 0; row < numberOfDOFs; row++) {
		for (int column = 0; column < numberOfDOFs; column++) {
			if (-1e-10<globalMatrix(row, column) && globalMatrix(row, column)<1e-10) {
				//std::cerr << "A value hass been rounded\n";
				globalMatrix(row, column) = 0;
			}

		}
	}
	return globalMatrix;
}

int main() {
	//input Trusses
	double A1 = 1;
	double A2 = 2;
	double E = 100;
	double alpha = .001;
	double P = 5;
	double U = 2;
	double deltaT = 30;
	double L = 10;
	
	std::map<int, Node> mNodes;
	std::vector<Truss> vTrusses;
	//input Nodes
	mNodes[1] = Node(0, L, 1);
	mNodes[2] = Node(0, 0, 2);
	mNodes[3] = Node(L, L, 3);
	mNodes[4] = Node(2*L, L, 4);
	mNodes[5] = Node(3*L, L, 5);
	mNodes[6] = Node(3*L, 0, 6);



	//input trusses
	vTrusses.push_back(Truss(&mNodes[1] , &mNodes[3], A2, E, 1));
	vTrusses.push_back(Truss(&mNodes[2], &mNodes[3], A1, E, 2));
	vTrusses.push_back(Truss(&mNodes[3], &mNodes[4], A2, E, 3));
	vTrusses.push_back(Truss(&mNodes[4], &mNodes[5], A2, E, 4));
	vTrusses.push_back(Truss(&mNodes[4], &mNodes[6], A1, E, 5));


	//print list of Nodes
	for (auto nodePair : mNodes) {
		std::cout << nodePair.second << "\n";
	}
	//print list of Trusses
	for (Truss truss : vTrusses) {
		std::cout << truss << " with Length of " << truss.getLength() << "\n";
		std::cout << "Local Matrix :\n";
		std::cout << truss.getGlobalMatrix() << "\n";
		std::cout << "---------------------------------------------------\n\n";
	}

	//create Global Stiffness Matrix
	Eigen::MatrixXd globalStiffnessMatrix = calculateGlobalMatrix(vTrusses, mNodes.size() * 2);
	std::cout << globalStiffnessMatrix;


	//reduce Matrix After applying BC




	return 0;
}