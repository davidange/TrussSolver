#include "Truss.h"
#include "math.h"

Truss::Truss(Node* node1, Node* node2, double A, double E, int id):
	_node1(node1),_node2(node2),_A(A),_E(E),_id(id)
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

std::ostream& operator<<(std::ostream& os, const Truss& truss)
{
	os << "Truss : " << truss._id << ": " << "From Node " << truss._node1->getId() <<
		"to Node " << truss._node2->getId();
	return os;
}
