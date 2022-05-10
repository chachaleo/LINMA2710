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
        this->mData[i] = 0.0;
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
    assert(i > 0);
    assert(i <= Size(1));
    assert(j > 0);
    assert(j <= Size(2));
    return mData[(i-1) * numCols + (j - 1)];
}

double& SubMatrix::operator()(int i, int j)
{
    assert(i > 0);
    assert(i <= Size(1));
    assert(j > 0);
    assert(j <= Size(2));
    return mData[(i-1) * numCols + (j-1)];
}

void SubMatrix::SendReceiveRows(int rowsend, int ranksend, int rowrecv, int rankrecv, int tag, MPI_Comm comm)
{
    assert((rowsend - 1) * numCols > -1);
    assert((rowsend - 1) * numCols < numCols * numRows);
    assert((rowrecv - 1) * numCols > -1);
    assert((rowrecv - 1) * numCols < numCols * numRows);
    
    MPI_Sendrecv(&mData[(rowsend - 1) * numCols], numCols, MPI_DOUBLE, ranksend, tag, &mData[(rowrecv - 1) * numCols], numCols ,MPI_DOUBLE, rankrecv, tag, comm, MPI_STATUS_IGNORE);
}




void SubMatrix::SendReceiveColumns(int colsend, int ranksend, int colrecv, int rankrecv, int tag, MPI_Comm comm) {
	
	MPI_Datatype column_type;
	MPI_Type_vector(numRows, 1, numCols, MPI_DOUBLE, &column_type);
	MPI_Type_commit(&column_type);
	MPI_Sendrecv(&mData[colsend - 1], 1, column_type, ranksend, tag, &mData[colrecv - 1], 1, column_type, rankrecv, tag, comm, MPI_STATUS_IGNORE);
	MPI_Type_free(&column_type);
}