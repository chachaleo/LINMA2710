#include <cmath>
#include <cstdio>
#include <cassert>
#include <vector>
#include <iostream>

#include "mpi.h"
#include "SubMatrix.hpp"

SubMatrix::SubMatrix(int numRows, int numCols)
{
    this->numRows = numRows;
    this->numCols = numCols;

    this-> mData = new double [numCols * numRows];
    for (int i = 0; i < (numCols * numRows); i++)
    {
        mData[i] = 0;
    }
}   

SubMatrix::~SubMatrix()
{
    delete[] mData;
}

int SubMatrix::Size(int i) const
{
    if(i == 1) return numRows;
    if(i == 2) return numCols;
    return 0;
}

double SubMatrix::Read(int i, int j) const
{
    return mData[(i-1) + numCols * (j - 1)]; //---------Moins 1 car 1-based indexing ?
}

double& SubMatrix::operator()(int i, int j)
{
    assert(i > -1);
    assert(i <= Size(1));
    assert(j > -1);
    assert(j <= Size(2));
    return mData[(i-1) * numCols + (j-1)];
}

void SubMatrix::SendReceiveRows(int rowsend, int ranksend, int rowrecv, int rankrecv, int tag, MPI_Comm comm)
{
    MPI_Sendrecv(&mData[(rowsend - 1) * numCols], numCols, MPI_DOUBLE, ranksend, tag, &mData[(rowrecv - 1) * numCols], numCols ,MPI_DOUBLE, rankrecv, tag, comm, MPI_STATUS_IGNORE);
}



void SubMatrix::SendReceiveColumns(int colsend, int ranksend, int colrecv, int rankrecv, int tag, MPI_Comm comm)
{

    double * buffer = new double[numRows];
    double * col = new double [numRows];

    for(int i = 0; i < numRows; i++)
    {
        col[i] = mData[i * numCols + (colsend-1)];
        std::cout << "col : " << col[i] << std::endl;
    }

    MPI_Sendrecv(col, numRows, MPI_DOUBLE, ranksend, tag, buffer, numRows, MPI_DOUBLE, rankrecv, tag, comm, MPI_STATUS_IGNORE);

    
    for(int i = 0; i < numRows; i++)
    {
        mData[i * numCols + (colrecv-1)] = buffer[i];
        std::cout << "buffer : " << buffer[i] << std::endl;
    }
    
}



    //MPI_Send(col, n, MPI_DOUBLE, ranksend, tag, comm);//----------C'est quoi n ? 
    //MPI_Recv(&number, n, MPI_DOUBLE, rankrecv, tag, comm, MPI_STATUS_IGNORE);//----------C'est quoi n/Maxcount et c'est quoi l'adresse de la personne qui envoit et recoit? 