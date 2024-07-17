#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#ifndef MATRIX_H
#define MATRIX_H
template< typename T>
class Matrix{
    int row, col;
    T **matrix;
public:
    //kostruktorok
    /**
     * @brief matrix konstruktora
     * 
     * @param row hosz
     * @param col szélesség
     */
    Matrix(int row =1, int col=1):row(row),col(col){
        matrix = new T*[row];
        for(int i = 0; i< row; i++){
            matrix[i] = new T[col];
        }
    }
    /**
     * @brief Másoló konstruktor
     * 
     * @param rhs másolando objektum
     */
    Matrix(const Matrix<T> &rhs):row(rhs.row),col(rhs.col){
        matrix = new T*[row];
        for(int i = 0; i< row; i++){
            matrix[i] = new T[col];
            for(int j = 0; j < col; j++){
                matrix[i][j] = rhs.matrix[i][j];
            }
        }
    }
    //destruktor
    /**
     * @brief a Matrix Destruktora amely felszabadítja a dinamikusan foglalt 2D-tömböt
     * 
     */
    ~Matrix(){
        for(int i = 0; i< row; i++){
            delete[] matrix[i];
        }
        delete[] matrix;
    }
    //getter
    /**
     * @brief lekérdezi a matrix hosszát
     * 
     * @return matrix hossza
     */
    int getRow()const{return row;}
    /**
     * @brief lekérdezi a matrix szélességét
     * 
     * @return mátrix szélessége
     */
    int getCol()const{return col;}
    //operators
    /**
     * @brief ezzel a fügvénnyel lehet elérni a matrix egyes elemeit
     * 
     * @param r sorszám
     * @param c oszlopszám
     * @return a matrix r. sor c. oszlop eleme 
     */
    T& operator()(int r, int c){
        if(c >= col ||r >= row)
            throw std::out_of_range("out of range");
        return matrix[r][c];
    }
    const T operator()(int r, int c)const{
        if(c >= col ||r >= row)
            throw std::out_of_range("out of range");
        return matrix[r][c];
    }
    /**
     * @brief két azonos méretű mátrixot add össze
     * 
     * @param rhs jobb oldali mátrix
     * @return összeg mátrix 
     */
    Matrix<T> operator+(const Matrix<T>& rhs)const{
        if(row != rhs.row || col != rhs.col){
            throw std::length_error("the shape doesn't match");
        }
        Matrix<T> res(row, col);
        for(int i = 0; i<row; i++){
            for(int j = 0; j<col; j++){
                res(i,j) = matrix[i][j] +rhs(i,j);
            }
        }
        return res;
    }
    /**
     * @brief Hadamard-szorzat, amely elementként szorozza össze a mátrixokat
     * 
     * @param rhs jobb oldali mátrix
     * @return eredmény mátrix 
     */
    Matrix<T> operator*(const Matrix<T>& rhs)const{
        if(row != rhs.row || col != rhs.col){
            throw std::length_error("the shape doesn't match");
        }
        Matrix<T> res(row, col);
        for(int i = 0; i<row; i++){
            for(int j = 0; j<col; j++){
                res(i,j) = matrix[i][j] * rhs(i,j);
            }
        }
        return res;
    }
    Matrix<T>& operator=(const Matrix<T>& rhs) {
        if (this == &rhs)
            return *this;

        for (int i = 0; i < row; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;

        row = rhs.row;
        col = rhs.col;
        matrix = new T*[row];
        for (int i = 0; i < row; i++) {
            matrix[i] = new T[col];
            for (int j = 0; j < col; j++) {
                matrix[i][j] = rhs(i, j);
            }
        }
        return *this;
    }/**
     * @brief mátrix kivonás
     * 
     * @param rhs jobb oldali mátrix
     * @return eredmény mátrix
     */
    Matrix<T> operator-(const Matrix<T>& rhs){
        if(row != rhs.row || col != rhs.col)
            std::cout<<"-the shape doesn't match"<<"("<<row<<","<<col<<") , ("<<rhs.row<<","<<rhs.col<<")\n";

        Matrix<T> res(row, col);
        for(int i = 0; i<row; i++){
            for(int j = 0; j<col; j++){
                res(i,j) = matrix[i][j] - rhs(i,j);
            }
        }
        return res;
    }/**
     * @brief kiiró fugvény
     * 
     * @param os 
     * @param m 
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream &os, const Matrix<T>& m){
        for(int i = 0; i< m.row; i++){
            os<<"[ ";
            for(int j = 0; j < m.col; j++){
                os<<std::setprecision(3)<<m.matrix[i][j]<<" ";
            }
            os<<"]\n";
        }
        return os;
    }
    //tagfugvenyek
    /**
     * @brief a mátrixot -0.5 és 0.5 közötti véletlen számokkal tölti fel
     * 
     */
    void mrand(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-0.5, 0.5);

        for(int i = 0; i < row; i++) {
            for(int j = 0; j < col; j++) {
                matrix[i][j] = dis(gen);
            }
        }
    }
    /**
     * @brief a mátrix minden elemét nullával tölti fel
     * 
     */
    void zeros(){
        for(int i = 0; i<row; i++){
            for(int j = 0; j<col; j++){
                matrix[i][j] = 0;
            }
        }
    }
    /**
     * @brief ez a fügvény a mátrix trasznponáltjával tér vissza
     * 
     * @return mátrix transzponáltja 
     */
    Matrix<T> Transpose()const{
        Matrix<T> res(col, row);
        for(int i = 0; i<row; i++){
            for(int j = 0; j<col; j++){
                res(j,i) = matrix[i][j];
            }
        }
        return res;
    }
};
/**
 * @brief két mátrixot szoroz össze
 * 
 * @tparam matrix tipusa 
 * @param lhs baloldali mátrix
 * @param rhs jobboldali mátrix
 * @return eredménymátrix 
 */
template<typename T>
Matrix<T> dot(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.getCol() != rhs.getRow()) {
        throw std::invalid_argument("The shapes don't match");
    }

    Matrix<T> res(lhs.getRow(), rhs.getCol());

    for (int i = 0; i < lhs.getRow(); i++) {
        for (int j = 0; j < rhs.getCol(); j++) {
            T temp = 0.0;
            for (int k = 0; k < lhs.getCol(); k++) {
                temp += lhs(i, k) * rhs(k, j);
            }
            res(i, j) = temp;
        }
    }
    return res;
}

/**
 * @brief egy T tipusú számbol kivonja a mátrix elemeit és ezt egy uj matrixban tárolja el
 * 
 * @tparam tipus
 * @param lhs bal oldali szám
 * @param rhs jobb oldali mátrix
 * @return eredmény mátrix
 */
template<typename T>
Matrix<T> operator-(T lhs, const Matrix<T>& rhs){
    Matrix<T> res(rhs.getRow(),rhs.getCol());
    for(int i = 0; i<rhs.getRow(); i++){
        for(int j = 0; j<rhs.getCol(); j++){
            res(i,j) = lhs-rhs(i,j);
        }
    }
    return res;
}
/**
 * @brief egy konstansal szorozza meg a mátrixot
 * 
 * @tparam tipus
 * @param lhs bal oldali szám (konstans)
 * @param rhs jobb oldali mátrix
 * @return eredmény mátrix
 */
template<typename T>
Matrix<T> operator*(T lhs, const Matrix<T>& rhs){
    Matrix<T> res(rhs.getRow(),rhs.getCol());
    for(int i = 0; i<rhs.getRow(); i++){
        for(int j = 0; j<rhs.getCol(); j++){
            res(i,j) = lhs*rhs(i,j);
        }
    }
    return res;
}
/**
 * @brief ez a fügvény összeadja a mátrix összes elemét
 * 
 * @tparam tipus
 * @param m mátrix
 * @return mátrix összege
 */
template<typename T>
T sum(const Matrix<T>& m){
    T Sum = 0.0;
    for(int i = 0; i<m.getRow(); i++){
        for(int j = 0; j<m.getCol(); j++){
            Sum += m(i,j); 
        }
    }
    return Sum;
}
#endif
