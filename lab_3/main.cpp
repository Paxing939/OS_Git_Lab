#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "log_duration.h"

void PrintMatrix(const std::vector<std::vector<double>> &matrix) {
    for (const auto &vec : matrix) {
        for (const auto &x_ : vec) {
            std::cout << std::setw(7) << std::setprecision(2) << std::left << x_ << " ";
        }
        std::cout << std::endl;
    }
}

void ReadDataFromFile(std::vector<std::vector<double>> &a, std::vector<std::vector<double>> &b, int &k) {
    std::ifstream reader("input.txt");
    int A, B, C;
    reader >> A >> B >> C >> k;
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

    for (int i = 0; i < A; i++) {
        for (int j = 0; j < C; j++) {
            for (int k = 0; k < B; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return result;
}

void FirstThreadFunction(const std::vector<std::vector<double>> &a, const std::vector<std::vector<double>> &b,
                         std::vector<std::vector<double>> &result, int num_flows, int index, int j_index) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    for (int i = index * A / num_flows; i < (index + 1) * A / num_flows; i++) {
        for (int j = j_index * C / num_flows; j < (j_index + 1) * C / num_flows; j++) {
            for (int k = 0; k < B; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

std::vector<std::vector<double>> FirstParallelProduct(const std::vector<std::vector<double>> &a,
                                                      const std::vector<std::vector<double>> &b, int num_flows) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    std::vector<std::vector<double>> result(A, std::vector<double>(C, 0));

    std::vector<std::thread> threads(num_flows);

    for (int i = 0; i < num_flows; i++) {
        for (int j = 0; j < num_flows; j++) {
            threads[j] = std::thread(FirstThreadFunction, std::ref(a), std::ref(b), std::ref(result), num_flows, j, i);
        }

        for (int j = 0; j < num_flows; j++) {
            threads[j].join();
        }
    }

    return result;
}

void SecondThreadFunction(const std::vector<std::vector<double>> &a, const std::vector<std::vector<double>> &b,
                          std::vector<std::vector<double>> &result, int num_flows, int index) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    for (int i = 0; i < A; i++) {
        for (int j = 0; j < C; j++) {
            for (int k = index * B / num_flows; k < (index + 1) * B / num_flows; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

std::vector<std::vector<double>> SecondParallelProduct(const std::vector<std::vector<double>> &a,
                                                       const std::vector<std::vector<double>> &b, int num_flows) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    std::vector<std::vector<double>> result(A, std::vector<double>(C, 0));

    std::vector<std::thread> threads(num_flows);

    for (int i = 0; i < num_flows; i++) {
        threads[i] = std::thread(SecondThreadFunction, std::ref(a), std::ref(b), std::ref(result), num_flows, i);
    }

    for (int j = 0; j < num_flows; j++) {
        threads[j].join();
    }

    return result;
}

void ThirdThreadFunction(const std::vector<std::vector<double>> &a, const std::vector<std::vector<double>> &b,
                         std::vector<std::vector<double>> &result, int num_flows, int i_index, int j_index, int k_index) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    for (int i = i_index * A / num_flows; i < (i_index + 1) * A / num_flows; i++) {
        for (int j = j_index * C / num_flows; j < (j_index + 1) * C / num_flows; j++) {
            for (int k = k_index * B / num_flows; k < (k_index + 1) * B / num_flows; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

std::vector<std::vector<double>> ThirdParallelProduct(const std::vector<std::vector<double>> &a,
                                                      const std::vector<std::vector<double>> &b, int num_flows) {
    int A = a.size(), B = a[0].size(), C = b[0].size();
    if (B != b.size()) {
        throw std::runtime_error("Incorrect dimensions of matrices!");
    }

    std::vector<std::vector<double>> result(A, std::vector<double>(C, 0));

    std::vector<std::thread> threads(num_flows);

    for (int i = 0; i < num_flows; i++) {
        for (int j = 0; j < num_flows; j++) {
            for (int k = 0; k < num_flows; k++) {
                threads[k] = std::thread(ThirdThreadFunction, std::ref(a), std::ref(b), std::ref(result), num_flows, i, j, k);
            }

            for (int k = 0; k < num_flows; k++) {
                threads[k].join();
            }
        }
    }

    return result;
}

int main() {
    std::vector<std::vector<double>> a, b, c;
    int k;
    ReadDataFromFile(a, b, k);

    c = SequentialProduct(a, b);
    PrintMatrix(c);
    std::cout << "\n\n";

    c = FirstParallelProduct(a, b, k);
    PrintMatrix(c);
    std::cout << "\n";

    c = SecondParallelProduct(a, b, k);
    PrintMatrix(c);
    std::cout << "\n";


    c = ThirdParallelProduct(a, b, k);
    PrintMatrix(c);
    std::cout << "\n";

    return 0;
}