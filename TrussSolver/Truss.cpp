#include "Truss.h"
#include "math.h"
#include <Eigen/Dense>

Truss::Truss(Node* node1, Node* node2, double A, double E, int id):
	_node1(node1),_node2(node2),_A(A),_E(E),_id(id),_thermalCoefficient(0)
{
}

Truss::Truss(Node* node1, Node* node2, double A, double E, double thermalCoefficient, int id):
	_node1(node1), _node2(node2), _A(A), _E(E), _id(id), _thermalCoefficient(thermalCoefficient)
{
}

double Truss::getArea()
{
	return _A;
}

double Truss::getE()
{
	return _E;
}

Node* Truss::getNode1()
{
	return _node1;
}

Node* Truss::getNode2()
{
	return _node2;
}

int Truss::getId()
{
	return _id;
}

double Truss::getLength()
{
	return 	std::sqrt(std::pow((_node1->getX() - _node2->getX()), 2) +
		std::pow((_node1->getY() - _node2->getY()), 2));;
}

double Truss::getThermalCoefficient()
{
	return _thermalCoefficient;
}

void Truss::setArea(double A)
{
	_A = A;
}

void Truss::setE(double E)
{
	_E = E;
}

void Truss::setNode1(Node* node)
{
	_node1 = node;
}

void Truss::setNode2(Node* node)
{
	_node2 = node;
}

void Truss::setId(int id)
{
	_id = id;
}

void Truss::setThermalCoefficient(double ThermalCoefficient)
{
	_thermalCoefficient = ThermalCoefficient;
}

Eigen::Matrix4d Truss::getLocalMatrix()
{
	//stiffness matrix of a Truss
	Eigen::Matrix4d localMatrix(4,4);
	localMatrix.Constant(0.0);
	localMatrix(0, 0) = 1;
	localMatrix(2, 0) = -1;
	localMatrix(0, 2) = -1;
	localMatrix(2, 2) = 1;
	localMatrix* _A* _E / getLength();
	return localMatrix;
}

Eigen::Matrix4d Truss::getGlobalMatrix()
{
	Eigen::Matrix4d R;

	//calculate angle (in radians)
	double angleRot = std::atan2(_node1->getY() - _node2->getY(),
		_node1->getX() - _node2->getX());
	double c = cos(angleRot);
	double s = sin(angleRot);
	R(0, 0) = c*c;
	R(1, 0) = s * c;
	R(2, 0) = -c * c;
	R(3, 0) = -s * c;
	R(0, 1) = s * c;
	R(1, 1) = s * s;
	R(2, 1) = -s * c;
	R(3, 1) = -s * s;
	R(0, 2) = -c * c;
	R(1, 2) = -s * c;
	R(2, 2) = c * c;
	R(3, 2) = s * c;
	R(0, 3) = -s * c;
	R(1, 3) = -s * s;
	R(2, 3) = s * c;
	R(3, 3) = s * s;

	return R *_A*_E/getLength();
}

Eigen::Vector4d Truss::getLocalThermalVectorLoad()
{
	Eigen::Vector4d tLoad =Eigen::Vector4d::Zero();
	tLoad(0) = _E * _A * _thermalCoefficient;
	tLoad(2) =-1* _E * _A * _thermalCoefficient;
	return tLoad;
}

Eigen::Vector4d Truss::getGlobalThermalVectorLoad()
{
	Eigen::Vector4d tLoad = Eigen::Vector4d::Zero();
	//calculate angle (in radians)
	double angleRot = std::atan2(_node1->getY() - _node2->getY(),
		_node1->getX() - _node2->getX());
	double c = cos(angleRot);
	double s = sin(angleRot);

	tLoad(0) = _E * _A * _thermalCoefficient*c;
	tLoad(1) = _E * _A * _thermalCoefficient * s;
	tLoad(2) = -1*_E * _A * _thermalCoefficient * c;
	tLoad(3) = -1*_E * _A * _thermalCoefficient * s;
	
	return tLoad;

}



std::ostream& operator<<(std::ostream& os, const Truss& truss)
{
	os << "Truss : " << truss._id << ": From Node " << truss._node1->getId() <<
		" to Node " << truss._node2->getId();
	return os;
}
