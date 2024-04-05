#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

using namespace std;

vector<int> iAndj;
vector<vector<int>> vAndp;
class Edge{
    public:
    int src, dst;
    Edge() {}
    Edge(int srco, int dsto) {
        src = srco;
        dst = dsto;
    }
};

class Graph{
    public :
    int vertex;
    vector<Edge> edges;
    Graph(int vNum,vector<Edge> outedges){
        vertex = vNum;
        edges = outedges;
    }
};

int stringToInt(string s){
    int ans = 0;
    int digit10times = 1;
    for (int i = 0; i < s.length()-1;i++){
        digit10times *= 10;
    }
    for (int i = 0; i < s.length();i++){
        if((s[i] >= '0')&&(s[i] <= '9')){
            int digit = s[i] - '0';
            digit = digit * digit10times;
            digit10times /= 10;
            ans += digit;
        }
    }
    return ans;
}

bool judgeValid(string s){
     for (int i = 0; i < s.length();i++){
        if((s[i] < '0')||(s[i] > '9')){
            cout << "Error:invalid number of vertex!" << endl;
            return false;
        }
    }
    return true;
}

bool CNFtoVertexCover(Graph G, int k){
    unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    int i,j;
    int n = G.vertex;
    Minisat::Lit x[n][k];
    for(i = 0; i < n; i++)
        for(j = 0; j < k; j++){
            x[i][j] = Minisat::mkLit(solver->newVar());
        }
    
    //∀i ∈ [1, k], a clause (x1,i ∨ x2,i ∨ · · · ∨ xn,i)
    for(i = 0;i < k; i++){
        Minisat::vec<Minisat::Lit> temp_clause;
        for(j = 0;j < n; j++)
           temp_clause.push(x[j][i]);
        solver -> addClause(temp_clause);
    }

    //∀m ∈ [1, n], ∀p, q ∈ [1, k] with p < q, a clause (¬xm,p ∨ ¬xm,q)
    for(int m = 0;m < n; m++)
        for(int p = 0;p < k; p++)
            for(int q = p + 1;q < k; q++){
                solver -> addClause(~x[m][p] , ~x[m][q]);
            }
    //∀m ∈ [1, k], ∀p, q ∈ [1, n] with p < q, a clause (¬xp,m ∨ ¬xq,m)
    for(int m = 0;m < k; m++)
        for(int p = 0;p < n; p++)
            for(int q = p + 1;q < n; q++){
                solver -> addClause(~x[p][m] , ~x[q][m]);
            }

    //∀<i, j> ∈ E, a clause (xi,1 ∨ xi,2 ∨ · · · ∨ xi,k ∨ xj,1 ∨ xj,2 ∨ · · · ∨ xj,k)
    for(int index = 0; index < G.edges.size(); index++){
        i = G.edges[index].src;
        j = G.edges[index].dst;
        Minisat::vec<Minisat::Lit> temp_clause;
        for(int index2 = 0; index2 < k; index2++)
            temp_clause.push(x[i - 1][index2]);
        for(int index2 = 0; index2 < k; index2++)
            temp_clause.push(x[j - 1][index2]);
        solver -> addClause(temp_clause);
    }

    bool res = solver->solve();
    if(res == (bool)1){
        for(i = 0; i < n; i++)
            for(j = 0; j < k; j++){
                if(Minisat::toInt(solver->modelValue(x[i][j])) == 0){
                    cout << i + 1 << " " ;
                }
            }
        cout << endl ;
    }
    solver.reset (new Minisat::Solver());
    return res;
}

int main(int argc, char **argv) {
    string command = "";
    string cmdStr;
    int vertexNum = 0;
    vector<Edge> edges;
    while (!cin.eof()) {
        getline(cin, command);
        if (command.size() > 0)
            cmdStr = string(command);
        if(cmdStr[0] == 'V'){
                int spacePos = command.find(" ");
                if(spacePos == -1){
                    cout << "Error:invalid command!" << endl;
                    continue;
                }
                string vNum = command.substr(spacePos+1, command.length()-spacePos-1);
                if(judgeValid(vNum))
                    vertexNum = stringToInt(vNum);
        }
        else if(cmdStr[0] == 'E'){
                if(vertexNum <= 1){
                    cout << "Error:invalid number of vertexes!" << endl;
                    continue;
                }
                int spacePos = command.find(" ");
                if(spacePos == -1){
                    cout << "Error:invalid command!" << endl;
                    continue;
                }
                int end = 0;
                int start = 0;
                edges.clear();
                string wholeEdge = command.substr(spacePos+2, command.length()-spacePos-3);
                while(wholeEdge != ""){
                    int lbra = wholeEdge.find("<");
                    int rbra = wholeEdge.find(">");
                    if((lbra == -1)||(rbra == -1)){
                        cout << "Error:invalid edges!" << endl;
                        continue;
                    }
                    string pair = wholeEdge.substr(lbra+1, rbra - lbra-1);
                    int comma = pair.find(",");
                    string first = pair.substr(0, comma);
                    string last = pair.substr(comma + 1, pair.length() - comma - 1);
                    if(judgeValid(first)&&judgeValid(last)){
                        start = stringToInt(first);
                        end = stringToInt(last);
                    }
                    else  continue;
                    if((start < 1)||(end < 1)||(start > vertexNum)||(end > vertexNum)){
                        cout << "Error:invalid edges!" << endl;
                        break;
                    }
                    Edge e = Edge(start,end);
                    edges.push_back(e);
                    wholeEdge = wholeEdge.substr(rbra + 1, wholeEdge.length() - rbra - 1);
            }
            Graph graph = Graph(vertexNum,edges);//读出图
            //iteration k ∈ [0, |V|].
            int k = 0;
            for(k = 0; k <= graph.vertex; k++)
                if(CNFtoVertexCover(graph, k) == 1)
                    break;
        }
        else{
            cout << "Error:invalid command!" << endl;      
        }
    }
    
    return 0;
}
