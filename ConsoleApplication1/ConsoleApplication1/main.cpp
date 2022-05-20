#include <stdio.h> 
#include <iostream>
#include <vector> 
#include <algorithm> 
#include <stack> 


using namespace std;

vector<int> adjList[1001];
bool isVisited[1001] = {};
stack<int> st;


void dfs(int level)
{
	if (isVisited[level] == true)
		return;

	std::cout << level;

}

int main()
{




	return 0;
}