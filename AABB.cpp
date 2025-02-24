#include<AABB.h>
#include<iostream>
AABB::~AABB()
{
	std::cout << "destroyed";
}

int AABB::cal() {
	return x * y;
}

