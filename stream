#include <string>
#include <iostream>
#include <vector>
#include <ctime>
#include "myself/QuadratMatrix.h"

using namespace std;

int main(int argc, char *argv[]){
    srand(time(0));

    size_t threads = 4;

    if(argc == 2){
        threads = atoi(argv[1]);
    }else if(argc > 2){
        cout << "Wrong args for program! Exit!" << endl;
        return 1;
    }

    --threads;

    if(threads < 1){
        cout << "Sorry, but use more threads! Exit!" << endl;
        return 2;
    }

    myself::QuadratMatrix::create_threads(threads);

    myself::QuadratMatrix left;
    myself::QuadratMatrix right;

    for(size_t i = 0; i < left.get_size(); ++i){
        for(size_t j = 0; j < left.get_size(); ++j){
            left[i][j] = rand();
            right[i][j] = rand();
        }
    }
    
    string ans_path = "temp/example.mtx";

    myself::QuadratMatrix ans = left * right;

    ans.to_file(ans_path);

    return 0;
}
