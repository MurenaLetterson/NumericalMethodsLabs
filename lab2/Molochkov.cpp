#include "header.hpp"

bool IfStop(const double* vectorXCurrent, const double* vectorXFollow, double* &vectorBuffer, const double epsilon1, const size_t n)
{
    // std::cout << "IfStop\n";
    VectorDiff(vectorXCurrent, vectorXFollow, vectorBuffer, n);
    double normDeltaX = CubicVectorNorm(vectorBuffer, n);

    // std::cout << "normdX = " << normDeltaX << " normC = " << normC << "\n";

    bool ifStop = (normDeltaX <= epsilon1);
    // std::cout << ifStop << "\n";
    return ifStop;
}

bool IfStopTS(const double* vectorX, const double epsilon, const size_t n, bool b)
{
    const double vectorXTrue[] = {6, -9, 12, 8};
    double vectorBuff[] = {0, 0, 0, 0};
    for(int i = 0; i < n; ++i)
    {
        vectorBuff[i] = vectorXTrue[i] - vectorX[i];
    }

    bool ifStop = (CubicVectorNorm(vectorBuff, n) < epsilon);
    if(ifStop || b)
    {
        std::cout << "|| x*-xk || " << CubicVectorNorm(vectorBuff, n) << std::endl;
    }

    return ifStop;
}

bool IfStop1(const double* vectorXCurrent, const double* vectorXFollow, double* vectorBuffer, const double epsilon, const size_t n)
{
    VectorDiff(vectorXCurrent, vectorXFollow, vectorBuffer, n);
    double norm = CubicVectorNorm(vectorBuffer, n);

    bool ifStop = norm < epsilon;

    if(ifStop)
    {
        std::cout << "|| xk-xk+1 || " << CubicVectorNorm(vectorBuffer, n) << std::endl;
    }

    return ifStop;
}

void GetCY_SimpleIter(const double* const* matrixA, const double* vectorB, const double thau, double** &matrixC, double* &vectorY, const size_t n)
{
    std::cout << "GetCY simple iter\n";
    MatrixCopy(matrixC, matrixA, n);
    MatrixDot(matrixC, thau, n);
    MatrixEDiff(matrixC, n);
    VectorCopy(vectorY, vectorB, n);
    VectorDot(vectorY, thau, n);
    std::cout << "norm C: " << CubicMatrixNorm(matrixC, n) << "\n";
}

double ResidualNorm(const double* const* matrixA, const double* vectorB, const double* vectorX, double* &vectorBuffer1, double* &vectorBuffer2, const size_t n)
{
    MatrixMult(matrixA, vectorX, vectorBuffer1, n);
    VectorDiff(vectorBuffer1, vectorB, vectorBuffer2, n);
    
    return CubicVectorNorm(vectorBuffer2, n);
}

void GetCY_Jacobi(const double* const* matrixA, const double* vectorB, double** &matrixC, double* &vectorY, const size_t n)
{
    std::cout << "GetCY Jacobi\n";
    for(int i = 0; i < n; ++i)
    {
        double Aii = matrixA[i][i];
        for(int j = 0; j < n; ++j)
        {
            if (i == j)
            {
                matrixC[i][j] = 0;
            }
            else
            {
                matrixC[i][j] = -matrixA[i][j] / Aii;
            }
        }
        vectorY[i] = vectorB[i] / Aii;
    }
    std::cout << "norm C: " << CubicMatrixNorm(matrixC, n) << "\n";
}

void Iterations(const double* const* matrixC, double* &vectorXCurrent, double* &vectorXFollow, const double* vectorY, double* &vectorBuffer, const double epsilon, const size_t n)
{
    VectorCopy(vectorXFollow, vectorXCurrent, n);
    std::cout << "Iterations\n";
    
    const double normC = CubicMatrixNorm(matrixC, n);
    const double epsilon1 = (1 - normC) / normC * epsilon;

    int iterationNumber = 1;

    do
    {
//        std::cout << "\niteration number = " << iterationNumber << "\n\n";
        iterationNumber++;

        VectorCopy(vectorXCurrent, vectorXFollow, n); // Xk=Xk+1
        MatrixMult(matrixC, vectorXCurrent, vectorBuffer, n); // Cx
//        LogVector("Cx:", vectorBuffer, n); 


        VectorAdd(vectorBuffer, vectorY, vectorXFollow, n); // Xk+1 = Cxk + y

//        LogVector("X:", vectorXCurrent, n);
//        LogVector("Xnext:", vectorXFollow, n);
    }
    while(iterationNumber < 449);
    LogVector("x:", vectorXCurrent, n);
    std::cout << "iterations: " << iterationNumber + 1 << "\n";
    IfStopTS(vectorXFollow, epsilon, n, 1);
}

void MCalculations(const double* const* matrixA, const double* vectorB, double** &matrixC, double* &vectorXCurrent, double* &vectorXFollow, double* &vectorY, double* &vectorBuffer, const double thau, const double epsilon, const size_t n)
{
    std::cout << "MCalculations\n";
    GetCY_SimpleIter(matrixA, vectorB, thau, matrixC, vectorY, n);
    //GetCY_Jacobi(matrixA, vectorB, matrixC, vectorY, n);
    Iterations(matrixC, vectorXCurrent, vectorXFollow, vectorY, vectorBuffer, epsilon, n);
    std::cout << "res norm: " << ResidualNorm(matrixA, vectorB, vectorXCurrent, vectorBuffer, vectorXFollow, n) << "\n";
}

