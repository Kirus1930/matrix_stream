#ifndef QUADRAT_MATRIX_H
#define QUADRAT_MATRIX_H

#include <vector>
#include <memory>
#include <fstream>
#include <thread>
#include "ThreadController.h"

namespace myself{

#define MATRIX_SIZE 100

class QuadratMatrix{
public:
    QuadratMatrix();
    QuadratMatrix(const QuadratMatrix& matrix);
    QuadratMatrix(QuadratMatrix&& matrix);
    QuadratMatrix(std::string& path);

    static void create_threads(size_t n_treads);

    friend const QuadratMatrix 
    operator*(const QuadratMatrix& left, const QuadratMatrix& right);

    std::vector<double>& operator[](const size_t index);
    const std::vector<double>& operator[](const size_t index) const;

    friend bool 
    operator==(const QuadratMatrix& right, const QuadratMatrix& left);

    friend std::ostream& 
    operator<<(std::ostream& os, const QuadratMatrix& matrix);
    friend std::istream& 
    operator>>(std::istream& is, QuadratMatrix& matrix);

    size_t get_size();
    size_t get_workers();

    void to_file(std::string& path);
    void from_file(std::string& path);

    ~QuadratMatrix(){};

private:
    // controller
    static std::shared_ptr<ThreadController> daemons;
    size_t size;
    std::vector<std::vector<double>> data;

    friend void 
    compute_i_line(const std::vector<double>& l, const QuadratMatrix& r, std::vector<double>& res);
};

std::shared_ptr<ThreadController> QuadratMatrix::daemons(nullptr);

QuadratMatrix::QuadratMatrix(){
    size = MATRIX_SIZE;
    data.assign(size, std::vector<double>(size, 0.0));
}

QuadratMatrix::QuadratMatrix(const QuadratMatrix& matrix){
    size = matrix.size;
    data = matrix.data;
}

QuadratMatrix::QuadratMatrix(QuadratMatrix&& matrix){
    size = matrix.size;
    data = std::move(matrix.data);
    matrix.data.assign(size, std::vector<double>(size, 0.0));
}

QuadratMatrix::QuadratMatrix(std::string& path)
: QuadratMatrix(){
    from_file(path);
}

void QuadratMatrix::from_file(std::string& path){
    std::ifstream file(path, std::ios::in);
    file >> *this;
    file.close();
}

void QuadratMatrix::to_file(std::string& path){
    std::ofstream file(path, std::ios::out);
    file << *this;
    file.close();
}

std::ostream& operator<<(std::ostream& os, const QuadratMatrix& matrix){
    for(size_t i = 0; i < matrix.size; ++i){
        for(size_t j = 0; j < matrix.size; ++j){
            os << matrix.data[i][j] << '\t';
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, QuadratMatrix& matrix){
    for(size_t i = 0; i < matrix.size; ++i){
        for(size_t j = 0; j < matrix.size; ++j){
            is >> matrix.data[i][j];
        }
    }
    return is;
}

size_t QuadratMatrix::get_size(){
    return size;
}

size_t QuadratMatrix::get_workers(){
    return daemons->workers();
}

std::vector<double>& QuadratMatrix::operator[](const size_t index){
    return data[index];
}

bool operator==(const QuadratMatrix& r, const QuadratMatrix& l){
    for(size_t i = 0; i < r.size; ++i){
        for(size_t j = 0; j < r.size; ++j){
            if(r[i][j] != l[i][j]){
                return false;
            }
        }
    }
    return true;
}

const std::vector<double>& QuadratMatrix::operator[](const size_t index) const{
    return data[index];
}

void QuadratMatrix::create_threads(size_t n_threads){
    daemons = std::make_shared<ThreadController>(n_threads);
}

void compute_i_line(const std::vector<double>& l, const QuadratMatrix& r, std::vector<double>& res){
    for(size_t j = 0; j < r.size; ++j){
        double ans = 0.0;
        for(size_t k = 0; k < r.size; ++k){
            ans += l[k] * r[k][j];
        }
        res[j] = ans;
    }
}

const QuadratMatrix operator*(const QuadratMatrix& l, const QuadratMatrix& r){
    QuadratMatrix ans;
    std::vector<std::future<void>> futures(ans.size);
    for(size_t i = 0; i < ans.size; ++i){
        futures[i] = QuadratMatrix::daemons->to_work(compute_i_line, std::ref(l[i]), std::ref(r), std::ref(ans[i]));
    }

    for(size_t i = 0; i < ans.size; ++i){
        futures[i].get();
    }
    return ans;
}

}
#endif
