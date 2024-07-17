/**
 * @file main.cpp
 * @author Kiss Balázs
 * @brief neurális hálozat
 * @version 0.1
 * @date 2024-05-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <cmath>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "memtrace.h"
#include "matrix.hpp"
#include "layer.h"
#include "network.h"
#include "mnist.h"
#include "image.h"

using namespace std;
void header();
bool extension_check(string& fname, const char *ext);
bool file_exists(const string &fname);

int main(){
#ifdef CPORTA
    auto start = std::chrono::high_resolution_clock::now();
    Network TestNN;
    TestNN.load("best.dat");
    int length = 1000;
    Matrix<double>* x = new  Matrix<double>[length];
    Matrix<double>* y = new Matrix<double>[length];
    for(int i = 0; i<length;i++){
        x[i] = Matrix<double>(28*28,1);
        y[i] = Matrix<double>(10,1);

    }
    //load mnist
    MNIST_images(x, length);
    MNIST_labels(y, length);

    int acc = 0;
    for(int i = 0; i<length; i++){
    int row, col;
    double max = 0.0;
        Matrix<double> result = TestNN.forward(x[i]);
        for(int r = 0; r<result.getRow(); r++){
            for(int c = 0; c<result.getCol(); c++){
                if(result(r,c) > max){
                    max = result(r,c);
                    row = r;
                    col = c;
                }
            }
        }
        if(y[i](row, col) == 1)
            acc++;    
    }
    double percentage = ((double) acc/length)*100;
    cout<<percentage<<"%";
    auto end = std::chrono::high_resolution_clock::now();    
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nexecution time: " << elapsed.count() << ". s\n";
    delete x;
    delete y;
#else
    //menu
    int menu1 = 0;
    bool error = true;
    while(error){
        system("cls");
        header();
        cout<<"1. tanitas\n";
        cout<<"2. kep beolvasasa\n";
        cin>>menu1;
        if(menu1 == 1){
            error = false;
            system("cls");
            header();
            bool err = true;
            int layers = 0;
            int *inputs;
            while(err){
                cout<<"retegek szama: ";
                cin>>layers;
                inputs = new int[layers+2];
                inputs[0] = 28*28;
                inputs[layers+1] = 10;
                cout<<"bemenetek szama: ";
                for(int i = 1; i<=layers; i++){
                    int input;
                    cin>>input;
                    inputs[i] = input;
                }
                cout<<"[ ";
                for(int i = 0; i<layers+2; i++){
                    cout<<inputs[i]<<" ";
                }
                cout<<"]\n";
                cout<<"minta merete: ";
                int length; 
                cin>>length;
                cout<<"epoch: ";
                int epoch;
                cin>>epoch;
                cout<<"tanulasi rata: ";
                double lr;
                cin>>lr;
                auto start = chrono::high_resolution_clock::now();
                Network NN(inputs, layers+2);
                Matrix<double> x[length];
                Matrix<double> y[length];
                for(int i = 0; i<length;i++){
                    x[i] = Matrix<double>(28*28,1);
                    y[i] = Matrix<double>(10,1);

                }
                //load mnist
                MNIST_images(x, length);
                MNIST_labels(y, length);

                cout<<endl;
                for(int i = 0; i<epoch; i++){
                    cout<<i+1<<". Epoch ";
                    NN.train(x, y, length, lr);
                }
                auto end = chrono::high_resolution_clock::now();
    
                chrono::duration<double> elapsed = end - start;
                cout << "\nexecution time: " << elapsed.count() << ". s\n";

                cout<<"milyen neven szeretne menteni az adott halozatot: ";
                string fname;
                cin>>fname;
                bool err2 = true;
                while(err2){
                    if(extension_check(fname, ".dat")){
                        NN.save(fname.c_str());
                        err2 = false;
                        err = false;
                    }
                    else{
                        cout<<"hibas fajl név vagy kiterjesztés,\nkerem adja meg ujra: ";
                        cin>>fname;
                    }
                }
                err = false;
            }

        }
        else if(menu1==2){
            error = false;
            Network NN;
            Matrix<double> img;
            bool err = true;
            system("cls");
            header();
            cout<<"network betoltese\nkerem adja meg a file nevet: ";
            string fname;
            cin>>fname;
            while(err){
                if(extension_check(fname, ".dat")){
                    if(file_exists(fname.c_str())){
                        NN.load(fname.c_str());
                        err = false;
                    }
                    else{  
                        system("cls");
                        header();
                        cout<<"hiba a fajl megnyitasakor,\n kerem adja meg ujra a nevet: ";
                        cin>>fname;
                    }
                } 
                else{
                    system("cls");
                    header();
                    cout<<"hibas fajlkiterjesztes,\n adja meg ujra a nevet: ";
                    cin>>fname;
                }
            }
            err = true;
            //kép betoltése
            bool folytat = true;
            while(folytat){
                system("cls");
                header();
                cout<<"kerem adja meg a kep nevet: ";
                cin>>fname;
                while(err){
                    if(file_exists(fname.c_str())){
                            err = false;
                            img = load_image(fname.c_str());
                    }
                    else{  
                        system("cls");
                        header();
                        cout<<"hiba a kep megnyitasakor,\n kerem adja meg ujra a nevet: ";
                        cin>>fname;
                    }
                }
                int idx = 0;
                for(int i = 0; i<28; i++){
                    for(int j = 0; j <28; j++){
                        double pix = img(idx++,0)*255;
                        int intense = (int)pix % 4;
                        switch (intense)
                        {
                        case 0:
                            cout << " ";
                            break;
                        case 1:
                            cout << ":";
                            break;
                        case 2:
                            cout << "x";
                            break;
                        case 3:
                            cout << "@";
                            break;
                        }
                    }
                    cout<<"\n";
                }
                cout<<"\n";
                Matrix<double> res = NN.forward(img);
                double max = 0;
                int max_id = 0;
                for(int i = 0; i<10; i++){
                    if(res(i,0)>max){
                        max = res(i,0);
                        max_id = i;
                    }
                }
                cout<<"a kepen lathato szam: "<<max_id;
                cout<<"\nszeretne tovabb folytatni (y/n)";
                char cont;
                cin>>cont;

                if(cont == 'y' or cont == 'Y'){
                    folytat = true;
                    err = true;
                }else if(cont == 'N' or cont == 'N'){
                    folytat = false;
                    return 0;
                }else
                    return 0;
            } 
        }
        else{
            system("cls");
            cout<<"nincs ilyen opcio";
            cerr<<"press any key to continue...";
            string c;
            cin>>c;
        } 
    }
#endif
    return 0;
}
/**
 * @brief ki rajzolja a program logóját
 * 
 */
void header(){
    cout<<  "\n/****     **                                   **\n";               
    cout<<  "/**/**   /**                                  /**\n";             
    cout<<  "/**//**  /**  *****  **   ** ******  ******   /**\n";
    cout<<  "/** //** /** **///**/**  /**//**//* //////**  /**\n";        
    cout<<  "/**  //**/**/*******/**  /** /** /   *******  /**\n";        
    cout<<  "/**   //****/**//// /**  /** /**    **////**  /**\n";        
    cout<<  "/**    //***//******//******/***   //******** ***\n";        
    cout<<  "//      ///  //////  ////// ///     //////// ///\n\n";                
    cout<<  "/****     **           **                              **\n";        
    cout<<  "/**/**   /**          /**                             /**\n";        
    cout<<  "/**//**  /**  *****  ****** ***     **  ******  ******/**  **\n";        
    cout<<  "/** //** /** **///**///**/ //**  * /** **////**//**//*/** **\n";        
    cout<<  "/**  //**/**/*******  /**   /** ***/**/**   /** /** / /****\n";        
    cout<<  "/**   //****/**////   /**   /****/****/**   /** /**   /**/**\n";
    cout<<  "/**    //***//******  //**  ***/ ///**//****** /***   /**//**\n";        
    cout<<  "//      ///  //////    //  ///    ///  //////  ///    //  //\n\n\n";
}
/**
 * @brief le ellenőrzi a fálj kiterjesztését ha nincs akkor kiegészití a megadott paraméterrel
 * 
 * @param fname fálj neve
 * @param ext kiterjesztés
 * @return true ha a kiterjesztés egyezik vagy ha nem adtak meg a fálj kiterjesztését
 * @return false ha A kiterjesztés nem egyezik
 */
bool extension_check(string &fname, const char * ext){
    string tmp;
    bool cp = false;
    for(auto a : fname){
        if(a == '.')
            cp = true;
        if(cp)
            tmp+=a;
    }
    if(cp)
        return strcmp(tmp.c_str(), ext) == 0;
    else
        fname+= ext;
    return true;
}
/**
 * @brief mégnézi hogy létezik-e az adott fájl
 * 
 * @param fname fálj név
 * @return true ha létezik a fálj
 * @return false ha nem létezik a fálj
 */
bool file_exists(const string &fname) {
    ifstream infile(fname);
    return infile.good();
}
