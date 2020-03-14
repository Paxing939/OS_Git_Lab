#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>

void ReadDataFromFile(std::vector<std::vector<double>> &a, std::vector<std::vector<double>> &b) {
    std::ifstream reader("input.txt");
    int A, B, C;
    reader >> A >> B >> C;
    a.resize(A, std::vector<double>(B));
    b.resize(B, std::vector<double>(C));

    for (int i = 0; i < A; i++) {
        for (int j = 0; j < B; j++) {
            reader >> a[i][j];
        }
    }

    for (int i = 0; i < B; i++) {
        for (int j = 0; j < C; j++) {
            reader >> b[i][j];
        }
    }
}

std::vector<std::vector<double>> SequentialProduct(const std::vector<std::vector<double>> &a,
                                                   const std::vector<std::vector<double>> &b) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    std::vector<std::vector<double>> result(A, std::vector<double>(C, 0));

    for (int i = 0; i < A; ++i) {
        for (int j = 0; j < C; ++j) {
            for (int k = 0; k < B; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return result;
}

std::vector<std::vector<double>> FirstParallelProduct(const std::vector<std::vector<double>> &a,
                                                      const std::vector<std::vector<double>> &b) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    std::vector<std::vector<double>> result(A, std::vector<double>(C, 0));



    return result;
}

int main() {

    std::cout << "Hello, World!" << std::endl;
    return 0;
}