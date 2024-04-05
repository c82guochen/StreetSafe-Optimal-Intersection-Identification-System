#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

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


vector<int> BFS(int vNum, int start, int end, vector<vector<int>> adList){
    queue<int> Q;
    Q.push(start);
    int pi[vNum+1];
    vector<int> visited;
    vector<int> checkQ;
    checkQ.push_back(start);
    for (int i = 0; i <= vNum;i++)
        pi[i] = 0;
    while (Q.size() != 0)
        {
            for (int i = 0; i < adList[Q.front()].size(); i++)
            {
                if(adList[Q.front()][i] == 0)
                    continue;
                if(find(visited.begin(), visited.end(), adList[Q.front()][i])==visited.end()){
                    if(find(checkQ.begin(),checkQ.end(),adList[Q.front()][i]) == checkQ.end()){
                        Q.push(adList[Q.front()][i]);
                        checkQ.push_back(adList[Q.front()][i]);
                        pi[adList[Q.front()][i]] = Q.front();
                    }
                }
            }
            visited.push_back(Q.front());
            Q.pop();
        }
        int index = end;
        vector<int> path;
        path.push_back(end);
        while(pi[index] != start){
            path.push_back(pi[index]);
            index = pi[index];
            if(index == 0){
                break;
            }
        }
        if(index != 0){
            path.push_back(start);
            for (int i = 0; i < path.size()/2;i++){
                int temp = path[i];
                path[i] = path[path.size() - i - 1];
                path[path.size() - i - 1] = temp;
            }
        }else{
            path.clear();
            path.push_back(0);
        }
        return path;

}

int main() {

    string command = "";
    int vertexNum = 0;
    int startGoal = 0;
    int endGoal = 0;
    vector<vector<int>> adList;

    while (!cin.eof()) {
        getline(cin, command);
        if (command.size() > 0){
            if(command[0] =='V' || command[0] =='E')
                cout << command << endl;
            string cmdStr = string(command);
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
                adList.clear();
                vector<int> entry;
                entry.push_back(0);
                for (int i = 0; i <= vertexNum; i++)
                    adList.push_back(entry);
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
                    if(adList[start][0] == 0)
                        adList[start].pop_back();
                    adList[start].push_back(end);
                    if(adList[end][0] == 0)
                        adList[end].pop_back();
                    adList[end].push_back(start);
                    wholeEdge = wholeEdge.substr(rbra + 1, wholeEdge.length() - rbra - 1);
            }
        }

            else if(cmdStr[0] == 's'){
                int spacePos = command.find(" ");
                if(spacePos == -1){
                    cout << "Error:invalid command!" << endl;
                    continue;
                }
                string vPair = command.substr(spacePos+1, command.length()-spacePos-1);
                spacePos = vPair.find(" ");
                if(spacePos == -1){
                    cout << "Error:invalid vertexes!" << endl;
                    continue;
                }
                string first = vPair.substr(0, spacePos);
                string last = vPair.substr(spacePos + 1, vPair.length() - spacePos - 1);
                if(judgeValid(first)&&judgeValid(last)){
                    startGoal = stringToInt(first);
                    endGoal = stringToInt(last);
                }
                else{
                    cout << "Error:invalid vertexes!" << endl;
                    continue;
                }
                if((startGoal < 1)||(endGoal < 1)||(startGoal > vertexNum)||(endGoal > vertexNum)){
                    cout << "Error:input vertex doesn't exist!"  << endl;
                    continue;
                }
                vector<int> path = BFS(vertexNum,startGoal, endGoal, adList);
                if((path.size() == 1)&&(path[0] == 0)){
                    cout << "Error:no path exists!" << endl;
                }else{
                    for (int i = 0; i < path.size();i++){
                        if(i == path.size() - 1)
                            cout << path[i] << endl;
                        else
                            cout << path[i] << "-";
                        }
                }
            }
            else{
                cout << "Error:invalid command!" << endl;
                
            }
        }
    }
}
