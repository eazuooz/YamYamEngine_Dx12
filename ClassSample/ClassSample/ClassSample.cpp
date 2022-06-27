// ConsoleApplication2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <math.h>

int map[7][7] =
{
   3,3,0,1,2,3,3,
   3,2,0,0,0,0,3,
   0,0,1,0,0,1,2,
   0,0,0,0,0,0,0,
   1,0,0,1,0,0,0,
   3,2,0,0,0,1,3,
   3,3,1,0,2,3,3,
};

struct Node
{
    int y = 0;
    int x = 0;

    int used[7][7] = {};
    int value = -1;
};

struct Feed
{
    int value;
    int count;
};

Node queue[256];

int head = 0;
int tail = 0;

int cnt = 0;

void Init()
{
    head = 0;
    tail = 1;

    for (size_t i = 0; i < 256; i++)
    {
        queue[i].y = 0;
        queue[i].x = 0;
        queue[i].value = -1;

        for (size_t y = 0; y < 7; y++)
        {
            for (size_t x = 0; x < 7; x++)
            {
                queue[i].used[y][x] = 0;
            }
        }

    }
}

bool gExist = false;

void bfs(int startY, int startX)
{
    int direct[4][2] =
    {
       0,1,
       1,0,
       -1,0,
       0,-1,
    };

    queue[head].y = startY;
    queue[head].x = startX;
    queue[0].used[startY][startX] = 1;

    int feed = map[startY][startX];

    //내 시작위치가 벽이거나 비어있는곳이면
    // 길을찾을 필요가 없기때문에 함수를 그냥 빠져나온다
    if (map[startY][startX] == 3
        || map[startY][startX] == 0)
    {
        return;
    }
    
    while (tail != head)
    {
        Node now = queue[head];
        head += 1;

        // 거리가 문어는 3칸이상
        // 새우는 2칸이상 넘어가면 더이상 길을 찾을 필요가 없음
        int disX = abs(now.x - startX);
        int disY = abs(now.y - startY);
        int dis = disX + disY;
        if (feed == 2 && dis > 3)
        {
            break;
        }
        else if (feed == 1 && dis > 2)
        {
            break;
        }

        //같은 해산물이 나오면 존재를 bool변수로 체크
        if ( map[now.y][now.x] == feed
            && now.y != startY && now.x != startX)
        {
            gExist = true;
            break;
        }



        for (size_t i = 0; i < 4; i++)
        {
            int newY = now.y + direct[i][0];
            int newX = now.x + direct[i][1];

            //배열 범위 체크
            if (newY >= 7 || newY < 0)
                continue;
            if (newX >= 7 || newX < 0)
                continue;
            //같은 먹이가 아닐경우 길을 찾을 필요가없음
            if (map[newY][newX] != 0
                && map[newY][newX] != feed)
                continue;
            //벽 체크
            if (map[newY][newX] == 3)
                continue;
            //지나온 경로체크
            if (now.used[newY][newX] == 1)
                continue;


            queue[tail] = now;
            queue[tail].y += direct[i][0];
            queue[tail].x += direct[i][1];

            int cy = queue[tail].y;
            int cx = queue[tail].x;

            queue[tail].used[cy][cx] = 1;
            tail++;
        }
    }

    return;
}

int main()
{
    Feed maxTile;
    maxTile.value = 0;
    maxTile.count = 0;
    for (size_t y = 0; y < 7; y++)
    {
        for (size_t x = 0; x < 7; x++)
        {
            Init();
            bfs(y, x);
        }
    }

    if (gExist == true)
    {
        //fail
    }
    else
    {
        //pass
    }

    return 0;
}