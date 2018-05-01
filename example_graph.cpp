//
// Created by root on 17-11-10.
//

#include <vector>
#include <iostream>
using namespace std;
template <typename DataType>
class Graph
{
public:
    DataType data(int k) const;
    void read(istream & in, int numVertices, int numEdges);
    void depthFirstSearch(int start, vector<bool> & unvisited);
    bool isConnected();
private:
    class EdgeNode{
    public:
        int vertex[3];
        EdgeNode* link[3];
    };
    typedef EdgeNode *EdgePointer;
    class VertexInfo{
    public:
        DataType data;
        EdgePointer first;
    };
    vector<VertexInfo> myEdgeLists;
    void depthFirstSearchAux(int start, vector<bool> & unvisited);
};

template <typename DataType>
inline DataType Graph<DataType>::data(int k) const {
    return myEdgeLists[k].data;
}

template <typename DataType>
void Graph<DataType>::read(istream &in, int numVertices, int numEdges) {
    Graph<DataType>::VertexInfo vi;
    myEdgeLists.push_back(vi);
    cout << "Enter label of vertex:\n";
    for(int i=1; i<=numVertices; i++){
        cout << "       " << i << ":     ";
        in >> vi.data;
        vi.first = 0;
        myEdgeLists.push_back(vi);
    }
    int endpoint;
    cout << "Enter endpoints of edge\n";
    for(int i=1; i<=numEdges; i++){
        cout << "    " << i << ":     ";
        EdgePointer newPtr = new EdgeNode;
        for(int j=1; j<=2; j++){
            in >> endpoint;
            newPtr->vertex[j] = endpoint;
            newPtr->link[j] = myEdgeLists[endpoint].first;
            myEdgeLists[endpoint].first = newPtr;
        }
    }
}

bool anyLeft(const vector<bool> & unvisited){
    for(int i=1; i<unvisited.size(); i++){
        if (unvisited[i])
            return true;
    }
    return false;
}

template <typename DataType>
void Graph<DataType>::depthFirstSearch(int start, vector<bool> &unvisited) {
    unvisited[start] = false;
    Graph<DataType>::EdgePointer ptr = myEdgeLists[start].first;
    while(anyLeft(unvisited) && ptr != 0){
        int startEnd = 1,
            otherEnd = 2;
        if (ptr->vertex[1] != start){
            startEnd = 2;
            otherEnd = 1;
        }
        int newStart = ptr->vertex[otherEnd];
        if (unvisited[newStart])
            depthFirstSearch(newStart, unvisited);
        ptr = ptr->link[startEnd];
    }
}

template <typename DataType>
inline bool Graph<DataType>::isConnected() {
    vector<bool> unvisited(myEdgeLists.size(), true);
    depthFirstSearch(1, unvisited);
    return !anyLeft(unvisited);
}

int main(){
    int numVertices, numEdges;
    cout << "Enter number of vertices and number of edges in graph: ";
    cin >> numVertices >> numEdges;
    Graph<char> g;
    
}