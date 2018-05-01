//
// Created by root on 17-11-9.
//

#include <list>
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
using namespace std;
template <typename DataType>
class Digraph
{
public:
    DataType Data(int k) const;
    void Read(istream & inStream);
    void display(ostream & out);
    void DepthFirstSearch(int start);
    vector<int> shortestPath(int start, int destination);
private:
    class VertexInfo{
    public:
        DataType data;
        list<int> adjacencyList;
    };
private:
    vector<VertexInfo> myAdjacencyLists;
    void depthFirstSearchAux(int start, vector<bool> & unvisited);
};

template <typename DataType>
inline DataType Digraph<DataType>::Data(int k) const{
    return myAdjacencyLists[k].data;
}

template<typename DataType>
void Digraph<DataType>::Read(istream& inStream){
    Digraph<DataType>::VertexInfo vi;
    int n, vertex;
    myAdjacencyLists.push_back(vi);
    for(;;){
        inStream >> vi.data;
        if (inStream.eof()) break;
        inStream >> n;
        list<int> adjList;
        for(int i=1; i<=n; i++){
            inStream >> vertex;
            adjList.push_back(vertex);
        }
        vi.adjacencyList = adjList;
        myAdjacencyLists.push_back(vi);
    }
}

template<typename DataType>
void Digraph<DataType>::display(ostream &out) {
    out << "Adjacency-List Representation:\n";
    for(int i=1; i<myAdjacencyLists.size(); i++){
        out << i << ": " << myAdjacencyLists[i].data << "-";
        for(list<int>::iterator it = myAdjacencyLists[i].adjacencyList.begin();
                it != myAdjacencyLists[i].adjacencyList.end(); it++)
            out << *it << "     ";
        out << endl;
    }
}

template<typename DataType>
inline void Digraph<DataType>::depthFirstSearchAux(int start, vector<bool> &unvisited)  {
    cout << myAdjacencyLists[start].data << endl;
    unvisited[start] = false;
    for(list<int>::iterator it = myAdjacencyLists[start].adjacencyList.begin();
            it != myAdjacencyLists[start].adjacencyList.end(); it++){
        if (unvisited[*it])
            depthFirstSearchAux(*it, unvisited);
    }
}

template<typename DataType>
vector<int> Digraph<DataType>::shortestPath(int start, int destination) {
    int n = myAdjacencyLists.size();
    vector<int> distLabel(n, -1), predLabel(n);
    distLabel[start] = 0;
    int distance = 0, vertex;
    queue<int> vertexQueue;
    vertexQueue.push(start);
    while(distLabel[destination] < 0 && !vertexQueue.empty()) {
        vertex = vertexQueue.front();
        vertexQueue.pop();
        if (distLabel[vertex] > distance)
            distance++;
        for (list<int>::iterator it = myAdjacencyLists[vertex].adjacencyList.begin();
             it != myAdjacencyLists[vertex].adjacencyList.end(); it++) {
            if (distLabel[*it] < 0) {
                distLabel[*it] = distance + 1;
                predLabel[*it] = vertex;
                vertexQueue.push(*it);
            }
        }
    }
    distance++;
    vector<int> path(distance+1);
    if (distLabel[destination] < 0)
        cout << "Destination not reachable from start vertex\n";
    else{
        path[distance] = destination;
        for(int k = distance - 1; k >= 0; k--)
            path[k] = predLabel[path[k+1]];
    }
    return path;
}


int main(){
    std::string str = "A\n"
            "3\n"
            "2\n"
            "4\n"
            "5\n"
            "B\n"
            "1\n"
            "6\n"
            "C\n"
            "2\n"
            "2\n"
            "4\n"
            "D\n"
            "2\n"
            "3\n"
            "8\n"
            "E\n"
            "1\n"
            "8\n"
            "F\n"
            "2\n"
            "7\n"
            "9\n"
            "G\n"
            "2\n"
            "2\n"
            "3\n"
            "H\n"
            "0\n"
            "I\n"
            "1\n"
            "8";

    stringstream ss(str);
    Digraph<char> digraph;
    digraph.Read(ss);
    digraph.display(cout);
    vector<int> vector1 = digraph.shortestPath(1, 8);
    copy(vector1.begin(), vector1.end(), ostream_iterator<int>(cout, " "));

}