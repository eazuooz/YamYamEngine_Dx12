#include "pch.h"
#include <stdio.h> 
#include <iostream>
#include <vector> 
#include <queue>
#include <algorithm> 
#include <stack> 


using namespace std;
int N, M, S;
vector<int> adjList[1001];
bool isVisited[1001] = {};
stack<int> st;
queue<int> que;


void dfs(int value)
{
	if (isVisited[value] == true)
		return;
	isVisited[value] = true;

	std::cout << value;
	for (size_t i = 0; i < adjList[value].size(); i++)
	{
		int next = adjList[value][i];
		dfs(next);
	}
}

void dfs2(int value)
{
	st.push(value);

	while (!st.empty())
	{
		int cur = st.top();

		st.pop();

		if (isVisited[cur])
			continue;

		isVisited[cur] = true;
		cout << cur;

		for (size_t i = 0; i < adjList[cur].size(); i++)
		{
			int next = adjList[cur][i];
			st.push(next);
		}
	}
}

void bfs(int v)
{
	que.push(v);

	while (!que.empty())
	{
		int cur = que.front();
		que.pop();

		if (isVisited[cur])
			continue;

		isVisited[cur] = true;
		cout << cur;

		for (int i = 0; i < adjList[cur].size(); i++)
		{
			// 인접 노드들 삽입 
			int next = adjList[cur][i];
			que.push(next);
		}
	}
}


int main()
{
	adjList[1].push_back(2);
	adjList[1].push_back(3);
	adjList[1].push_back(8);

	adjList[2].push_back(1);
	adjList[2].push_back(7);

	adjList[3].push_back(1);
	adjList[3].push_back(4);
	adjList[3].push_back(5);

	adjList[4].push_back(3);
	adjList[4].push_back(5);

	adjList[5].push_back(3);
	adjList[5].push_back(4);

	adjList[6].push_back(7);

	adjList[7].push_back(2);
	adjList[7].push_back(6);
	adjList[7].push_back(8);

	adjList[8].push_back(1);
	adjList[8].push_back(7);

	// sort 
	for (int i = 0; i < 1001; i++)
	{
		sort(adjList[i].begin(), adjList[i].end());
	}

	fill_n(isVisited, 1001, false);
	// 방문 여부 초기화 
	dfs(1);
	fill_n(isVisited, 1001, false);
	// 방문 여부 초기화 
	dfs2(1);

	fill_n(isVisited, 1001, false); // 방문 여부 초기화 bfs(S);
	bfs(S);




	return 0;
}