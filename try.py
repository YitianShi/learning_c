def fib(j):
    dic = {0:1, 1:1}
    def dfs(i):
        if not i in dic.keys():
            dic[i] = dfs(i-1) + dfs(i-2)
        return dic[i]
    print(dfs(j))

def min_maze(k: int):
    mat = [[0]*k for _ in range(k)]
    x_m = [0, 1, 1]
    y_m = [1, 0, 1]
    r=len(mat)
    c=len(mat[0])    
    reg = [[0] * len(mat[0]) for _ in range (len(mat))]
    stc = [(0, 0)]
    dic = {(0, 0):0}
    d = 0
    while stc:
        i, j = stc.pop(-1)
        for a, b in zip(x_m, y_m):
            if i+a<r and i+a >=0 and j+b<c and j+b >=0 and reg[i+a][j+b] == 0 :
                dic[(i+a,j+b)]= dic[(i,j)] + 1
                reg[i+a][j+b] = dic[(i+a,j+b)]
                stc.insert(0, (i+a, j+b))
            if (k-1, k-1) in stc:
                break
    return 0

def count_maze(k : int):
    mat = [[0]*k for _ in range(k)]
    x_m = [0, -1, -1]
    y_m = [-1, 0, -1]
    r=len(mat)
    c=len(mat[0])
    reg = [[0] * len(mat[0]) for _ in range (len(mat))]
    def dfs(i, j):
        if reg[i][j] == 0:
            if not i and not j:
                return 1
            for a, b in zip(x_m, y_m):
                if i+a<r and i+a >=0 and j+b<c and j+b >=0:
                    reg[i][j] += dfs(i+a, j+b)
        return reg[i][j]
    print(dfs(r-1, c-1))
    return 0

    


def solution(A, B):
    # Implement your solution here
    num=max((max(A), max(B))) +1
    mat = [[0]*num for _ in range(num)]
    for k in range(num):
        mat[k][k] = 1
    visited = set()
    connection=[]
    for node_s in range(num):
        node = node_s
        connection = [node]
        while node in A:
            child = B[A.index(node)]
            connection.append(child)
            node = child
        if len(connection) == 1:
            continue
        for i in range(len(connection)):
            for j in range(i+1, len(connection)):
                mat[connection[j]][connection[i]] = 1
    
    for row in range(len(mat)):
        if sum(mat[row]) == num:
            return row

    return -1

A=[0,1,2,4,5]
B=[2,3,3,3,2]
print(solution(A, B))