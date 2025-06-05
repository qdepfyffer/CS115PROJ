#ifndef MATRIX_H
#define MATRIX_H

#include <tuple>

class Matrix {
public:

    Matrix(int n, int m);
    Matrix(const Matrix& m1);
    Matrix();
    ~Matrix();

    void init(double initialValue);
    void identity();

    [[nodiscard]] int getRows() const;
    [[nodiscard]] int getCols() const;
    [[nodiscard]] double getCell(int x, int y);
    std::tuple<double, int, int> getMin();
    void setCell(int x, int y, double value);

    Matrix add(const Matrix& m1);
    Matrix sub(const Matrix& m1);

    double operator()(int x, int y) const;
    double& operator()(int x, int y);
    Matrix& operator=(const Matrix& m1);
    Matrix operator+(const Matrix& m1);
    Matrix operator+(double value);
    Matrix operator-(const Matrix& m1);
    Matrix operator-(double value);
    Matrix operator*(const Matrix& m1);
    Matrix operator*(double value);

    void display() const;

private:
    int n; // Rows.
    int m; // Cols.
    double** matrix; // The actual matrix.

    void clean();

};

#endif //MATRIX_H
