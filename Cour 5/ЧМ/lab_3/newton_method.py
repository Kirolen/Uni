import numpy as np

def F(x_vec):
    """
    Векторна функція F(x̄) = (f₁(x₁, x₂), f₂(x₁, x₂))ᵀ

    Система нелінійних рівнянь:
    f₁(x₁, x₂) = sin(x₁ - x₂) - x₁x₂ + 1 = 0
    f₂(x₁, x₂) = x₁² - x₂² - 0.75 = 0
    """
    x1, x2 = x_vec[0], x_vec[1]
    f1 = np.sin(x1 - x2) - x1 * x2 + 1
    f2 = x1**2 - x2**2 - 0.75
    return np.array([f1, f2])

def F_prime(x_vec):
    """
    Матриця Якобі F'(x̄)

    F'(x̄) = | ∂f₁/∂x₁   ∂f₁/∂x₂ |
            | ∂f₂/∂x₁   ∂f₂/∂x₂ |

    де:
    ∂f₁/∂x₁ = cos(x₁ - x₂) - x₂
    ∂f₁/∂x₂ = -cos(x₁ - x₂) - x₁
    ∂f₂/∂x₁ = 2x₁
    ∂f₂/∂x₂ = -2x₂
    """
    x1, x2 = x_vec[0], x_vec[1]

    df1_dx1 = np.cos(x1 - x2) - x2
    df1_dx2 = -np.cos(x1 - x2) - x1
    df2_dx1 = 2 * x1
    df2_dx2 = -2 * x2

    J = np.array([
        [df1_dx1, df1_dx2],
        [df2_dx1, df2_dx2]
    ])

    return J

def modified_newton_method(x0_vec, epsilon=1e-6, max_iterations=100):
    """
    Модифікований метод Ньютона для розв'язку системи нелінійних рівнянь

    Ітераційна формула: x̄^(k+1) = x̄^k - A₀F(x̄^k)
    де A₀ = (F'(x̄⁰))⁻¹ обчислюється один раз на початку

    Умова припинення: ||x̄^(k+1) - x̄^k||₁ ≤ ε
    """

    print("\n" + "="*80)
    print("МОДИФІКОВАНИЙ МЕТОД НЬЮТОНА")
    print("="*80)

    print("\nСистема нелінійних рівнянь:")
    print("f₁(x₁, x₂) = sin(x₁ - x₂) - x₁x₂ + 1 = 0")
    print("f₂(x₁, x₂) = x₁² - x₂² - 0.75 = 0")

    print(f"\nПочаткове наближення: x̄⁰ = ({x0_vec[0]:.6f}, {x0_vec[1]:.6f})")

    print("\n" + "="*80)
    print("ОБЧИСЛЕННЯ МАТРИЦІ A₀ = (F'(x̄⁰))⁻¹")
    print("="*80)

    F_prime_0 = F_prime(x0_vec)
    A0 = np.linalg.inv(F_prime_0)

    print(f"\nМатриця Якобі F'(x̄⁰) в точці x̄⁰ = ({x0_vec[0]:.6f}, {x0_vec[1]:.6f}):")
    print("F'(x̄⁰) = | ∂f₁/∂x₁   ∂f₁/∂x₂ |")
    print("         | ∂f₂/∂x₁   ∂f₂/∂x₂ |")

    print(f"\n∂f₁/∂x₁ = cos(x₁ - x₂) - x₂ = cos({x0_vec[0]:.6f} - {x0_vec[1]:.6f}) - {x0_vec[1]:.6f} = {F_prime_0[0,0]:.6f}")
    print(f"∂f₁/∂x₂ = -cos(x₁ - x₂) - x₁ = -cos({x0_vec[0]:.6f} - {x0_vec[1]:.6f}) - {x0_vec[0]:.6f} = {F_prime_0[0,1]:.6f}")
    print(f"∂f₂/∂x₁ = 2x₁ = 2×{x0_vec[0]:.6f} = {F_prime_0[1,0]:.6f}")
    print(f"∂f₂/∂x₂ = -2x₂ = -2×{x0_vec[1]:.6f} = {F_prime_0[1,1]:.6f}")

    print("\nF'(x̄⁰) =")
    for row in F_prime_0:
        print(f"  [{row[0]:9.6f}, {row[1]:9.6f}]")

    print(f"\nОбернена матриця A₀ = (F'(x̄⁰))⁻¹:")
    for row in A0:
        print(f"  [{row[0]:9.6f}, {row[1]:9.6f}]")

    print("\n" + "="*80)
    print("ІТЕРАЦІЙНИЙ ПРОЦЕС")
    print("="*80)
    print("\nФормула: x̄^(k+1) = x̄^k - A₀F(x̄^k)")
    print(f"Умова припинення: ||x̄^(k+1) - x̄^k||₁ ≤ ε = {epsilon}")

    x_current = x0_vec.copy()
    iterations = []

    for k in range(max_iterations):
        F_current = F(x_current)

        delta_x = A0 @ F_current

        x_next = x_current - delta_x

        diff = x_next - x_current
        norm_diff = np.linalg.norm(diff, ord=1)
        norm_F = np.linalg.norm(F_current, ord=1)

        iterations.append({
            'k': k,
            'x_current': x_current.copy(),
            'x_next': x_next.copy(),
            'F_current': F_current.copy(),
            'delta_x': delta_x.copy(),
            'norm_diff': norm_diff,
            'norm_F': norm_F
        })

        if k < 5 or norm_diff <= epsilon or k == max_iterations - 1:
            print(f"\n{'='*80}")
            print(f"Ітерація k = {k}")
            print('='*80)

            print(f"\nПоточне наближення: x̄^{k} = ({x_current[0]:.6f}, {x_current[1]:.6f})")

            print(f"\nОбчислення F(x̄^{k}):")
            print(f"f₁(x̄^{k}) = sin({x_current[0]:.6f} - {x_current[1]:.6f}) - {x_current[0]:.6f}×{x_current[1]:.6f} + 1")
            print(f"       = sin({x_current[0] - x_current[1]:.6f}) - {x_current[0]*x_current[1]:.6f} + 1")
            print(f"       = {F_current[0]:.6f}")

            print(f"f₂(x̄^{k}) = ({x_current[0]:.6f})² - ({x_current[1]:.6f})² - 0.75")
            print(f"       = {x_current[0]**2:.6f} - {x_current[1]**2:.6f} - 0.75")
            print(f"       = {F_current[1]:.6f}")

            print(f"\nF(x̄^{k}) = [{F_current[0]:.6f}, {F_current[1]:.6f}]ᵀ")
            print(f"||F(x̄^{k})||₁ = {norm_F:.6e}")

            print(f"\nОбчислення приросту Δx̄^{k} = A₀F(x̄^{k}):")
            print(f"Δx̄^{k} = [{A0[0,0]:9.6f}, {A0[0,1]:9.6f}] × [{F_current[0]:9.6f}]")
            print(f"         [{A0[1,0]:9.6f}, {A0[1,1]:9.6f}]   [{F_current[1]:9.6f}]")

            print(f"\nΔx₁ = {A0[0,0]:.6f}×{F_current[0]:.6f} + {A0[0,1]:.6f}×{F_current[1]:.6f} = {delta_x[0]:.6f}")
            print(f"Δx₂ = {A0[1,0]:.6f}×{F_current[0]:.6f} + {A0[1,1]:.6f}×{F_current[1]:.6f} = {delta_x[1]:.6f}")

            print(f"\nНове наближення:")
            print(f"x̄^{k+1} = x̄^{k} - Δx̄^{k}")
            print(f"x₁^{k+1} = {x_current[0]:.6f} - {delta_x[0]:.6f} = {x_next[0]:.6f}")
            print(f"x₂^{k+1} = {x_current[1]:.6f} - {delta_x[1]:.6f} = {x_next[1]:.6f}")

            print(f"\nx̄^{k+1} = ({x_next[0]:.6f}, {x_next[1]:.6f})")

            print(f"\nОцінка точності:")
            print(f"||x̄^{k+1} - x̄^{k}||₁ = |{diff[0]:.6f}| + |{diff[1]:.6f}| = {norm_diff:.6e}")

            F_next = F(x_next)
            norm_F_next = np.linalg.norm(F_next, ord=1)
            print(f"||F(x̄^{k+1})||₁ = {norm_F_next:.6e}")

        elif k == 5:
            print(f"\n... (проміжні ітерації до ітерації {max_iterations-1}) ...")

        if norm_diff <= epsilon:
            print(f"\n{'='*80}")
            print(f"ЗБІЖНІСТЬ ДОСЯГНУТА на ітерації k = {k}")
            print('='*80)
            print(f"||x̄^{k+1} - x̄^{k}||₁ = {norm_diff:.6e} ≤ ε = {epsilon}")
            break

        x_current = x_next

    print("\n" + "="*80)
    print("ФІНАЛЬНИЙ РЕЗУЛЬТАТ")
    print("="*80)

    print(f"\nРозв'язок знайдено за {len(iterations)} ітерацій:")
    print(f"x₁* = {x_current[0]:.10f}")
    print(f"x₂* = {x_current[1]:.10f}")

    print(f"\nПеревірка (підстановка в систему):")
    F_final = F(x_current)
    print(f"f₁(x̄*) = sin(x₁ - x₂) - x₁x₂ + 1 = {F_final[0]:.6e}")
    print(f"f₂(x̄*) = x₁² - x₂² - 0.75 = {F_final[1]:.6e}")
    print(f"||F(x̄*)||₁ = {np.linalg.norm(F_final, ord=1):.6e}")

    return x_current, iterations
