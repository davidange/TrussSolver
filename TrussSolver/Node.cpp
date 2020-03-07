#include "Node.h"

Node::Node(double x, double y, int id):_x(x),_y(y),_id(id)
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

int Node::getId()
{
	return _id;
}

void Node::setX(double x)
{
	_x = x;
}

void Node::setY(double y)
{
	_y = y;
}

void Node::setId(int id)
{
	_id = id;
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
	os << "Node "<< n._id<< ": (" << n._x << "," << n._y << ")";
	return os;
}

std::istream& operator>>(std::istream& is, Node& n)
{
	is >> n._x;
	is >> n._y;
	is >> n._id;
	// TODO: insert return statement here
}
