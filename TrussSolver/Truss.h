#pragma once
#include <iostream>
#include "Node.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>

class Truss {
private:
	Node* _node1;
	Node* _node2;
	double _A;
	double _E;
	double _thermalCoefficient;
	int _id;

public:
	Truss(Node* node1, Node* node2, double A, double E, int id);
	Truss(Node* node1, Node* node2, double A, double E,double thermalCoefficient, int id);
	friend std::ostream& operator << (std::ostream& os, const Truss& truss);
	
	double getArea();
	double getE();
	Node* getNode1();
	Node* getNode2();
	int getId();
	double getLength();
	double getThermalCoefficient();


	void setArea(double A);
	void setE(double E);
	void setNode1(Node* node);
	void setNode2(Node* node);
	void setId(int id);
	void setThermalCoefficient(double ThermalCoefficient);

	Eigen::Matrix4d getLocalMatrix();
	Eigen::Matrix4d getGlobalMatrix();
	Eigen::Vector4d getLocalThermalVectorLoad();
	Eigen::Vector4d getGlobalThermalVectorLoad();


};