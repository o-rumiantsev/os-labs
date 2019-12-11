import matplotlib.pyplot as plt
from statistics import mean
from random import randint
import numpy as np

def generate_coord(n, coords):
    coord = (randint(0, n -1), randint(0, n - 1))

    while coord in coords:
        coord = generate_coord(n, coords)

    return coord

def init_matrix(n, fullness):
    elements_count = n ** 2
    delegations = int(elements_count * fullness / 100) or 1

    if fullness == 100:
        return np.array([[1] * n for _ in range(n)]), delegations

    matrix = [[0] * n for _ in range(n)]
    coords = []

    for _ in range(delegations):
        coord = generate_coord(n, coords)
        coords.append(coord)

    for i, j in coords:
        matrix[i][j] = 1

    return np.array(matrix), delegations

def count_necessary_delegations(matrix):
    count = 0

    for i, row in enumerate(matrix):
        for j, element in enumerate(row):
            if element == 1 and check_is_necessary(i, j, matrix):
                count += 1

    return count

def check_is_necessary(i, j, matrix):
    unique_in_row = True
    unique_in_column = True

    for other_j, element in enumerate(matrix[i]):
        if other_j != j and element == 1:
            unique_in_row = False
            break

    for other_i, row in enumerate(matrix):
        if other_i != i and row[j] == 1:
            unique_in_column = False
            break

    return unique_in_row or unique_in_column

def mean_necessary_delegations_percent(n, fullness):
        results = []

        for _ in range(100):
            matrix, delegations = init_matrix(n, fullness)
            necessary_delegations = count_necessary_delegations(matrix)
            results.append(necessary_delegations / delegations)

        return mean(results)

def test(n):
    x_fullness = []
    y_results = []

    for fullness in range(1, 101):
        result = mean_necessary_delegations_percent(n, fullness)
        x_fullness.append(fullness)
        y_results.append(result)


    plt.subplot(3, 2, int((n - 10) / 5 + 1))
    plt.plot(x_fullness, y_results)
    plt.title(f'N = {n}')

test(10)
test(15)
test(20)
test(25)
test(30)

plt.show()
