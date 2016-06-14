#include "DynammicProgramming.h"
#include <limits>
#include <iostream>
#include <ctime>

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

vector<int> DynammicProgramming::Solve(double &cost)
{
    clock_t begin = clock();
    int startVertex = vertices.front();
    unordered_set<int> hashSet(vertices.begin(), vertices.end());
    hashSet.erase(startVertex);

    Node root;
    cost = GetMinimumCostRoute(startVertex, hashSet, root);
    clock_t end = clock();
    cout << "Elapsed time in DynammicPrograming : " << (end-begin)/1000 << "." << (end-begin)%1000 << " s" << endl;
    return TraverseTree(root, startVertex);

}
vector<int> DynammicProgramming::getBestRoute(double &cost, vector<int> &cities, int &root, int where_id){
    //cin.get();
    //cout << "getBestRoute => cost: " << cost << " => root: " << root << " => where_id: "<< where_id << endl << "Cities => ";
    //for (unsigned i=0; i<cities.size(); ++i)
    //    cout << ' ' << cities[i];
    //cout << endl;
    vector<int> result, local;
    local = cities;
    local[where_id]=-1;
    //cout << "Local => ";
    //for (unsigned i=0; i<local.size(); ++i)
    //    cout << ' ' << local[i];
    //cout << endl;
    int from = where_id;
    /*for (const auto& i: local){
        if (local[i] != -1){
            from = i;
            break;
        }
    }*/
    if (root == -1)
        root = from;
    int any = 0;
    double temp_cost = -1;
    double min_cost = -1;
    int id_min = -1;
    vector<int> temp_route, min_route;
    for (int i=0;i<local.size();i++){
        if (local[i] != -1){
            any = any+1;
            temp_route = getBestRoute(temp_cost, local, root, i);
            if (where_id==0){
                cout << "local[i] => "<< local[i] << endl;
                cout << temp_cost << " + " <<adjacencyMatrix[from][i] << " = " << (temp_cost + adjacencyMatrix[from][i]) << endl;
                cout << "menor que ? min_cost => " << min_cost << endl;
            }
            if ((min_cost==-1) || ((temp_cost + adjacencyMatrix[from][i]) < min_cost)){

                min_cost = temp_cost + adjacencyMatrix[from][i];
                min_route = temp_route;
                id_min = i;
                if (where_id==0)
                    cout << "SIMM!!" << endl;
            }

            temp_cost = -1;
            temp_route.clear();

        }
        //cout << any <<endl;
    }
    if (any==0){
        result.push_back(root);
        result.push_back(from);
        cost = adjacencyMatrix[from][root];
        cout << "Last stop ("<<from<<":"<<root<<") => cost: " << cost << " => route : ";
        for (unsigned i=0; i<result.size(); ++i)
            cout << ' ' << result[i];
        cout << endl;
        return result;
    }

    cost = min_cost;// + adjacencyMatrix[from][id_min];
    result = min_route;
    result.push_back(from);
    cout << "Middle Stop Min=("<<from<<":"<<id_min<<") => cost: " <<
    min_cost << " + " << adjacencyMatrix[from][id_min] << " = " << cost << " => route : ";
    for (unsigned i=0; i<result.size(); ++i)
        cout << ' ' << result[i];
    cout << endl;
    return result;

}

/*vector<int> DynammicProgramming::getBestRoute(double &cost, vector<int> &cities, int &root, int where_id){
    vector<int> result, local;
    local = cities;
    //local = vertices;
    local[where_id]=-1;
    from = -1;
    for (const auto& i: local){
        if (local[i] != -1){
            from = i;
            break;
        }
    }
    //int from = local.top();
    //local.pop();
    if (root == -1)
        root = from;
    int any = 0;
    for (const auto& i: local){

        if (local[i] != -1){
            getBestRoute(double &cost, vector<int> &cities, int &root, int where_id)
        }
    }

    if (local.empty()){
        result.push_back(root);
        cost = adjacencyMatrix[from][root];
        return result;
    } else{
        int temp_cost = -1;
        int min_cost = -1;
        int id_min = -1;
        vector<int> temp_route, min_route;
        for (const auto& i: local){
            temp_route = getBestRoute(temp_cost,local,root,i);
            if (min_cost== -1 || temp_cost < min_cost){
                min_cost = temp_cost;
                min_route = temp_route;
                id_min = i;
            }
            temp_cost = -1;
            temp_route.clear();
        }
        cost = min_cost + adjacencyMatrix[];
        return min_route;
    }

}
*/
vector<int> DynammicProgramming::BruteForce(double &cost)
{
    clock_t begin = clock();
    //vector<int> cities;
    //for (const auto& vert: vertices){
    //   cities.push_back(vertices[vert]);
    //}
    int root = -1;
    vector<int> result = getBestRoute(cost,vertices, root, 0);


    /*int startVertex = vertices.front();
    unordered_set<int> hashSet(vertices.begin(), vertices.end());
    hashSet.erase(startVertex);
    Node root;
    cost = GetMinimumCostRoute(startVertex, hashSet, root);*/


    clock_t end = clock();
    cout << "Elapsed time in Brute Force : " << (end-begin)/1000 << "." << (end-begin)%1000 << " s" << endl;
    return result;

}

double DynammicProgramming::GetMinimumCostRoute(int startVertex, unordered_set<int> &hashSet, Node &root)
{
    if (hashSet.empty()) {
        Node node;
        node.Value = vertices.front();
        node.Selected = true;
        root.ChildNodes.push_back(node);
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
            i++;
        }
    }
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
bool DynammicProgramming::CheckHamiltonian(vector<int> &route)
{
    if (route.size() != vertices.size() + 1) {
        cout << "Grafo nao hamiltoniano." << endl;
        return false;
    }
    return true;
}

int DynammicProgramming::vertices_size()
{
    return vertices.size();
}

int main()
{
    //exemplo hamiltoniano simple
    vector<int> vertics  = {0,1,2,3};
    vector<vector<double> > adjMatrix = {
        {-1, 10, 15, 20},
        {5, -1 , 9, 10},
        {6, 13, -1, 12},
        {8, 8, 9, -1},
    };
    /*//exemplo hamiltoniano
    vector<int> vertics  = {0,1,2,3, 4, 5, 6, 7, 8, 9};
    vector<vector<double> > adjMatrix = {
        {-1, 10, 15, 20, 0, 10, 15, 20, 0 , 10},
        {5, -1 , 9, 10, 5, 0, 9, 10, 5, 9},
        {6, 13, -1, 12, 6, 13, 9, 12, 6, 13},
        {8, 8, 9, -1, 8, 8, 9, 0, 8, 8},
        {0, 10, 15, 20, -1, 10, 15, 20, 0 , 10},
        {5, 0 , 9, 10, 5, -1, 9, 10, 5, 9},
        {6, 13, 9, 12, 6, 13, -1, 12, 6, 13},
        {8, 8, 9, 0, 8, 8, 9, -1, 8, 8},
        {6, 13, 9, 12, 6, 13, 9, 12, -1, 13},
        {8, 8, 9, 0, 8, 8, 9, 0, 8, -1},
    };*/

    //exemplo nao hamiltoniano

   /* vector<int> vertics = {0,1,2,3,4,5};
    vector<vector<double> > adjMatrix = {
        {-1,-1,-1,-1,1,1},
        {-1,-1,-1,-1,1,1},
        {-1,-1,-1,-1,1,1},
        {-1,-1,-1,-1,1,1},
        {1,1,1,1,-1,-1},
        {1,1,1,1,-1,-1}
        };*/

    DynammicProgramming dynammicProgramming(vertics, adjMatrix);
    double cost, brute_cost;
    vector<int> route = dynammicProgramming.Solve(cost);
    bool isHamiltonian = true;
    isHamiltonian = dynammicProgramming.CheckHamiltonian(route);
    if (isHamiltonian) {
        // Print Dynammic Result
        cout << "Route (Dynammic) - " << vertics.size() << " verts"  << endl;
        for (const auto& elem: route)
            {cout << elem << " " ;}
        cout << endl;
        cout << "Cost: " << cost << endl;
        // Call and print Brute Force
        vector<int> brute_route = dynammicProgramming.BruteForce(brute_cost);
        cout << "Route (Brute) - " << vertics.size() << " verts"  << endl;
        for (const auto& elem: brute_route)
            {cout << elem << " " ;}
        cout << endl;
        cout << "Cost: " << brute_cost << endl;
    }

    return 0;

}
