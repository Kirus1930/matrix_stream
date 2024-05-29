#include <string>
#include <iostream>
#include <vector>
#include "myself/QuadratMatrix.h"

using namespace std;

#define NUM_OF_THREADS 3

int main(){
    myself::QuadratMatrix::create_threads(NUM_OF_THREADS);

    string left_path = "tests/leftN.mtx";
    string right_path = "tests/rightN.mtx";
    string ans_path = "temp/answerN.mtx";
    string res_path = "tests/resultN.mtx";

    bool final = true;

    cout << "Testing:" << endl;
    for(char c = '1'; c <= '9'; ++c){
        left_path[10] = c;
        right_path[11] = c;
        ans_path[11] = c;
        res_path[12] = c;
        myself::QuadratMatrix left(left_path);
        myself::QuadratMatrix right(right_path);
        myself::QuadratMatrix res(res_path);
        myself::QuadratMatrix ans = left * right;
        bool compare = (res == ans);
        cout << "Test №" << c << ": " << (compare ? "TRUE" : "FALSE") << endl; 

        final &= compare;

        ans.to_file(ans_path);
    }

    cout << "All tests correct: " << (final ? "TRUE" : "FALSE") << endl;

    return 0;
}
