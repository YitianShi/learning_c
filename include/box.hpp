#include<iostream>
#include<algorithm>
#include<vector>

struct bbox
{
	int m_left;
	int m_top;
	int m_width;
	int m_height;
 
	bbox() {}
	bbox(int left, int top, int width, int height) 
	{
		m_left = left;
		m_top = top;
		m_width = width;
		m_height = height;
	}
};