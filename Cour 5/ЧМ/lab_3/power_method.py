import numpy as np

def power_method_with_details(A, epsilon=1e-6, max_iterations=1000):
    n = A.shape[0]
    x = np.ones(n)

    lambda_old = 0
    iterations = []

    print(f"\nПочатковий вектор x^0 = {x}")

    for k in range(max_iterations):
        x_new = A @ x

        m = np.argmax(np.abs(x_new))
        lambda_new = x_new[m] / x[m] if x[m] != 0 else x_new[m]

        iterations.append({
            'iteration': k + 1,
            'x_old': x.copy(),
            'x_new': x_new.copy(),
            'm': m,
            'lambda': lambda_new
        })

        if k < 5 or k == max_iterations - 1 or abs(lambda_new - lambda_old) < epsilon:
            print(f"\n--- Ітерація {k+1} ---")
            print(f"x̄^{k+1} = A * x̄^{k}")
            print(f"x̄^{k+1} = {x_new}")
            print(f"m = {m} (індекс максимального за модулем елемента)")
            print(f"λ^{k+1} = x^{k+1}_{m} / x^{k}_{m} = {x_new[m]:.6f} / {x[m]:.6f} = {lambda_new:.6f}")
            print(f"|λ^{k+1} - λ^{k}| = |{lambda_new:.6f} - {lambda_old:.6f}| = {abs(lambda_new - lambda_old):.6e}")
        elif k == 5:
            print(f"\n... (проміжні ітерації 6-{max_iterations}) ...")

        if abs(lambda_new - lambda_old) < epsilon:
            print(f"\nУмова припинення виконана: |λ^{k+1} - λ^{k}| = {abs(lambda_new - lambda_old):.6e} <= ε = {epsilon}")
            break

        x = x_new
        lambda_old = lambda_new

    x_normalized = x / np.linalg.norm(x)

    return lambda_new, x_normalized, iterations

def find_min_eigenvalue_with_details(A, epsilon=1e-6):
    n = A.shape[0]

    print("\nКрок 1: Обчислення ||A||_∞ (максимум сум модулів елементів рядків)")
    row_sums = []
    for i in range(n):
        row_sum = sum(abs(A[i, j]) for j in range(n))
        row_sums.append(row_sum)
        print(f"  Рядок {i}: |{A[i,0]:.0f}| + |{A[i,1]:.0f}| + |{A[i,2]:.0f}| + |{A[i,3]:.0f}| = {row_sum:.0f}")

    norm_inf = max(row_sums)
    print(f"\n||A||_∞ = max({', '.join(map(str, map(int, row_sums)))}) = {norm_inf:.0f}")

    print(f"\nКрок 2: Обчислення матриці B = ||A||_∞ * E - A")
    print(f"B = {norm_inf:.0f} * E - A")

    B = norm_inf * np.eye(n) - A

    print("\nМатриця B:")
    for i, row in enumerate(B):
        print(f"  [{', '.join(f'{val:6.1f}' for val in row)}]")

    print("\n" + "="*80)
    print("Крок 3: Застосування степеневого методу до матриці B")
    print("Знаходимо λ_max(B)")
    print("="*80)

    lambda_max_B, eigenvector, iterations = power_method_with_details(B, epsilon)

    print("\n" + "="*80)
    print("Крок 4: Обчислення мінімального власного значення")
    print("="*80)
    print(f"\nλ_min(A) = ||A||_∞ - λ_max(B)")
    print(f"λ_min(A) = {norm_inf:.6f} - {lambda_max_B:.6f} = {norm_inf - lambda_max_B:.6f}")

    lambda_min = norm_inf - lambda_max_B

    return lambda_min, eigenvector, iterations
