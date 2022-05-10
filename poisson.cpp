#include <cmath>
#include <cstdio>
#include <cassert>
#include <vector>
#include <iostream>
#include "poisson.hpp"
#include "utils.hpp"
#include "SubMatrix.hpp"
#include "DistributedMatrix.hpp"


void jacobi_iteration(const DistributedMatrix &a, const DistributedMatrix &f, DistributedMatrix &b)
{
    double h = 1/(a.Size(1)-1);

    for (int i = a.FirstWriteRow(); i <= a.LastWriteRow(); i++)//Write row /cols
    {
        for (int j = a.FirstWriteColumn(); j <= a.LastWriteColumn(); j++)
        {            
        //for(int j = 1; j < a.Size(2) + 1; j++){
            std::cout << "A READ : " <<  a.Read(i-1,j)<< std::endl;
            std::cout << "A READ : " <<  a.Read(i,j+1)<< std::endl;
            std::cout << "A READ : " <<  a.Read(i,j-1)<< std::endl;
            std::cout << "A READ : " <<  a.Read(i+1,j)<< std::endl;
            std::cout << "F READ : " <<  f.Read(i,j)<< std::endl;
            b(i,j) = (( a.Read(i-1,j) + a.Read(i,j+1) + a.Read(i,j-1) + a.Read(i+1,j)) - (h * h * f.Read(i,j)))/4;
            std::cout << "AVANT B :" << (( a.Read(i-1,j) + a.Read(i,j+1) + a.Read(i,j-1) + a.Read(i+1,j)) - (h * h * f.Read(i,j)))/4 << std::endl;
            std::cout << "B :" << b.Read(i,j) << std::endl;
        }
    }
}


double sum_squares(const DistributedMatrix &a, const DistributedMatrix &b)
{
    double error_local = 0.0;
    double error_global;
    for (int i = a.FirstWriteRow(); i <= a.LastWriteRow(); i++)
    {
        for (int j = a.FirstWriteColumn(); j <= a.LastWriteColumn(); j++)
        {
            error_local = error_local + ((a.Read(i,j) - b.Read(i,j)) * (a.Read(i,j) - b.Read(i,j))) ;
        }
    }
    MPI_Allreduce(&error_local, &error_global, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return error_global;
}
