#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <sstream>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <vector>
#include <signal.h>
using namespace std;

int childRgen(char** args){
    execv("./rgen",args);
    delete [] args;
    return 0;
}

int childPya1(){
    char* argue[3];
    argue[0] = (char*)"python3";
    argue[1] = (char*)"./Identity Intersections.py";
    argue[2] = nullptr;
    execv("/usr/bin/python3",argue);
    return 0;
}

int childCppa2(){
    char* args2[2];
    args2[0] = (char*)"BFS_shortest_path";
    args2[1] = nullptr;
    execv("./BFS_shortest_path",args2);
    return 0;
}

bool stringisNum(string s){
    for(int i = 0 ; i < s.size(); i++){
        if(s[0] <= '0' || s[0] >= '9')
            return false;
    }
    return true;
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

int main(int argc, char** argv) {
    std::vector<pid_t> kids;

    int rgenToa1[2];
    pipe(rgenToa1);
    int a1Toa2[2];
    pipe(a1Toa2);

//除了第一个参数，后面的如果有字母但不是s n l c这四个字母 ，是这四个字母但后面跟的不是数字都报错后退出！
    string sk = "10";
    string nk = "5";
    string lk = "5";
    string ck = "20";
    for (int i = 1; i < argc; i++){ //i从1开始的
    //-s 必须和数字成对出现，不然也会报错！ 
        if(argv[i][0] == '-' && argv[i][1] == 'n'){
            if(argc - 1 - i >= 1){
                if(stringisNum(argv[i + 1])){
                    if(stringToInt(argv[i + 1]) >= 1)
                        nk = argv[i + 1];
                    else{
                        cerr << "Error : invalid arguments!" << endl;
                        exit(1);
                    }
                }
                else{
                    cerr << "Error : invalid arguments!" << endl;
                    exit(1);
                }
            }else{
                cerr << "Error : invalid arguments!" << endl;
                exit(1);
            }
        }
        else if(argv[i][0] == '-' && argv[i][1] == 'l'){
            if(argc - 1 - i >= 1){
                if(stringisNum(argv[i + 1])){
                    if(stringToInt(argv[i + 1]) >= 5)
                        lk = argv[i + 1];
                    else{
                        cerr << "Error : invalid arguments!" << endl;
                        exit(1);
                    }
                }
                else{
                    cerr << "Error : invalid arguments!" << endl;
                    exit(1);
                }
            }else{
                cerr << "Error : invalid arguments!" << endl;
                exit(1);
            }
        }    
        else if(argv[i][0] == '-' && argv[i][1] == 'c'){
            if(argc - 1 - i >= 1){
                if(stringisNum(argv[i + 1])){
                    if(stringToInt(argv[i + 1]) >= 1)
                        ck = argv[i + 1];
                    else{
                        cerr << "Error : invalid arguments!" << endl;
                        exit(1);
                    }
                }
                else{
                    cerr << "Error : invalid arguments!" << endl;
                    exit(1);
                }
            }else{
                cerr << "Error : invalid arguments!" << endl;
                exit(1);
            }
        }    
        else if(argv[i][0] == '-' && argv[i][1] == 's'){
            if(argc - 1 - i >= 1){
                if(stringisNum(argv[i + 1])){
                    if(stringToInt(argv[i + 1]) >= 2)
                        sk = argv[i + 1];
                    else{
                        cerr << "Error : invalid arguments!" << endl;
                        exit(1);
                    }
                }
                else{
                    cerr << "Error : invalid arguments!" << endl;
                    exit(1);
                }
            }else{
                cerr << "Error : invalid arguments!" << endl;
                exit(1);
            }
        }
        else if((argv[i][0] != '-' && !stringisNum(argv[i]))
        ||(argv[i][0] == '-' && argv[i][1] != 's' && argv[i][1] != 'n' && argv[i][1] != 'l' && argv[i][1] != 'c')
        ||(stringisNum(argv[i]) && argv[i - 1][0] != '-')
        ||(stringisNum(argv[i]) && argv[i - 1][0] == '-' && argv[i - 1][1] != 's' && argv[i - 1][1] != 'n' && argv[i - 1][1] != 'l' && argv[i - 1][1] != 'c')){
            cerr << "Error : invalid arguments!" << endl;
            exit(1);
        }
    }
    char** args = new char*[6];
    args[0] = (char*)"./rgen";
    args[1] = (char*)&sk[0];
    args[2] = (char*)&nk[0];
    args[3] = (char*)&lk[0];
    args[4] = (char*)&ck[0];
    args[5] = nullptr;

    pid_t child_pid;
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdout to the pipe
        dup2(rgenToa1[1], STDOUT_FILENO);
        close(rgenToa1[0]);
        close(rgenToa1[1]); 
        return childRgen(args);
    } else if (child_pid < 0) {
        cerr << "Error: could not fork\n";
        return 1;
    }

    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0) { //a1
        dup2(rgenToa1[0], STDIN_FILENO); //rgentoa1
        close(rgenToa1[1]);
        close(rgenToa1[0]);
        dup2(a1Toa2[1], STDOUT_FILENO); //a1toa2
        close(a1Toa2[0]);
        close(a1Toa2[1]); 
        return childPya1();
    } else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    kids.push_back(child_pid);

    child_pid = fork();
    if (child_pid == 0) { //a2
        dup2(a1Toa2[0], STDIN_FILENO);
        close(a1Toa2[0]);
        close(a1Toa2[1]);
        return childCppa2();
    }else if (child_pid < 0){
        std::cerr << "Error starting a process, fork failed!" << std::endl;
        return 1;
    }

    dup2(a1Toa2[1], STDOUT_FILENO); //a1toa2
    close(a1Toa2[0]);
    close(a1Toa2[1]);
    while (!cin.eof()) {
        string line;
        getline(cin, line);
        cout << line << endl;
    }


    //   send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return 0;
}
