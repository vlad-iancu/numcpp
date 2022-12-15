import numpy as np

el = 0
A = np.zeros((4, 4, 4), order='F')

for j in range(4):
    for i in range(4):
        for k in range(4):
            A[k][i][j] = el
            el = el + 1


print(A)
print(A.shape)
print(A.strides)

B = A[0, 1, 0:4]


print(B)
print(B.shape)
print(B.strides)
