#include "Node.h"

Node::Node(double x, double y):_x(x),_y(y)
{
	
}

double Node::getX()
{
	return _x;
}

double Node::getY()
{
	return _y;
}

void Node::setX(double x)
{
	_x = x;
}

void Node::setY(double y)
{
	_y = y;
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	os << "(" << n._x << "," << n._y << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Node& n)
{
	is >> n._x;
	is >> n._y;
	// TODO: insert return statement here
}
