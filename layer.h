#include "matrix.hpp"
#ifndef LAYER_H
#define LAYER_H
class Layer{
    Matrix<double> weights, bias;
public:
    Layer(int inputs=1, int outputs=1);
    //getterek
    Matrix<double> getW()const{return weights;}
    Matrix<double> getB()const{return bias;}

    void setW(Matrix<double> w){weights = w;}
    void setB(Matrix<double> b){bias = b;}

    Matrix<double> forward(const Matrix<double>& input);
    Matrix<double> backprop(const Matrix<double>& error, const Matrix<double>& prev_out,const Matrix<double>& z, double lr);
};
Matrix<double> sigmoid(const Matrix<double>& z);
Matrix<double> sigmoid_prime(const Matrix<double>& z);
#endif