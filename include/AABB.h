#pragma once

class AABB
{
public:
	int x;
	int y;
	AABB(int x_,int y_) :x(x_),y(y_) {};
	~AABB();
	int  cal();
private:

};

