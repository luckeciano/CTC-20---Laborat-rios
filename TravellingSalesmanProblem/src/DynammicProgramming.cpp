#include "DynammicProgramming.h"
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <map>
#include <windows.h>
#include <psapi.h>

using namespace std;

DynammicProgramming::DynammicProgramming(vector<int> verts, vector<vector<double> > matrix)
{
    //ctor
    vertices = verts;
    adjacencyMatrix = matrix;
}

DynammicProgramming::~DynammicProgramming()
{
    //dtor
}

int DynammicProgramming::SolveHeldKarp(double &cost)
{
    clock_t begin = clock();
    int startVertex = vertices.front();
    int bitmask = 1;
    int K = 1 << vertices.size();
    vector<vector<int> > memo (vertices.size(), vector<int>(K));
    for (int i = 0; i < vertices.size(); i++)
    {
        for (int j = 0; j < K ; j++)
        {
            memo[i][j] = -1;
        }
    }
    cost = TSP (startVertex, bitmask, memo);
    clock_t end = clock();
    cout << "Elapsed time in DynammicPrograming : " << (end-begin)/1000 << "." << (end-begin)%1000 << " s" << endl;
    return cost;

}
vector<int> DynammicProgramming::Solve(double &cost)
{
    clock_t begin = clock();
    int startVertex = vertices.front();
    unordered_set<int> hashSet(vertices.begin(), vertices.end());
    hashSet.erase(startVertex);

    Node root;
    cost = GetMinimumCostRoute(startVertex, hashSet, root);
    clock_t end = clock();
    cout << "Elapsed time in Brute Force: " << (end-begin)/1000 << "." << (end-begin)%1000 << " s" << endl;
    return TraverseTree(root, startVertex);

}

double DynammicProgramming::TSP (int startVertex,int bitmask, vector<vector<int> > &memo)
{

    if (bitmask == (1 <<(vertices.size()))-1) {
        if (adjacencyMatrix[startVertex][0] == -1)
            return numeric_limits<double>::max();
        return adjacencyMatrix[startVertex][0];
    }
    if (memo[startVertex][bitmask] != -1) {
        return memo[startVertex][bitmask];
    }

    double result = numeric_limits<double>::max();
    for (int i = 0; i <= vertices.size() - 1; i++) {
        if (i != startVertex && (bitmask & (1 << i)) == 0 && adjacencyMatrix[startVertex][i] != -1) {

            result = std::min(result, adjacencyMatrix[startVertex][i] + TSP(i, bitmask | (1 << i), memo));

        }
    }
    return result;
}

double DynammicProgramming::GetMinimumCostRoute(int startVertex, unordered_set<int> &hashSet, Node &root)
{
    if (hashSet.empty()) {
        Node node;
        node.Value = vertices.front();
        node.Selected = true;
        root.ChildNodes.push_back(node);
        if (adjacencyMatrix[startVertex][0] == -1)
            return 999999;
        return adjacencyMatrix[startVertex][0];
    }

    double totalCost = numeric_limits<double>::max();
    int i = 0;
    int selectIdx = i;

    for (const auto& destination: hashSet)    {


        if (adjacencyMatrix[startVertex][destination] != -1) {
            Node node;
            node.Value = destination;
            node.Selected = false;
            root.ChildNodes.push_back(node);


            double costOfVisitingCurrentNode = adjacencyMatrix[startVertex][destination];

            unordered_set<int> newHashSet = hashSet;

            newHashSet.erase(destination);

            double costOfVisitingOtherNodes = GetMinimumCostRoute(destination, newHashSet, root.ChildNodes[i]);
            double currentCost = costOfVisitingCurrentNode + costOfVisitingOtherNodes;

            if (totalCost > currentCost)
            {
                totalCost = currentCost;
                selectIdx = i;
            }
            else root.ChildNodes[i].ChildNodes.clear();
            i++;

        }
    }
    hashSet.clear();
    //problema here
    if (root.ChildNodes.size() !=0)
        root.ChildNodes[selectIdx].Selected = true;
    return totalCost;
}

vector<int> DynammicProgramming::TraverseTree(Node &root, int startInt)
{

    queue<int> q;
    q.push(startInt);
    TraverseTreeUtil(root, q);

    vector<int> result;
    while (!q.empty())
    {
        result.push_back(q.front());
        q.pop();
    }
    return result;
}

void DynammicProgramming::TraverseTreeUtil(Node &root, queue<int> &vertics)
{
    if (root.ChildNodes.empty())
    {
        return;
    }
    for (auto& child:root.ChildNodes)
    {
        if(child.Selected)
        {
            vertics.push(child.Value);
            TraverseTreeUtil(child, vertics);
        }
    }
}
bool DynammicProgramming::CheckHamiltonian(vector<int> &route, double &brute_cost)
{
    if (route.size() != vertices.size() + 1 || brute_cost > 999999 ) {
        cout << "Grafo nao hamiltoniano." << endl;
        return false;
    }
    return true;
}

struct c_unique {
  int current;
  c_unique() {current=0;}
  int operator()() {return current++;}
} UniqueNumber;
int main()
{
         // How to get a number.
     int order = 0;
        string input = "";
     while (true) {
       cout << "Digite a ordem do grafo para teste: ";
       std::getline(cin, input);

       // This code converts from string to number safely.
       stringstream myStream(input);
       if (myStream >> order)
         break;
       cout << "Numero invalido, tente novamente." << endl;
     }
     cout << "Gerando Grafo de ordem " << order << endl << endl;

    vector<int> vertics;
    vertics.resize(order);
    generate (vertics.begin(), vertics.end(), UniqueNumber);

    //Para apenas 10 arestas - ! Entrar ordem 6 no console !
    vector<vector<double> > adjMatrix;
    adjMatrix.resize(order);
    for (int i = 0; i < order; ++i)
        adjMatrix[i].resize(order);
    for (int i = 0; i < order; ++i){
        for (int j = 0; j < order; ++j)
            adjMatrix[i][j] = -1;
    }
    for (int k = 0; k < 10; ++k){
        int m = rand() % 6;
        int n = rand() % 6;
        int value;
        if (m>n){
            value = rand() % 10 +1;
            adjMatrix[m][n] = double(value);
            adjMatrix[n][m] = double(value);
        }
        else
            k=k-1;
    };


    // Para tabela de arestas completa

    /*vector<vector<double> > adjMatrix;
    adjMatrix.resize(order);
    for (int i = 0; i < order; ++i)
        adjMatrix[i].resize(order);
    for (int i = 0; i < order; ++i){
        for (int j = 0; j < order; ++j)
            if (i!=j)
                adjMatrix[i][j] = double(rand() % 10 +1);
            else
                adjMatrix[i][j] = -1;
    }*/

    // Show the randoms
    cout << "Vetor gerado : ";
    for (int i = 0; i < order; ++i)
        cout << " " << vertics[i];
    cout << endl;
    cout << "Matriz gerada : " << endl;
    for (int i = 0; i < order; ++i){
        for (int j = 0; j < order; ++j)
            cout << " " << setw(3) << adjMatrix[i][j];
        cout << endl;
    }




    DynammicProgramming dynammicProgramming(vertics, adjMatrix);
    double cost, brute_cost;
    vector<int> route = dynammicProgramming.Solve(brute_cost);
    bool isHamiltonian = true;
    isHamiltonian = dynammicProgramming.CheckHamiltonian(route, brute_cost);
    if (isHamiltonian) {
        cout << "Cost Held Karp: " << dynammicProgramming.SolveHeldKarp(cost) << endl;
        cout << "Cost Brute Force: " << brute_cost << endl << endl << endl;
        cout << "Route - " << vertics.size() << " verts"  << endl;
        for (const auto& elem: route)
            {cout << elem << " " ;}
        cout << endl;

    }

    return 0;

}
