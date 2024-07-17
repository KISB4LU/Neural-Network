#include <iostream>
#include <cmath>
#include "layer.h"


Layer::Layer(int inputs, int outputs):weights(outputs, inputs),bias(outputs,1){
    weights.mrand();
    bias.mrand();
}
/**
 * @brief ez a fügvény kiszámolja az adott réteg kimeneti értékeit
 * 
 * @param input az adat vagy az előző réteg neuron rétékei 
 * @return a réteg kimeneti neuronjai mátrixban eltárolva 
 */
Matrix<double> Layer::forward(const Matrix<double>& input){
    Matrix<double> ret = dot(weights, input) + bias;
    return ret;
}/**
 * @brief ezzel a fügvénnyel lehet tanítani a hálozatott
 * 
 * @param error az l+1. réteg hibája (deriváltja)
 * @param prev_out az l-1. réteg kimenete (aktivácios fügvény után)
 * @param z az l-1 réteg kimenete aktivációs fügvény meghivása elött
 * @param lr tanulási ráta
 * @return l-1. réteg hibája
 */
Matrix<double> Layer::backprop(const Matrix<double>& error, const Matrix<double>& prev_out,const Matrix<double>& z, double lr){
    Matrix<double> err=dot(weights.Transpose(), error)*sigmoid_prime(z);
    bias = bias - lr*error;
    weights = weights -lr*dot(error, prev_out.Transpose());

    return err;
}/**
 * @brief a sigmoid fügvény egy nem lineáris leképzést használ a fügvény értéke 0 és 1 közé esik
 * 
 * @param z a réteg kimeneti értéke 
 * @return Matrix<double> 
 */
Matrix<double> sigmoid(const Matrix<double>& z){
    Matrix<double> ret(z.getRow(), z.getCol());
    for(int i = 0; i<z.getRow(); i++){
        for(int j = 0; j<z.getCol(); j++){
            ret(i,j) = 1.0/(1.0+exp(-z(i,j)));
        }
    }
    return ret;
}
/**
 * @brief sigmoid fügvény deriváltja ami a tanulási algoritmushoz nélkülözhetetlen
 * 
 * @param z 
 * @return Matrix<double> 
 */
Matrix<double> sigmoid_prime(const Matrix<double>& z){
    Matrix<double> ret = sigmoid(z)*( 1.0-sigmoid(z));
    return ret;
}
