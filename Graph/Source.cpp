//  MATT RUTTER
//  CSCI 250   FINAL PROJECT


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int max = 625;
const int maxS = 25;
struct edge //There will be an array of these to build the list of edges
{
	int to, from, weight;
};

class Graph 
{
private:
	int** adjacencyMatrix;
	int vertexCount;

public:							  //edgenum is the number of edges in the graph, unum is the number
	int edgenum, unum, set[maxS]; //of edges in the MST, and set is array labeling system
	edge e[max], unions[max]; //e is the array of all the edges, unions is the array of the MST edges
	Graph(int vertexCount) 
	{
		this->vertexCount = vertexCount;
		adjacencyMatrix = new int*[vertexCount];
		for (int i = 0; i < vertexCount; i++) 
		{
			adjacencyMatrix[i] = new int[vertexCount];
			for (int j = 0; j < vertexCount; j++)
				adjacencyMatrix[i][j] = 0;
		}
	}

	void addEdge(int i, int j, int weight)
	{
		if (i >= 0 && i < vertexCount && j > 0 && j < vertexCount)
		{
			adjacencyMatrix[i][j] = weight;
			adjacencyMatrix[j][i] = weight;
		}
	}

	void removeEdge(int i, int j)
	{
		if (i >= 0 && i < vertexCount && j > 0 && j < vertexCount)
		{
			adjacencyMatrix[i][j] = 0;
			adjacencyMatrix[j][i] = 0;
		}
	}

	bool isEdge(int i, int j)
	{
		if (i >= 0 && i < vertexCount && j > 0 && j < vertexCount)
			return (adjacencyMatrix[i][j] > 0);
		else
			return false;
	}

	int edgeWeight(int i, int j)
	{
		if (isEdge(i, j))
			return adjacencyMatrix[i][j];
		else
			return 0;
	}

	void printGraph()
	{
		int i, j = 0;
		cout << setw(3) << " ";
		for (int i = 0; i < vertexCount; i++)
			cout << setw(3) << i;
		cout << endl;
		for (i = 0; i < vertexCount; i++)
		{
			cout << setw(3) << i;
			for (int j = 0; j < vertexCount; j++)
				cout << setw(3) << adjacencyMatrix[i][j];
			cout << endl;
		}
	}
};

void swapem(edge &a, edge &b)
{
	edge temp;
	temp = a;
	a = b;
	b = temp;
}

void sortem(edge e[], int num)  //Sorting the arrays with bubble sort
{
	int i, j;
	for (j = 0; j < num - 1; j++)
		for (i = 0; i < num - 1; i++)
			if (e[i].weight > e[i + 1].weight)
				swapem(e[i], e[i + 1]);
}

void printem(edge u[], int &num) //printing the arrays
{
	int i;
	for (i = 0; i < num; i++)
		cout << right << endl << setw(3) << u[i].from << setw(3) << u[i].to 
		<< setw(5) << u[i].weight;
	cout << endl;
}

int find(int set[], int &num) //This function recursivly finds the original tree label
{							  //so it can determine if the node is already labeled
	if (set[num] < 0)		  //as part of a tree, if not, it creates a new tree label
		return num;			  //The label is found in the set[] list
	else
		return find(set, set[num]); //The node will be labeled if it's already in a tree so the 
}									//function keeps going until it finds what tree it's a part of

void kruskal(Graph &g) //This is of course the Minimum Spanning Tree function using 
{					   //Kruskal's Algorithm
	int i, a, b, temp;
	g.unum = 0;
	for (i = 0; i < g.edgenum; i++)
	{
		a = find(g.set, g.e[i].from); //This finds a label for the a and b nodes each
		b = find(g.set, g.e[i].to);
		if (a != b)  //Detects cycles by grouping each node to a tree label
		{
			if (g.set[a] <= g.set[b]) //This if statement labels which
			{						  //tree group the segment is going to be
				temp = g.set[a];	  //a part of (whichever tree is more prevailent)
				g.set[a] = b;
				g.set[b] += temp;
			}
			else
			{
				temp = g.set[b];
				g.set[b] = a;
				g.set[a] += temp;
			}
			g.unions[g.unum].from = g.e[i].from;
			g.unions[g.unum].to = g.e[i].to;
			g.unions[g.unum].weight = g.e[i].weight;
			g.unum++;
		}
	}
	printem(g.unions, g.unum);
}

int treesum(Graph &g)
{
	int sum = 0, i;
	for (i = 0; i < g.unum; i++)
		sum += g.unions[i].weight;
	return sum;
}

void main()
{
	Graph a(10), b(25);
	int i, j, weight;
	for (i = 0; i < maxS; i++) //This initializes all the lables in set[] to -1
		a.set[i] = -1;
	for (i = 0; i < maxS; i++)
		b.set[i] = -1;
	ifstream inf;
	inf.open("Graph.dat");
	for (j = 0; j < 10; j++)
		for (i = 0; i < 10; i++)
		{
			inf >> weight;
			a.addEdge(i, j, weight);
			if (weight > 0 && j < i) //This if statement builds the 
			{						 //list of edges with weight
				a.e[a.edgenum].weight = weight;
				a.e[a.edgenum].to = i;
				a.e[a.edgenum].from = j;
				a.edgenum++;
			}
		}
	a.printGraph();
	sortem(a.e, a.edgenum);
	kruskal(a);	
	cout << "Tree sum a: " << treesum(a);
	cout << endl << endl;
	for (j = 0; j < 25; j++)
		for (i = 0; i < 25; i++) 
			{
			inf >> weight;
			b.addEdge(i, j, weight);
			if (weight > 0 && j < i) //This if statement builds the 
			{					     //list of edges with weight
				b.e[b.edgenum].weight = weight;
				b.e[b.edgenum].to = i;
				b.e[b.edgenum].from = j;
				b.edgenum++;
			}
		}
	b.printGraph();
	sortem(b.e, b.edgenum);
	kruskal(b);
	cout << "Tree sum b: " << treesum(b) << endl;
	system("pause");
}