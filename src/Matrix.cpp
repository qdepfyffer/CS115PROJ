#include "Matrix.h"

#include <iostream>
#include <tuple>
#include <cfloat>

// Clears allocated memory.
void Matrix::clean() {
    if (matrix) {
        // Iterate through the array of pointers to doubles, free memory allocated to each array of doubles.
        for (int i = 0; i < n; i++) {
            delete[] matrix[i];
        }
        // Free memory allocated to array of pointers to doubles.
        delete[] matrix;
    }
}

// Constructor with given size.
Matrix::Matrix(int n, int m) : n(n), m(m) {
    // Initialize the array of pointers to doubles that will later point to arrays of doubles.
    matrix = new double*[n];
    // Iterate through the array of pointers to doubles, and initialize each as a new array of doubles.
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[m];
        // Iterate through the new arrays of doubles and initialize each as 0.
        for (int j = 0; j < m; j++) {
            matrix[i][j] = 0;
        }
    }
}

// Copy constructor.
Matrix::Matrix(const Matrix& m1) : n(m1.n), m(m1.m) {
    // Initialize the array of pointers to doubles that will later point to arrays of doubles.
    matrix = new double*[n];
    // Iterate through the array of pointers to doubles, and initialize each as a new array of doubles.
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[m];
        // Iterate through the new arrays of doubles and initialize each as 0.
        for (int j = 0; j < m; j++) {
            matrix[i][j] = m1.matrix[i][j];
        }
    }
}

// Default constructor (defaults to a 5 x 5 matrix).
Matrix::Matrix(): Matrix(5, 5) {}

// Destructor.
Matrix::~Matrix() {
    clean();
}

// Initializes all values in the matrix to some given initial value.
void Matrix::init(double initialValue) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = initialValue;
        }
    }
}

// Initializes the object as the identity matrix of the object instance this function is called on.
void Matrix::identity() {
    // If the array is square (which it must be for this function to work), initialize all values to 0, then set the values along the diagonal to 1.
    if (n == m) {
        init(0);
        for (int i = 0; i < n; i++) {
            matrix[i][i] = 1;
        }
    }
}

// Returns the number of rows in the matrix.
int Matrix::getRows() const {
    return n;
}

// Returns the number of columns in the matrix.
int Matrix::getCols() const {
    return m;
}

// Returns the value in the given cell of the matrix.
double Matrix::getCell(int x, int y) {
    // Check the bounds. If there is an issue, inform the user and return a flagging value.
    if (x < 0 || x >= n || y < 0 || y >= m) {
        std::cerr << "Error with index\n";
        return DBL_MIN;
    }
    // If everything was okay, return the specified cell.
    return matrix[x][y];
}

// Returns the minimum value in the matrix, along with its position. Then, it sets the value at that position to 0.
std::tuple<double, int, int> Matrix::getMin() {
    // Initialize some temporary variables to store the minimum value in the matrix and its location.
    double min = DBL_MAX;
    int minX = 0;
    int minY = 0;
    // Iterate through the matrix and find the minimum value, store it and its position.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] < min && matrix[i][j] > 0) {
                min = matrix[i][j];
                minX = i;
                minY = j;
            }
        }
    }
    // Set the location of the minimum value to 0.
    matrix[minX][minY] = 0;
    // Return a tuple containing all this information.
    return std::make_tuple(min, minX, minY);

}

// Sets the value in the given cell to the given value.
void Matrix::setCell(int x, int y, double value) {
    // Check the bounds. If there is an issue, inform the user and return a flagging value.
    if (x < 0 || x >= n || y < 0 || y >= m) {
        std::cerr << "Error with index\n";
    }
    // If everything was okay, return the specified cell.
    matrix[x][y] = value;
}

// Adds a specified matrix to the object instance this function is called on.
Matrix Matrix::add(const Matrix& m1) {
    // Create a new matrix for output
    Matrix output(n, m);
    // Iterate through the matrices and add values, inserting them into the output matrix. Then, return the output matrix.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output(i, j) = matrix[i][j] + m1.matrix[i][j];
        }
    }
    return output;
}

// Subtracts a specified matrix from the object instance this function is called on.
Matrix Matrix::sub(const Matrix& m1) {
    // Create a new matrix for output
    Matrix output(n, m);
    // Iterate through the matrices and subtract values, inserting them into the output matrix. Then, return the output matrix.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output(i, j) = matrix[i][j] - m1.matrix[i][j];
        }
    }
    return output;
}

// Get value.
double Matrix::operator()(int x, int y) const {
    return matrix[x][y];
}

// Set value.
double& Matrix::operator()(int x, int y) {
    return matrix[x][y];
}

// Assignment operator.
Matrix& Matrix::operator=(const Matrix& m1) {
    // Handle self-assignment.
    if (this == &m1) {
        return *this;
    }
    clean();
    n = m1.n;
    m = m1.m;
    // Initialize the array of pointers to doubles that will later point to arrays of doubles.
    matrix = new double*[n];
    // Iterate through the array of pointers to doubles, and initialize each as a new array of doubles.
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[m];
        // Iterate through the new arrays of doubles and initialize each as 0.
        for (int j = 0; j < m; j++) {
            matrix[i][j] = m1.matrix[i][j];
        }
    }
    return *this;
}

// Adds two matrices.
Matrix Matrix::operator+(const Matrix& m1) {
    // Create a new matrix for output
    Matrix output(n, m);
    // Iterate through the matrices and add values, inserting them into the output matrix. Then, return the output matrix.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output(i, j) = matrix[i][j] + m1.matrix[i][j];
        }
    }
    return output;
}

// Adds a scalar and a matrix.
Matrix Matrix::operator+(double value) {
    // Create a new matrix for output
    Matrix output(n, m);
    // Iterate through the matrices and add values, inserting them into the output matrix. Then, return the output matrix.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output(i, j) = matrix[i][j] + value;
        }
    }
    return output;
}

// Subtracts two matrices.
Matrix Matrix::operator-(const Matrix& m1) {
    // Create a new matrix for output
    Matrix output(n, m);
    // Iterate through the matrices and subtract values, inserting them into the output matrix. Then, return the output matrix.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output(i, j) = matrix[i][j] - m1.matrix[i][j];
        }
    }
    return output;
}

// Subtracts a scalar from a matrix
Matrix Matrix::operator-(double value) {
    // Create a new matrix for output
    Matrix output(n, m);
    // Iterate through the matrices and subtract values, inserting them into the output matrix. Then, return the output matrix.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output(i, j) = matrix[i][j] - value;
        }
    }
    return output;
}

// Multiplies two matrices.
Matrix Matrix::operator*(const Matrix& m1) {
    // First, ensure matrices are of compatible sizes.
    if (m == m1.n) {
        // If so, create an output matrix of the correct size.
        Matrix output(n, m1.m);
        // Perform the multiplication.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m1.m; j++) {
                double tmp = 0;
                for (int k = 0; k < m; k++) {
                    tmp += matrix[i][k] * m1.matrix[k][j];
                }
                output.matrix[i][j] = tmp;
            }
        }
        return output;
    }
    // If the matrices are not of compatible sizes, inform the user and return the original matrix.
    std::cout << "Error with dimensions." << std::endl;
    return *this;
}

// Multiplies a matrix with by a scalar.
Matrix Matrix::operator*(double value) {
    // Create an output matrix, then perform scalar multiplication.
    Matrix output(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output.setCell(i, j, matrix[i][j] * value);
        }
    }
    return output;
}

// Displays the matrix in the console, one row per line.
void Matrix::display() const {
    std::cout << "Dimensions: " << n << " x " << m << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}