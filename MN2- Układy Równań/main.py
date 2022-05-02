import copy
from math import sin, sqrt
import plotly.graph_objects as go

import Matrix as mx
import Methods

def zE():
    N = [100, 500, 1000]
    time_jacobi = []
    time_gs = []
    time_lu = []
    for n in N:
        print("Rozmiar: ", n)
        matrix = mx.Matrix(n)
        matrix.addDiagonal([a1] * n, 0)
        matrix.addDiagonal([a2] * n, 1)
        matrix.addDiagonal([a3] * n, 2)
        matrix.addDiagonal([a2] * n, -1)
        matrix.addDiagonal([a3] * n, -2)
        b = matrix.createVectorB()
        time_jacobi.append(Methods.jacobi(matrix, b))
        time_gs.append(Methods.gauss_seidel(matrix, b))
        time_lu.append(Methods.LU_factorization(matrix, b))

    fig = go.Figure()
    fig.add_trace(go.Scatter(name="Jacobi", y=time_jacobi, x=N, mode='markers'))
    fig.add_trace(go.Scatter(name="Gauss-Seidl", y=time_gs, x=N, mode='markers'))
    fig.add_trace(go.Scatter(name="LU", y=time_lu, x=N, mode='markers'))
    fig.update_layout(title="Czas trwania w zależnosci od liczby zmiennych dla poszczególnych metod",
                      xaxis_title="Liczba zmiennych",
                      yaxis_title="Czas [s]")
    fig.data[0].line.color = "#ffe476"
    fig.data[1].line.color = "blue"
    fig.data[2].line.color = "red"
    fig.update_traces(marker_size=15)
    fig.show()

a1 = 5 + 3
a2 = -1
a3 = -1
N = 7 * 2 * 9
matrix1 = mx.Matrix(N)
matrix1.addDiagonal([a1]*N, 0)
matrix1.addDiagonal([a2]*N, 1)
matrix1.addDiagonal([a3]*N, 2)
matrix1.addDiagonal([a2]*N, -1)
matrix1.addDiagonal([a3]*N, -2)
b = matrix1.createVectorB()

N = 9 * 7 * 2
matrix2 = mx.Matrix(N)
matrix2.addDiagonal([3]*N, 0)
matrix2.addDiagonal([a2]*N, 1)
matrix2.addDiagonal([a3]*N, 2)
matrix2.addDiagonal([a2]*N, -1)
matrix2.addDiagonal([a3]*N, -2)
b = matrix2.createVectorB()

zE()

