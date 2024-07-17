
#include <iostream>
#include <fstream>
#include "mnist.h"
using namespace std;
int ReverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}
/**
 * @brief a megadott fájlból beolvassa az tanítási adatokat egy mátrixokból álló tömbe
 * 
 * @param x mátrix tömb
 * @param len a mátrix tömbnek a hossza
 */
void MNIST_images(Matrix<double> x[], int len)
{
    ifstream file ("t10k-images.idx3-ubyte",ios::binary);
    if (file.is_open())
    {
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        file.read((char*)&magic_number,sizeof(magic_number));
        magic_number= ReverseInt(magic_number);
        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= ReverseInt(number_of_images);
        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= ReverseInt(n_rows);
        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= ReverseInt(n_cols);
        for(int i=0;i<len;++i)
        {
            int idx = 0;
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    x[i](idx++,0)= (double)temp/255;
                }
            }
        }
    }
    else
       throw std::invalid_argument("cant open the file");
}
/**
 * @brief a megadott fájlból beolvassa a kivánt kimeneti értékeket egy mátrixokból álló tömbe
 * 
 * @param y mátrix tömb
 * @param len a mátrix tömbnek a hossza
 */
void MNIST_labels(Matrix<double> y[], int len){
     ifstream file ("t10k-labels.idx1-ubyte",ios::binary);
     if(file.is_open()){
        int magic_number=0;
        int number_of_labels = 0;
        file.read((char*)&number_of_labels,sizeof(number_of_labels));
        magic_number= ReverseInt(magic_number);
        file.read((char*)&number_of_labels,sizeof(number_of_labels));
        number_of_labels= ReverseInt(number_of_labels);
        int idx = 0;
        for(int i = 0; i<len; i++){
            y[i].zeros();
            
            unsigned char temp=0;
            file.read((char*)&temp,sizeof(temp));
            idx = (int)temp;
            y[i](idx,0) = 1;
        }
        
     }
     else
      throw std::invalid_argument("cant open the file");
}
