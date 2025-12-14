import numpy as np
from logger import Logger

def cubic_spline(x, y, d2_start=None, d2_end=None):
    n = len(x) - 1
    h = np.zeros(n+1)

    Logger.print_section("ЕТАП 1: Обчислення кроків h_i")
    Logger.print_info("Обчислюємо відстані між сусідніми вузлами")

    for i in range(1, n+1):
        h[i] = x[i] - x[i-1]
        Logger.print_calculation(f"h[{i}] = x[{i}] - x[{i-1}] = {x[i]} - {x[i-1]}", f"{h[i]:.6f}")

    Logger.print_success(f"Обчислено {n} значень h_i")

    c = np.zeros(n+1)

    if n-1 > 0:
        A = np.zeros((n-1, n-1))
        B = np.zeros(n-1)

        Logger.print_section("ЕТАП 2: Формування системи рівнянь для c_i")
        Logger.print_info("Будуємо тридіагональну систему Ac = B")

        for i in range(1, n):
            A[i-1, i-1] = 2 * (h[i] + h[i+1])

            if i-1 >= 1:
                A[i-1, i-2] = h[i]
            if i <= n-2:
                A[i-1, i] = h[i+1]

            B[i-1] = 6 * ((y[i+1] - y[i]) / h[i+1] - (y[i] - y[i-1]) / h[i])

            Logger.print_subsection(f"Рівняння {i-1}:")
            Logger.print_calculation(
                f"{h[i]:.4f}·c[{i-1}] + {2*(h[i]+h[i+1]):.4f}·c[{i}] + {h[i+1]:.4f}·c[{i+1}]"
            )
            Logger.print_calculation(
                f"6·(({y[i+1]:.2f}-{y[i]:.2f})/{h[i+1]:.2f} - ({y[i]:.2f}-{y[i-1]:.2f})/{h[i]:.2f})",
                f"{B[i-1]:.6f}"
            )

        if d2_start is not None:
            B[0] -= h[1] * d2_start
            Logger.print_info(f"Застосовано граничну умову на початку: s''(x[0]) = {d2_start:.6f}")
            Logger.print_calculation(f"B[0] = B[0] - h[1]·d2_start", f"{B[0]:.6f}")

        if d2_end is not None:
            B[-1] -= h[n] * d2_end
            Logger.print_info(f"Застосовано граничну умову в кінці: s''(x[n]) = {d2_end:.6f}")
            Logger.print_calculation(f"B[n-1] = B[n-1] - h[n]·d2_end", f"{B[-1]:.6f}")

        Logger.print_subsection("Розв'язання системи методом Гауса")
        c[1:n] = np.linalg.solve(A, B)
        Logger.print_success("Систему успішно розв'язано")

    if d2_start is not None:
        c[0] = d2_start
    if d2_end is not None:
        c[n] = d2_end

    Logger.print_section("ЕТАП 3: Знайдені коефіцієнти c_i")
    for i in range(n+1):
        Logger.print_result(f"c[{i}]", f"{c[i]:.8f}")

    a = np.zeros(n+1)
    b = np.zeros(n+1)
    d = np.zeros(n+1)

    Logger.print_section("ЕТАП 4: Обчислення решти коефіцієнтів (a, b, d)")

    for i in range(1, n+1):
        Logger.print_subsection(f"Інтервал [{x[i-1]:.2f}, {x[i]:.2f}]")

        a[i] = y[i]
        Logger.print_calculation(f"a[{i}] = y[{i}]", f"{a[i]:.6f}")

        d[i] = (c[i] - c[i-1]) / h[i]
        Logger.print_calculation(
            f"d[{i}] = (c[{i}] - c[{i-1}]) / h[{i}] = ({c[i]:.6f} - {c[i-1]:.6f}) / {h[i]:.6f}",
            f"{d[i]:.6f}"
        )

        b[i] = h[i]/2 * c[i] - h[i]**2/6 * d[i] + (y[i] - y[i-1]) / h[i]
        Logger.print_calculation(
            f"b[{i}] = h[{i}]/2·c[{i}] - h[{i}]²/6·d[{i}] + (y[{i}]-y[{i-1}])/h[{i}]",
            f"{b[i]:.6f}"
        )

    Logger.print_success("Всі коефіцієнти обчислено")

    return a, b, c, d, h


def spline_polynomial(a, b, c, d, x):
    p3 = d / 6
    p2 = c / 2 - d * x / 2
    p1 = b - c * x + d * x**2 / 2
    p0 = a - b * x + c / 2 * x**2 - d / 6 * x**3

    return p3, p2, p1, p0


def print_spline_summary(x, y, a, b, c, d):
    Logger.print_section("ПІДСУМКОВА ТАБЛИЦЯ КОЕФІЦІЄНТІВ")

    n = len(x) - 1
    headers = ["i", "x_i", "y_i", "a_i", "b_i", "c_i", "d_i"]
    Logger.print_table_header(headers)

    for i in range(0, n+1):
        values = [
            i,
            f"{x[i]:.2f}",
            f"{y[i]:.2f}",
            f"{a[i]:.4f}",
            f"{b[i]:.4f}",
            f"{c[i]:.4f}",
            f"{d[i]:.4f}"
        ]
        Logger.print_table_row(values)


def print_spline_polynomials(x, a, b, c, d):
    Logger.print_section("КУБІЧНІ МНОГОЧЛЕНИ СПЛАЙНА")
    Logger.print_info("Представлення у формі: s_i(x) = p₃·x³ + p₂·x² + p₁·x + p₀")

    n = len(x) - 1
    for i in range(1, n+1):
        p3, p2, p1, p0 = spline_polynomial(a[i], b[i], c[i], d[i], x[i])

        sign_p2 = "+" if p2 >= 0 else ""
        sign_p1 = "+" if p1 >= 0 else ""
        sign_p0 = "+" if p0 >= 0 else ""

        polynomial_str = (
            f"s_{i}(x) = {p3:.6f}·x³ {sign_p2}{p2:.6f}·x² "
            f"{sign_p1}{p1:.6f}·x {sign_p0}{p0:.6f}"
        )

        Logger.print_polynomial(f"[{x[i-1]:.2f}, {x[i]:.2f}]", polynomial_str)