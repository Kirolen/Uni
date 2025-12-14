import math
import numpy as np
import sympy as sp
from chebyshev import get_chebyshev_nodes
from utils import compute_table
from lagrange import lagrange_polynomial

def estimate_error(a, b, n):
    print("\n=== КРОК 4: Оцінка похибки інтерполяції ===")
    x = sp.symbols('x')
    f_expr = x**6 + 2*x**5 + 3*x**4 - 3

    f_deriv = sp.diff(f_expr, x, n+1)
    f_deriv_abs = sp.lambdify(x, sp.Abs(f_deriv), 'numpy')

    xs = np.linspace(a, b, 1000)
    M = np.max(f_deriv_abs(xs))

    error_bound = M / math.factorial(n+1) * ((b - a)**(n+1)) / (2**(2*n+1))
    print(f"M_{n+1} = {M:.6f}")
    print(f"Оцінка максимальної похибки на [{a},{b}]: {error_bound:.6f}\n")
    return error_bound


def find_root_and_interval(a, b, num_nodes):
    print("\n=== КРОК 5: Пошук кореня функції f(x) ===")
    x = sp.symbols('x')
    f_expr = x**6 + 2*x**5 + 3*x**4 - 3

    roots = sp.solve(f_expr, x)
    roots_real = []
    print("Символьні корені (усі):", roots)
    for r in roots:
        r_num = r.evalf()
        if abs(sp.im(r_num)) < 1e-12:
            roots_real.append(float(sp.re(r_num)))

    print("Знайдені дійсні корені функції f(x):")
    for r in roots_real:
        print(f"  x = {r:.12f}")

    if len(roots_real) == 0:
        print("Дійсних коренів не знайдено. Повертаємо початковий інтервал.")
        return None, a, b, None, None

    root = roots_real[0]
    print(f"\nОбраний корінь для побудови локального інтервалу: x = {root:.12f}")
    print("Пояснення: вибираємо перший дійсний корінь зі списку знайдених. "
          "Якщо потрібно інший корінь — змініть індекс у коді.")

    new_a = root - 0.5
    new_b = root + 0.5
    print(f"Інтервал навколо кореня: [{new_a:.12f}, {new_b:.12f}]")

    new_nodes = get_chebyshev_nodes(new_a, new_b, num_nodes)
    new_values = compute_table(new_nodes)
    new_P = lagrange_polynomial(new_nodes, new_values)

    return new_P, new_a, new_b, new_nodes, new_values

def direct_interpolation_root(P, a, b):
    print("\n=== ПРЯМА ІНТЕРПОЛЯЦІЯ (перевірка коренів P(x)=0)) ===")
    x = sp.symbols('x')

    roots = sp.solve(P, x)
    print("Усі корені, які повернув sympy.solve():", roots)

    roots_real_in_interval = []

    for r in roots:
        r_val = r.evalf()
        if abs(sp.im(r_val)) >= 1e-12:
            print(f"Корінь {r} — комплексний (ім). Відкидаємо: Im={sp.im(r_val):.3e}")
            continue

        r_real = float(sp.re(r_val))
        print(f"Корінь (дійсна): {r_real:.12f}")

        if a <= r_real <= b:
            print(f" -> Приймаємо: {r_real:.12f} знаходиться в інтервалі [{a:.12f}, {b:.12f}]")
            roots_real_in_interval.append(r_real)
        else:
            print(f" -> Відкидаємо: {r_real:.12f} НЕ належить інтервалу [{a:.12f}, {b:.12f}]")

    if not roots_real_in_interval:
        print("Результат: у заданому інтервалі коренів інтерполяційного полінома не знайдено.")
    else:
        print("\nРезультат: знайдені корені інтерполяційного полінома в інтервалі:")
        for r in roots_real_in_interval:
            print(f"  x = {r:.12f}  (взятий як наближений корінь f(x)=0 через пряму інтерполяцію)")

    return roots_real_in_interval

def inverse_lagrange_polynomial(nodes, values):
    print("\n=== ОБЕРНЕНА ІНТЕРПОЛЯЦІЯ — Побудова x = P_inv(y) ===")
    y = sp.symbols('y')
    P_inv = 0
    n = len(nodes)

    for k in range(n):
        yk = values[k]
        xk = nodes[k]
        Lk = 1
        for j in range(n):
            if j != k:
                Lk *= (y - values[j]) / (yk - values[j])
        Lk_s = sp.simplify(Lk)
        print(f"Базисний L_{k}(y) (спрощено) = {Lk_s}")
        P_inv += xk * Lk

    P_inv = sp.simplify(P_inv)
    print("\n=== Кінцевий обернений поліном (спрощений) ===")
    print("x = P_inv(y) = ", P_inv)
    print("Пояснення: цей поліном апроксимує x як функцію від y = f(x).")
    return P_inv

def inverse_interpolation_root(nodes, values):
    print("\n=== ОБЕРНЕНА ІНТЕРПОЛЯЦІЯ — Знаходження кореня f(x)=0 ===")
    P_inv = inverse_lagrange_polynomial(nodes, values)
    y = sp.Symbol('y')

    x_at_zero = P_inv.subs(y, 0)
    x_at_zero_eval = sp.N(x_at_zero)
    print(f"Підставляємо y=0 у P_inv(y): P_inv(0) = {x_at_zero}  -> чисельно: {x_at_zero_eval}")

    print("Пояснення: оскільки y = f(x), при f(x)=0 отримаємо y=0, отже корінь x приблизно P_inv(0).")

    try:
        x_val = float(x_at_zero_eval)
        print(f"Результат оберненої інтерполяції: x ≈ {x_val:.12f}")
        return x_val
    except (TypeError, ValueError):
        print("Увага: P_inv(0) не дає однозначного числового значення.")
        return None