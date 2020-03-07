#pragma once
#include <iostream>

#include "Node.h"
class Truss {
private:
	Node* _node1;
	Node* _node2;
	double _A;
	double _E;
	int _id;
public:
	Truss(Node* node1, Node* node2, double A, double E, int id);

	friend std::ostream& operator << (std::ostream& os, const Truss& truss);
	
	double getArea();
	double getE();
	Node* getNode1();
	Node* getNode2();
	int getId();
	double getLength();


	void setArea(double A);
	void setE(double E);
	void setNode1(Node* node);
	void setNode2(Node* node);
	void setId(int id);


};