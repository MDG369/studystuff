from math import sin


class Matrix:
    def __init__(self, N, M=None, numbers=None):

        if M is None:
            self.A = [[0]*N for _ in range(N)]
            self.N = N
        else:
            self.A = [[0]* M for _ in range(N)]
            self.N = N

    def addDiagonal(self, diag, diagNum):
        i = 0
        if diagNum >= 0:
            if len(diag) == len(self.A):
                for num in diag:
                    if diagNum + i >= len(self.A):
                        break
                    self.A[i][diagNum+i] = num
                    i += 1
            else:
                print("Length of diag is not equal to matrix size")
        else:
            for num in diag:
                if diagNum + i >= len(self.A):
                    break
                if diagNum + i + 1 > 0:
                    self.A[i][diagNum + i] = num
                i += 1


    def addRow(self, row, rowNum):
        self.A.insert(rowNum, row)

    def getDiagonal(self, diagNum):
        diag = []
        for i in range(N-diagNum):
            diag[i] = self.A[i][i+diagNum]
        return diag

    def createVectorB(self):
        b = []
        for i in range(self.N):
            b.append(sin(i * (4 + 1)))
        return b


