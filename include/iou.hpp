#include"box.hpp"
using namespace std;
 
float IOU(const bbox& b1, const bbox& b2)
{
	float w = std::min(b1.m_left + b1.m_width, b2.m_left + b2.m_width)
		      - std::max(b1.m_left, b2.m_left);
	float h = std::min(b1.m_top + b1.m_height, b2.m_top + b2.m_height)
		      - std::max(b1.m_top, b2.m_top);
 
 
	if (w <= 0 || h <= 0)
		return 0;
 
	std::cout << "w :" << w << "h :" << h << std::endl;
 
	return (w * h) / ((b1.m_height * b1.m_width) + (b2.m_height * b2.m_width) - (w * h));
}