import numpy as np
from power_method import find_min_eigenvalue_with_details
from jacobi_method import jacobi_method_with_details
from newton_method import modified_newton_method

def main():
    A = np.array([
        [3, 0, 1, 1],
        [0, 4, 1, 2],
        [1, 1, 2, 0],
        [1, 2, 0, 3]
    ], dtype=float)

    # A = np.array([
    #     [2, -1, 0, 0],
    #     [-1, 2, -1, 0],
    #     [0, -1, 2, -1],
    #     [0, 0, -1, 2]
    # ], dtype=float)

    print("="*80)
    print("ЗНАХОДЖЕННЯ ВЛАСНИХ ЗНАЧЕНЬ МАТРИЦІ")
    print("="*80)
    print("\nВихідна матриця A:")
    for row in A:
        print(f"  [{', '.join(f'{val:3.0f}' for val in row)}]")

    print("\n" + "="*80)
    print("1. СТЕПЕНЕВИЙ МЕТОД (знаходження мінімального власного значення)")
    print("="*80)

    epsilon = 1e-6
    lambda_min, eigenvector_min, iterations_min = find_min_eigenvalue_with_details(A, epsilon)

    print(f"\nРезультат:")
    print(f"λ_min(A) = {lambda_min:.6f}")
    print(f"Власний вектор (нормалізований): {eigenvector_min}")

    print("\n" + "="*80)
    print("2. МЕТОД ОБЕРТАНЬ ЯКОБІ (знаходження всіх власних значень)")
    print("="*80)

    eigenvalues_jacobi, eigenvectors_jacobi, iterations_jacobi = jacobi_method_with_details(A, epsilon=1e-6, max_iterations=100)

    print("\n" + "="*80)
    print("РЕЗУЛЬТАТИ")
    print("="*80)

    sorted_indices = np.argsort(eigenvalues_jacobi)
    eigenvalues_sorted = eigenvalues_jacobi[sorted_indices]
    eigenvectors_sorted = eigenvectors_jacobi[:, sorted_indices]

    print("\nВласні значення (метод Якобі):")
    for i, lambda_val in enumerate(eigenvalues_sorted):
        print(f"  λ_{i+1} = {lambda_val:.6f}")

    print("\nВласні вектори (стовпці матриці U):")
    for i, row in enumerate(eigenvectors_sorted):
        print(f"  [{', '.join(f'{val:7.4f}' for val in row)}]")

    print("\n" + "="*80)
    print("ПЕРЕВІРКА (numpy.linalg.eig)")
    print("="*80)

    eigenvalues_numpy, eigenvectors_numpy = np.linalg.eig(A)
    sorted_indices_numpy = np.argsort(eigenvalues_numpy)
    eigenvalues_numpy_sorted = eigenvalues_numpy[sorted_indices_numpy]

    print("\nВласні значення (numpy):")
    for i, lambda_val in enumerate(eigenvalues_numpy_sorted):
        print(f"  λ_{i+1} = {lambda_val:.6f}")

    print("\nПорівняння мінімального власного значення:")
    print(f"  Степеневий метод: λ_min = {lambda_min:.6f}")
    print(f"  Метод Якобі:      λ_min = {eigenvalues_sorted[0]:.6f}")
    print(f"  NumPy:            λ_min = {eigenvalues_numpy_sorted[0]:.6f}")

    print("\n" + "="*80)

    print("\n\n" + "="*80)
    print("3. МОДИФІКОВАНИЙ МЕТОД НЬЮТОНА")
    print("="*80)

    x0_vec = np.array([1.0, 0.5])
    result = modified_newton_method(x0_vec, epsilon=1e-6, max_iterations=100)

    print("\n" + "="*80)

if __name__ == "__main__":
    main()
