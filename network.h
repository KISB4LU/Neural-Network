#include "matrix.hpp"
#include "layer.h"

class Network{
    Layer *layers;
    int len;
public:
    Network(int net[], int len);
    Network();
    void train(Matrix<double> Dataset[],Matrix<double> y[], int epoch, double lr);
    Matrix<double> forward(const Matrix<double>& input);
    void save(const char* name)const;
    void load(const char* name);
    ~Network();
};
