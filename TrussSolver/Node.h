#pragma once
#include <iostream>
class Node {
private:
	double _x, _y;
	int _id;
public:
	Node(double x, double y, int id);

	friend std::ostream& operator << (std::ostream& os, const Node& n);
	friend std::istream& operator >> (std::istream& is, Node& n);

	double getX();
	double getY();
	int getId();
	void setX(double x);
	void setY(double y);
	void setId(int id);
};