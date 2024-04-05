#include <iostream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iomanip>
#include <unistd.h>
#include <random>
using namespace std;

int nk = 5;
int lk = 5;
int ck = 20;
int sk = 10;
int errorTimes = 0;
class Vertex{
    public :
        float x, y;
        Vertex() {
            uniform_int_distribution<int> d(-20, 20);
            random_device rd("/dev/urandom");
            x = d(rd);
            y = d(rd);
        }
        Vertex(int ck) {
            uniform_int_distribution<int> d(-ck,ck);
            random_device rd("/dev/urandom");
            x = d(rd);
            y = d(rd);
        }
        Vertex(float xcoor,float ycoor) {
            x = xcoor;
            y = ycoor;
        }
        bool operator==(const Vertex &a) const
        {
            if (x == a.x && y == a.y)
                return true;
            return false;
        }
        bool operator!=(const Vertex &a) const
        {
            if (x != a.x || y != a.y)
                return true;
            return false;
        }
};

class Edge{
    public:
    Vertex src, dst;
    Edge(Vertex srco, Vertex dsto) {
        src = srco;
        dst = dsto;
    }
};

Vertex intersect (Edge l1,Edge l2,int ck);
class Street{
    public :
    int realck;
    string name;
    int realnk;
    vector<Edge> edges;
    vector<Vertex> vertexes;
    Street(){}
    Street(int nk,int ck){
        uniform_int_distribution<int> d3(1, nk);
        random_device rd3("/dev/urandom");
        realnk = d3(rd3); //nk
        realck = ck;
        name = getName();
        getCorrds();
        getEdges();
    }
    string getName(){
        int index = 0;
        char name[10];
        while(index < 10)
        {
            uniform_int_distribution<int> d(31, 123);
            random_device rd("/dev/urandom");
            char c = char(d(rd));
            if(index == 0 || index == 9){
                if(isalpha(c)){
                    name[index] = c;
                    index++;
                }
            }
            else if(isalpha(c) || isspace(c)){
                name[index] = c;
                index++;
            }  
        }
        string str(name);
        string sname = str.substr(0, 10);
        return sname;
    }
    void getCorrds(){
        int realcorrds = realnk + 1;
        Vertex vertex;
        for (int i = 0; i < realcorrds; i++)
        {
            vertex = Vertex(realck);
            vertexes.push_back(vertex);
        }
        int i, j;
        for (i = 0; i < realcorrds; i++){
            for (j = 0; j < realcorrds;j++){
                if(i != j){
                    while(vertexes[i] == vertexes[j]){
                        // cout << "re-corrds" << endl;
                        errorTimes++;
                        if(errorTimes == 25){
                            cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
                            exit(1);
                        }
                        vertexes[j] = Vertex(realck);
                        i = 0;
                        j = 1;
                        }
                    }   //1.check if there are repeated vectex in same street
                    errorTimes = 0;
                }
            }
        }
    void getEdges(){
        Vertex intersect (Edge l1,Edge l2,int ck);
        int i, j;
        for (i = 0; i < vertexes.size() - 1;i++){
            Edge edge = Edge(vertexes[i], vertexes[i + 1]);
            edges.push_back(edge);
        }
        for (i = 0; i < edges.size();i++){
            for (j = 0; j < edges.size();j++){
                if(i != j){
                    Vertex inPoint = intersect(edges[i], edges[j],realck);
                     while ((inPoint != edges[i].src && inPoint != edges[i].dst && inPoint != edges[j].src && inPoint != edges[j].dst)
                    ||(inPoint == edges[i].src && inPoint != edges[j].src && inPoint != edges[j].dst)
                    ||(inPoint == edges[i].dst && inPoint != edges[j].src && inPoint != edges[j].dst)
                    ||(inPoint == edges[j].dst && inPoint != edges[i].src && inPoint != edges[i].dst)
                    ||(inPoint == edges[j].src && inPoint != edges[i].src && inPoint != edges[i].dst)){
                        if((inPoint.x <= realck && inPoint.y <= realck)||(inPoint.x == realck + 2 && inPoint.y == realck + 2)){
                            //regenerate edge[j]: 2.intersection 3.overlap
                            int index = 0;
                            int k = 0;
                            for (k = 0; k < vertexes.size();k++){
                                if(vertexes[k] == edges[j].dst){
                                    index = k;
                                    break;
                                }
                            }
                            //先重新生成一个vertex,和原来剩下的进行对比，如果全都不一样，再赋值给vertex[k],如果有一样的就再次重新生成 
                            Vertex temp = Vertex(realck);
                            bool isSame = false;
                            while(true){
                                for(k = 0; k < vertexes.size();k++){
                                if(temp == vertexes[k])
                                    {
                                        isSame = true;
                                    }
                                }
                                if(isSame){
                                    temp = Vertex(realck);
                                    //重新生成在这里
                                    errorTimes++;
                                    if(errorTimes == 25){
                                        cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
                                        exit(1);
                                    }
                                }else{
                                    errorTimes = 0;
                                    vertexes[index] = temp;
                                    break;
                                }
                            }
                            edges[j] = Edge(edges[j].src,  vertexes[index]);
                            if(j < edges.size() - 1)
                                edges[j + 1] = Edge( vertexes[index], edges[j + 1].dst);
                            inPoint = intersect(edges[i], edges[j],realck);
                        }
                        else{
                            break;
                        }
                    }
                }
            }
        }
    }
    string output(){
        string corrd;
        string corrds = "";
        for (int i = 0; i < vertexes.size() ;i++){
            string cx = to_string(vertexes[i].x);
            int pointPos = cx.find(".");
            cx = cx.substr(0, pointPos);
            // cx = cx.substr(0, pointPos + 2 + 1);
            string cy = to_string(vertexes[i].y);
            pointPos = cy.find(".");
            cy = cy.substr(0, pointPos);
            // cy = cy.substr(0, pointPos + 2 + 1);
            if(i == vertexes.size() - 1)
                corrd = "(" + cx + "," +cy + ")";
            else
                corrd = "(" + cx + "," +cy + ") ";
            corrds = corrds + corrd;
        }
        return "add \"" + name + "\" " + corrds;
    }
};

class Database{
    int realnk = 5;
    int realsk = 10;
    int realck = 20;
    vector<Street> streets;
    public :
    Database(int sk,int nk,int ck){
        realck = ck;
    }
    void getStreets(){
        for(int i = 0;i < streets.size();i++){
            cout << "rm \"" << streets[i].name <<"\""<< endl;
        }
        streets.clear();
        uniform_int_distribution<int> d2(2, sk);
        random_device rd2("/dev/urandom");
        realsk = d2(rd2);//sk
        for (int i = 0; i < realsk;i++){
            Street street = Street(realnk, realck);
            streets.push_back(street);
        }
    }
    void checkOverlap(){
        int i, j,m ,n;
        for (i = 0; i < streets.size();i++){
            for (j = 0; j < streets.size();j++){
                if(i != j){
                    for (m = 0; m < streets[i].edges.size();m++){
                        for (n = 0; n < streets[j].edges.size();n++){
                            Edge l1 = streets[i].edges[m];
                            Edge l2 = streets[j].edges[n];
                            Vertex inPoint = intersect(l1, l2, realck);
                            if((inPoint.x == realck + 2 && inPoint.y == realck + 2)){
                                errorTimes++;
                                if(errorTimes == 25){
                                    cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
                                    exit(1);
                                }
                                // cout << streets[j].name << " re-street" << endl;
                                streets[j] = Street(realnk,realck);
                                i = 0;
                                j = 1;
                                n = 0;
                                m = 0;
                                break;
                            }
                            else{
                                errorTimes = 0;
                            }
                        }
                        if(i == j)
                            break;
                    }
                }
            }
        }
    }
    void fianlAns(){
        getStreets();
        checkOverlap();
        for (int i = 0; i < streets.size(); i++){
            cout << streets[i].output() << endl;
        }
        cout << "gg" <<endl;
    }
};

Vertex intersect (Edge l1,Edge l2,int ck){
    float xcoor, ycoor;
    int x1 = l1.src.x;
    int y1 = l1.src.y;
    int x2 = l1.dst.x;
    int y2 = l1.dst.y;
    int x3 = l2.src.x;
    int y3 = l2.src.y;
    int x4 = l2.dst.x;
    int y4 = l2.dst.y;
    Vertex noneVertex(ck + 1, ck + 1);
    Vertex isOverlap(ck + 2, ck + 2);
    int xnum = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    int xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    int ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    int yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if(xden != 0)
        xcoor = float(xnum) / float(xden);
    else
        if(xnum != 0)
            return noneVertex;
        else
            if(ynum == 0)
                return isOverlap;
            else
                return noneVertex;
    if(yden != 0)
        ycoor = float(ynum) / float(yden);
    else
        if(ynum != 0)
            return noneVertex;
        else
            if(xnum == 0)
                return isOverlap;
            else
                return noneVertex;
    if(xcoor>= min(x1, x2) && xcoor <= max(x1, x2) && xcoor >= min(x3, x4) && xcoor <= max(x3, x4)
    && ycoor >= min(y1, y2) && ycoor <= max(y1, y2) && ycoor >= min(y3, y4) && ycoor <= max(y3, y4)){
        if(xcoor == -0)
            xcoor = 0;
        if(ycoor == -0)
            ycoor = 0;
        return Vertex(xcoor, ycoor);
    }
    else
        return noneVertex;
}

template<class T>
void to_string(string & result,const T& t)
{
    ostringstream oss;//创建一个流
    oss << setprecision(2) << t; //把值传递如流中
    result=oss.str();//获取转换后的字符转并将其写入result
}

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

int main(int agrc, char** agrv){ 
    
    sk = stringToInt(agrv[1]);
    nk = stringToInt(agrv[2]);
    lk = stringToInt(agrv[3]);
    ck = stringToInt(agrv[4]);

    //sk在database里，nk在每个Street里，lk在每个循环里
    Database database(sk, nk, ck);
    while(true){
        uniform_int_distribution<int> d1(5, lk);
        random_device rd1("/dev/urandom");
        int reallk = d1(rd1); //lk
        database.fianlAns();
        sleep(reallk);
    }
    return 0;
}
