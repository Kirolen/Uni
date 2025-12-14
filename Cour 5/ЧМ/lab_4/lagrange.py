import sympy as sp

def lagrange_polynomial(nodes, values):
    print("\n=== КРОК 3: Побудова інтерполяційного полінома Лагранжа ===")
    x = sp.symbols('x')
    P = 0
    n = len(nodes)

    for k in range(n):
        print(f"\n--- Обчислення L_{k}(x) ---")
        xk = nodes[k]
        Lk = 1
        for j in range(n):
            if j != k:
                Lk *= (x - nodes[j]) / (xk - nodes[j])
        Lk_s = sp.simplify(Lk)
        print(f" L_{k}(x) = {Lk_s}")
        P += values[k] * Lk

    P = sp.expand(P)
    print("\n=== Кінцевий поліном Лагранжа ===")
    print("P(x) = ", P)
    print()
    return P