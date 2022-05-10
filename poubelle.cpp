        for(int i = 0; i < b.LastReadRow() ; i++)
        {
            for(int j = 0; j < b.LastReadColumn() ; j++)
        {
            std::cout << "SHOULD BE 0 : " <<  b.m->Read(i+1,j+1) << std::endl;
        }
        }

            assert(0 * numCols + (colsend-1) > -1);
    assert((numRows - 1) * numCols + (colsend-1) < numCols * numRows);
    assert(0 * numCols + (colrecv-1) > -1);
    assert((numRows - 1) * numCols + (colrecv-1) < numCols * numRows);

    double * buffer = new double[numRows];
    double * col = new double [numRows];
    for(int i = 0; i < numRows; i++)
    {
        buffer[i] = 0;
        col[i] = 0;

    }


    for(int i = 0; i < numRows; i++)
    {
        col[i] = mData[i * numCols + (colsend-1)];
        std::cout << "mdata : " <<  mData[i * numCols + (colsend-1)] << std::endl;
        std::cout << "col : " << col[i] << std::endl;
    }

    MPI_Sendrecv(col, numRows, MPI_DOUBLE, ranksend, tag, buffer, numRows, MPI_DOUBLE, rankrecv, tag, comm, MPI_STATUS_IGNORE);

    
    for(int i = 0; i < numRows; i++)
    {
        mData[i * numCols + (colrecv-1)] = buffer[i];
        std::cout << "buffer : " << buffer[i] << std::endl;
    }
    for(int i = 0; i < numRows * numCols; i++)
    {
        std::cout << "MDATA : " <<  mData[i] << std::endl;
        if(mData[i]  > 10){
            std::cout << "ALORS : " <<  i  << std::endl;
        }
        
    }
    delete[] buffer;
    delete[] col;



    void SubMatrix::SendReceiveColumns(int colsend, int ranksend, int colrecv, int rankrecv, int tag, MPI_Comm comm)
{

    MPI_Datatype stridesend;
    MPI_Type_vector(numCols,1,numRows,MPI_DOUBLE,&stridesend);
    
    MPI_Datatype striderecv;
    MPI_Type_vector(numCols,1,numRows,MPI_DOUBLE,&striderecv);

    MPI_Sendrecv(&mData[(colsend-1)], numRows, stridesend, ranksend, tag, &mData[(colrecv-1)], numRows, striderecv, rankrecv, tag, comm, MPI_STATUS_IGNORE);

}
