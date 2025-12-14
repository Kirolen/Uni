import numpy as np

def jacobi_method_with_details(A, epsilon=1e-6, max_iterations=100):
    n = A.shape[0]
    A_k = A.copy()
    U_total = np.eye(n)

    iterations = []

    print("\nПочаткова матриця A_0 = A")

    for k in range(max_iterations):
        max_val = 0
        i_k, j_k = 0, 1

        for i in range(n):
            for j in range(i + 1, n):
                if abs(A_k[i, j]) > max_val:
                    max_val = abs(A_k[i, j])
                    i_k, j_k = i, j

        if k < 5:
            print(f"\n{'='*80}")
            print(f"Ітерація {k+1}")
            print('='*80)
            print(f"\nКрок 1: Пошук максимального позадіагонального елемента")
            print(f"max|a_ij| = |a_{i_k}{j_k}| = {max_val:.6f} (елемент на позиції [{i_k}][{j_k}])")

            print(f"\nКрок 2: Обчислення кута обертання φ")

        if A_k[i_k, i_k] == A_k[j_k, j_k]:
            phi = np.pi / 4 if A_k[i_k, j_k] > 0 else -np.pi / 4
            if k < 4:
                print(f"a_{i_k}{i_k} = a_{j_k}{j_k}, тому φ = π/4 = {phi:.6f}")
        else:
            numerator = 2 * A_k[i_k, j_k]
            denominator = A_k[i_k, i_k] - A_k[j_k, j_k]
            phi = 0.5 * np.arctan(numerator / denominator)

            if k < 5:
                print(f"φ = (1/2) * arctan(2*a_{i_k}{j_k} / (a_{i_k}{i_k} - a_{j_k}{j_k}))")
                print(f"φ = (1/2) * arctan(2*{A_k[i_k, j_k]:.6f} / ({A_k[i_k, i_k]:.6f} - {A_k[j_k, j_k]:.6f}))")
                print(f"φ = (1/2) * arctan({numerator:.6f} / {denominator:.6f})")
                print(f"φ = (1/2) * arctan({numerator/denominator:.6f})")
                print(f"φ = (1/2) * {2*phi:.6f} = {phi:.6f} рад ({np.degrees(phi):.2f}°)")

        U_k = np.eye(n)
        U_k[i_k, i_k] = np.cos(phi)
        U_k[j_k, j_k] = np.cos(phi)
        U_k[i_k, j_k] = -np.sin(phi)
        U_k[j_k, i_k] = np.sin(phi)

        if k < 5:
            print(f"\nКрок 3: Формування матриці обертання U_{k+1}")
            print(f"cos(φ) = {np.cos(phi):.6f}, sin(φ) = {np.sin(phi):.6f}")
            print(f"U_{k+1} (елементи на позиціях [{i_k}][{i_k}], [{j_k}][{j_k}], [{i_k}][{j_k}], [{j_k}][{i_k}]):")
            for i, row in enumerate(U_k):
                print(f"  [{', '.join(f'{val:7.4f}' for val in row)}]")

        A_k_new = U_k.T @ A_k @ U_k
        U_total = U_total @ U_k

        off_diagonal_sum = sum(A_k_new[i, j]**2 for i in range(n) for j in range(n) if i != j)

        if k < 5:
            print(f"\nКрок 4: Обчислення A_{k+2} = U_{k+1}^T * A_{k+1} * U_{k+1}")
            print(f"Матриця A_{k+2}:")
            for i, row in enumerate(A_k_new):
                print(f"  [{', '.join(f'{val:7.4f}' for val in row)}]")

            print(f"\nКрок 5: Перевірка умови припинення")
            print(f"t(A_{k+2}) = Σ(a_ij)² = {off_diagonal_sum:.6e}")
            print(f"t(A_{k+2}) {'<=' if off_diagonal_sum <= epsilon else '>'} ε = {epsilon}")

        A_k = A_k_new

        iterations.append({
            'iteration': k + 1,
            'i_k': i_k,
            'j_k': j_k,
            'phi': phi,
            'max_off_diagonal': max_val,
            'A_k': A_k.copy(),
            'off_diagonal_sum': off_diagonal_sum
        })

        if off_diagonal_sum <= epsilon:
            print(f"\nУмова припинення виконана на ітерації {k+1}")
            break

    if len(iterations) > 5 and off_diagonal_sum > epsilon:
        print(f"\n... (виконано ще {len(iterations) - 4} ітерацій) ...")
        print(f"\nОстаточна матриця A_{len(iterations)} після {len(iterations)} ітерацій:")
        for i, row in enumerate(A_k):
            print(f"  [{', '.join(f'{val:7.4f}' for val in row)}]")
        print(f"t(A_{len(iterations)}) = {off_diagonal_sum:.6e}")

    eigenvalues = np.diag(A_k)
    eigenvectors = U_total

    return eigenvalues, eigenvectors, iterations
