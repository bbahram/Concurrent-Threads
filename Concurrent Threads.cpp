#include <iostream>
#include <Windows.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <thread>
using namespace std;
//base**********************************
struct answers {
	int* best_cities = NULL;
	int bestdis;
	int ID;
	int city_num;
	int ttime;
	int trycount = 0;
};
//functions******************************
bool ifis(int x, int* selected_city, int city_num)
{
	for (int i = 0; i < city_num; i++)
		if (x == selected_city[i])
			return true;
	return false;
}

void random_select(int city_count, int* selected_city, int city_num, int process_num)
{
	int i = 0;
	while (i < city_num)
	{
		int x = rand() % city_count;
		if (!ifis(x, selected_city, i))
		{
			selected_city[i] = x;
			i++;
		}
	}
}

int find_max(int** cities, int city_count, int* selected_city, int city_num)
{
	int max = 0;
	for (int i = 0; i < city_count; i++)
	{
		if (i != selected_city[i])
		{
			int min = INT_MAX;
			for (int j = 0; j < city_num; j++)
				if (min > cities[i][selected_city[j]])
					min = cities[i][selected_city[j]];
			if (max < min)
				max = min;
		}
	}
	return max;
}
//thread********************************
DWORD WINAPI test(LPVOID param)
{
	answers* point = (answers*)param;
	int city_count, ** cities = NULL, Time, max, best = INT_MAX, *selected_city = NULL;
	srand(time(NULL) + point->ID);
	ifstream f;
	f.open("C:\\Users\\bahram\\Desktop\\os test file.txt");
	f >> city_count;
	cities = new int* [city_count];
	for (int i = 0; i < city_count; i++)
		cities[i] = new int[city_count];
	for (int i = 0; i < city_count; i++)
		for (int j = 0; j < city_count; j++)
			f >> cities[i][j];
	selected_city = new int[point->city_num];
	point->best_cities = new int[point->city_num];
	Time = time(NULL);
	while (time(NULL)-Time!=point->ttime)
	{
		point->trycount++;
		random_select(city_count, selected_city, point->city_num, point->ID);
		max = find_max(cities, city_count, selected_city, point->city_num);
		if (best > max)
		{
			best = max;
			for (int i = 0; i < point->city_num; i++)
				point->best_cities[i] = selected_city[i];
		}
	}
	point->bestdis = best;
	return 0;
}
//main****************************************
int main()
{
	unsigned const int cores = std::thread::hardware_concurrency();
	int min = INT_MAX, x=0, city_num, ttime;
	HANDLE* h = new HANDLE[cores];
	answers* th = new answers[cores];
	cout << "how many shops? "; cin >> city_num;
	cout << "how much time? "; cin >> ttime;
	for (int i = 0; i < cores; i++)
	{
		th[i].ID = i;
		th[i].city_num = city_num;
		th[i].ttime = ttime;
		h[i] = CreateThread(NULL, 0, test, &th[i], 0, NULL);
	}
	WaitForMultipleObjects(cores, h, false, INFINITE);
	cout << "best answers:" << endl;
	for (int i = 0; i < cores; i++)
	{
		cout << "thread " << i + 1 << "` answer:" << th[i].bestdis << endl;
		if (min > th[i].bestdis)
		{
			min = th[i].bestdis;
			x = i;
		}
	}
	cout << endl << "best distance: " << th[x].bestdis << endl << endl;
	cout << "best cities;" << endl;
	for (int i = 0; i < cores; i++)
		cout << "trycount: " << th[i].trycount << endl;
	for (int i = 0; i < cores; i++)
		CloseHandle(h[i]);
	system("Pause");
	return 0;
}