import copy
from datetime import datetime
import Matrix as mx
from math import sqrt

def LU_factorization(A, b):
    time = datetime.now()
    iterations = 0
    N = len(A.A[0])
    ones = [1] * N
    l = mx.Matrix(N)
    l.addDiagonal(ones, 0)
    u = mx.Matrix(N)
    x = [1] * N
    y = [0] * N
    for i in range(N):
        for j in range(i + 1):
            u.A[j][i] += A.A[j][i]
            for k in range(j):
                u.A[j][i] -= l.A[j][k] * u.A[k][i]
        for j in range(i + 1, N):
            for k in range(i):
                l.A[j][i] -= l.A[j][k] * u.A[k][i]
            l.A[j][i] += A.A[j][i]
            l.A[j][i] /= u.A[i][i]

    for i in range(N):
        value = b[i]
        for j in range(i):
            value -= l.A[i][j] * y[j]

        y[i] = value / l.A[i][i]

    for i in range(N - 1, -1, -1):
        value = y[i]
        for j in range(i + 1, N):
            value -= u.A[i][j] * x[j]
        x[i] = value / u.A[i][i]

    res = subVector(dotProduct(A.A, x), b)

    print("LU factorization")
    print("Czas wykonania faktoryzacji LU: ", datetime.now() - time)
    print("Residuum: ", norm(res))
    return (datetime.now() - time).total_seconds()

def gauss_seidel(A, b):
    time = datetime.now()
    iterations = 0
    x_prev = [0] * len(A.A)
    x = copy.copy(x_prev)
    while True:
        for i in range(len(A.A)):
            value = b[i]
            for j in range(len(A.A)):
                if i != j:
                    value -= A.A[i][j] * x[j]
            value /= A.A[i][i]
            x[i] = value
        res = subVector(dotProduct(A.A, x), b)
        if norm(res) < pow(10, -9):
            break
        iterations += 1
    print("Gauss-Seidel method")
    print("Liczba iteracji: ", iterations)
    print("Czas wykonania metody GS: ", datetime.now() - time)
    return (datetime.now() - time).total_seconds()


def jacobi(A, b):
    time = datetime.now()
    iterations = 0
    x_prev = [0] * len(A.A[0])
    x = copy.copy(x_prev)
    while True:
        for i in range(len(A.A)):
            value = b[i]
            for j in range(len(A.A)):
                if i != j:
                    value -= A.A[i][j] * x[j]
            value /= A.A[i][i]
            x_prev[i] = value
        x = copy.copy(x_prev)
        res = subVector(dotProduct(A.A, x), b)
        if norm(res) < pow(10, -9):
            break
        iterations += 1
    print("Jacobi method")
    print("Liczba iteracji: ", iterations)
    print("Czas wykonania metody Jacobiego: ", datetime.now() - time)
    return (datetime.now() - time).total_seconds()

def norm(r):
    n = 0
    for i in range(len(r)):
        n += r[i]**2
    return sqrt(n);


def subVector(x, y):
    result = copy.copy(x)
    for i in range(len(x)):
        result[i] -= y[i]
    return result

def dotProduct(x, y):
    copyX = copy.copy(x)
    copyY = copy.copy(y)
    c = [0] * len(x)
    for i in range(len(x)):
        for j in range(len(x[0])):
            c[i] += copyX[i][j] * copyY[j]
    return c