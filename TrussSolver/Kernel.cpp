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
Eigen::VectorXd calculateThermalLoadForceVector(std::map<int, double>& mTemperatures, std::vector<Truss>& vTrusses, int numOfDOFS) {
	Eigen::VectorXd globalThermalLoadVector = Eigen::VectorXd::Zero(numOfDOFS);
	int EFT[4];

	for (auto thermalLoad : mTemperatures) {
		Truss& truss = vTrusses[thermalLoad.first - 1];
		EFT[0] = (truss.getNode1()->getId() - 1) * 2;
		EFT[1] = (truss.getNode1()->getId() - 1) * 2 + 1;
		EFT[2] = (truss.getNode2()->getId() - 1) * 2;
		EFT[3] = (truss.getNode2()->getId() - 1) * 2 + 1;
		Eigen::Vector4d thermalLoadVector = truss.getGlobalThermalVectorLoad()*thermalLoad.second;
		for (int row = 0; row < 4; row++) {
			globalThermalLoadVector(EFT[row]) += thermalLoadVector(row);
		}

	}
	return globalThermalLoadVector;
}

Eigen::VectorXd calcualtePrescribedEquivalentForceVector(std::map<int, double>& mConstraints,
	  Eigen::MatrixXd& globalStiffnessMatrix, int numOfDOFS) {
	Eigen::VectorXd globalPrescribedEquivalentForce = Eigen::VectorXd::Zero(numOfDOFS);
	for (auto constraint : mConstraints) {
		if (constraint.second != 0) {
			Eigen::VectorXd PrescribedConstraintForce = Eigen::VectorXd::Zero(numOfDOFS);
			for (int row = 0; row < numOfDOFS; row++) {
				PrescribedConstraintForce(row) = constraint.second * globalStiffnessMatrix(row, constraint.first - 1);
			}
			globalPrescribedEquivalentForce += PrescribedConstraintForce;
		}
	}
	return globalPrescribedEquivalentForce;
};


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
	std::map<int, double>mConstraints;
	std::map<int, double>mLoads;
	std::map<int, double>mTemperatures;
	//input Nodes
	mNodes[1] = Node(0, L, 1);
	mNodes[2] = Node(0, 0, 2);
	mNodes[3] = Node(L, L, 3);
	mNodes[4] = Node(2*L, L, 4);
	mNodes[5] = Node(3*L, L, 5);
	mNodes[6] = Node(3*L, 0, 6);



	//input trusses
	vTrusses.push_back(Truss(&mNodes[1] , &mNodes[3], A2, E,  1));
	vTrusses.push_back(Truss(&mNodes[2], &mNodes[3], A1, E,  2));
	vTrusses.push_back(Truss(&mNodes[3], &mNodes[4], A2, E, alpha, 3));
	vTrusses.push_back(Truss(&mNodes[4], &mNodes[5], A2, E,  4));
	vTrusses.push_back(Truss(&mNodes[4], &mNodes[6], A1, E, 5));


	//input Constraints
	mConstraints[1] = 0;
	mConstraints[2] = 0;
	mConstraints[3] = 0;
	mConstraints[4] = 0;
	mConstraints[9] = 0;
	mConstraints[10] = 0;
	mConstraints[11] = 0;
	mConstraints[12] = -1*U;

	//input Loads
	mLoads[6] = -P;
	mLoads[8] = -P;

	//Thermal delta Temperature at Element i
	mTemperatures[1] = deltaT;
	mTemperatures[3] = deltaT;
	mTemperatures[4] = deltaT;
	
	




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
	std::cout << "\n-------------------------------------------------------------------\n";


	//calculate Force Vector
	Eigen::VectorXd forceVector = Eigen::VectorXd::Zero(mNodes.size() * 2);
	for (auto load : mLoads) {
		forceVector[load.first - 1] = load.second;
	}
	std::cout << "Force Vector:\n";
	std::cout << forceVector << "\n";
	std::cout << "\n-------------------------------------------------------------------\n";
	//Calculate Thermal Force Vector
	Eigen::VectorXd thermalForceVector = calculateThermalLoadForceVector(mTemperatures, vTrusses, mNodes.size() * 2);
	std::cout << "Thermal Force Vector:\n";
	std::cout << thermalForceVector << "\n";
	std::cout << "\n-------------------------------------------------------------------\n";

	Eigen::VectorXd prescribedDisplacementLoads =
		calcualtePrescribedEquivalentForceVector(mConstraints, globalStiffnessMatrix, mNodes.size() * 2);
	std::cout << "Prescribed Displacement Force Vector:\n";
	std::cout << prescribedDisplacementLoads << "\n";
	std::cout << "\n-------------------------------------------------------------------\n";
	//add all Forces Vector
	forceVector = forceVector + thermalForceVector-prescribedDisplacementLoads;
	std::cout << "\n-------------------------------------------------------------------\n";

	std::cout << "Total Force Vector:\n";
	std::cout << forceVector << "\n";
	std::cout << "\n-------------------------------------------------------------------\n";

	/// IMPORTANT SHIT HERE
	//reduce Matrix After applying BC
	// By making rows & Columns 0s at index of the constraint
	// make 1 equal to where the row=column=DOF of constraint
	// Reduce the force Vector to 0 where the Boundary Conditions are applied.
	for (auto constraint : mConstraints) {
		for (int row = 0; row < mNodes.size()*2; row++) {
			globalStiffnessMatrix(row, constraint.first - 1) = 0;
			globalStiffnessMatrix(constraint.first - 1, row) = 0;
		}
		globalStiffnessMatrix(constraint.first - 1, constraint.first - 1)=1;
		forceVector(constraint.first - 1) = 0;

		
	}

	

	std::cout << "Modified Force Vector:\n";
	std::cout << forceVector<<"\n";
	std::cout << "\n-------------------------------------------------------------------\n";
	std::cout << "Reduced Stiffness Matrix:\n";
	std::cout << globalStiffnessMatrix;

	Eigen::VectorXd displacementsResult= globalStiffnessMatrix.inverse() * forceVector;
	std::cout << "\n-------------------------------------------------------------------\n";
	std::cout << "Displacement Vector Resultant:\n";
	std::cout << displacementsResult;


	//Thermal Load Force Vector

	

	return 0;
}