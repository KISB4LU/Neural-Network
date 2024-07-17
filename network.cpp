#include <iostream>
#include <fstream>
#include "matrix.hpp"
#include "layer.h"
#include "network.h"




Network::Network(int net[], int len):len(len-1){
    layers = new Layer[this->len];
    for(int i = 0; i<this->len; i++){
        layers[i] = Layer(net[i], net[i+1]);
    }
}
Network::Network(){
    layers = NULL;
    len = 0;
}

Network::~Network(){
    delete[] layers;
}
/**
 * @brief ez a fügvény szerepel az egész hálozat tanításáért
 * 
 * @param Dataset tanulási adat 
 * @param y kivánt kimenet
 * @param epoch tanulás hossza
 * @param lr tanulási ráta
 */
void Network::train(Matrix<double> Dataset[],Matrix<double> y[], int epoch, double lr){
    // forward metódus
    Matrix<double> *z =new Matrix<double>[len];
    Matrix<double> *a =new Matrix<double>[len];
    double Cost = 0.0;
    double accuarcy = 0;
    for(int i = 0; i<epoch; i++){
        Matrix<double> input = Dataset[i];
        for(int j = 0; j<len; j++){
            z[j] = layers[j].forward(input);
            a[j] = sigmoid(z[j]);
            input = a[j];
        }
        // a költség kiszámítása (menyire tér el a neurális hálózat eredménye a valóságtól)
        Matrix<double> cost = (a[len-1]-y[i])*(a[len-1]-y[i]);
        Cost += sum(cost);
        Matrix<double> error = 2.0*(a[len-1]-y[i])*sigmoid_prime(z[len-1]);
        // visszaterjesztéses tanulás
        for(int j = len-1; j>0; j--){
            error = layers[j].backprop(error,a[j-1],z[j-1], lr);
        }
        error = layers[0].backprop(error, Dataset[i], Dataset[i], lr);
        
        //megvizsgálja hogy megegyezik-e a neurális hálozat eredménye a valos adattal 
        int row, col;
        double max = 0.0;
        for(int r = 0; r<a[len-1].getRow(); r++){
            for(int c = 0; c<a[len-1].getCol(); c++){
                if(a[len-1](r,c) > max){
                    max = a[len-1](r,c);
                    row = r;
                    col = c;
                }
            }
        }
        if(y[i](row, col) == 1)
            accuarcy++;    
    }
    accuarcy = ((double)accuarcy/epoch)*100;
    std::cout<<"accuarcy: "<<accuarcy<<"% cost: "<<Cost/epoch<<std::endl;
    delete a;
    delete z;
}
/**
 * @brief kiszámtija a neurális hálozat eredményét
 * 
 * @param x kiindulási adat
 * @return Matrix<double> 
 */
Matrix<double> Network::forward(const Matrix<double>& x){
    Matrix<double> z[len];
    Matrix<double> a[len];
    Matrix<double> input = x;
    for(int j = 0; j<len; j++){
        z[j] = layers[j].forward(input);
        a[j] = sigmoid(z[j]);
        input = a[j];
    }
    return input;
}
/**
 * @brief menti az adott neurális hálózatot
 * 
 * @param name fálj név 
 */
void Network::save(const char* name)const{
    std::ofstream file(name, std::ios::out | std::ios::binary);
    if (!file) {
        throw std::invalid_argument("cant open the file");
    }
    file.write((char*)&len,sizeof(len));
    for(int i = 0;i<len; i++){
        Matrix<double> curW = layers[i].getW();
        Matrix<double> curB = layers[i].getB();
        
        int row = curW.getRow();
        int col = curW.getCol();
        
        file.write((char*)&row, sizeof(row));
        file.write((char*)&col, sizeof(col));
        for(int r = 0; r<row; r++){
            for(int c = 0; c<col; c++){
                double temp = curW(r,c); 
                file.write((char*)&temp, sizeof(temp));
            }
        }

        row = curB.getRow();
        col = curB.getCol();
        file.write((char*)&row, sizeof(row));
        file.write((char*)&col, sizeof(col));
        for(int r = 0; r<row; r++){
            for(int c = 0; c<col; c++){
                double temp = curB(r,c); 
                file.write((char*)&temp, sizeof(temp));
            }
        }

    }
}
/**
 * @brief külső fájlból tölti be a neurálsi hálózat adatait
 * 
 * @param name fájl név
 */
void Network::load(const char* name){
     std::ifstream file(name, std::ios::binary);
    if (!file) {
        throw std::invalid_argument("cant open the file");
    }
    file.read((char*)&len,sizeof(len));
    if(layers){
        delete[] layers;
    }
    layers = new Layer[len];
    for(int i = 0; i<len; i++){
        int row, col;
        
        file.read((char*)&row,sizeof(row));
        file.read((char*)&col,sizeof(col));
        Matrix<double> currW(row, col);
        for(int r = 0; r<row; r++){
            for(int c = 0; c<col; c++){
                double temp;
                file.read((char*)&temp, sizeof(temp));
                currW(r,c) = temp;
            }
        }

        file.read((char*)&row,sizeof(row));
        file.read((char*)&col,sizeof(col));
        Matrix<double> currB(row, col);
        for(int r = 0; r<row; r++){
            for(int c = 0; c<col; c++){
                double temp;
                file.read((char*)&temp, sizeof(temp));
                currB(r,c) = temp;
            }
        }
        Layer currL;
        currL.setW(currW);
        currL.setB(currB);
        layers[i] = currL;
    }
}