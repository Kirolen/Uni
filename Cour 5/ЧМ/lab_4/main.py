from chebyshev import get_chebyshev_nodes
from utils import compute_table
from lagrange import lagrange_polynomial
from interpolation import (
    estimate_error, 
    find_root_and_interval, 
    direct_interpolation_root, 
    inverse_interpolation_root
)

if __name__ == "__main__":
    a, b = 3, 7
    num_nodes = 5
    n = num_nodes - 1

    nodes = get_chebyshev_nodes(a, b, num_nodes)
    values = compute_table(nodes)
    P = lagrange_polynomial(nodes, values)
    error_bound = estimate_error(a, b, n)

    new_P, new_a, new_b, new_nodes, new_values = find_root_and_interval(a, b, num_nodes)

    if new_P is None:
        print("Не вдалося побудувати локальну інтерполяцію (немає дійсних коренів). Завершення.")
    else:
        direct_roots = direct_interpolation_root(new_P, new_a, new_b)
        inv_root = inverse_interpolation_root(new_nodes, new_values)