
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "image.h"
#include "matrix.hpp"
/** 
 * @brief beolvassa a felhasznéló által megadott képet amit egy mátrixban tárol el
 * 
 * @param fname file név
 * @return kép mátrixa
*/
Matrix<double> load_image(const char *fname){
    Matrix<double> res(28*28,1);
    unsigned char* data; 
    int width, height, channels;
    if((data = stbi_load(fname, &width, &height, &channels, 0)) != NULL) {
        if(width != 28 && height != 28){
            throw std::invalid_argument("the image size doesnt mach");
        }
        if(channels >1){
            int size = width*height*channels;
            int idx = 0;
            for(unsigned char *i =data; i != data+size; i+=channels ){
                double temp = (*i +*(i+1)+*(i+2))/3.0;
                res(idx++,0) = temp/255;
            }
        }else{
            int size = height*width;
            for(int i = 0; i < size; i++){
                double temp = (double) data[i];
                res(i, 0) = temp/255;
            }
        }
    }
    else{
         throw std::invalid_argument("cant open the file");
    }
    return res;
}