// ConsoleApplication2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <math.h>


int n = 3;
void bbq(int level)
{
	if (level == 2)
		return;


	for (size_t i = 0; i < 3; i++)
	{
		bbq(level + 1);
	}
	//bbq(level + 1);
	//bbq(level + 1);
	/*bbq(level + 1);*/
	std::cout << n;
}

int main()
{
	bbq(0);

	return 0;
}
