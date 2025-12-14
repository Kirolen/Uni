import numpy as np
from logger import Logger


def piecewise_linear_poly(x, y):
    n = len(x) - 1

    Logger.print_section("КУСКОВО-ЛІНІЙНА ІНТЕРПОЛЯЦІЯ")
    Logger.print_info("Формула: L1^i(x) = (f_{i-1}·(x_i - x) + f_i·(x - x_{i-1})) / (x_i - x_{i-1})")
    Logger.print_info("Спрощена форма: L1^i(x) = a·x + b")

    for i in range(1, n+1):
        xi_1, xi = x[i-1], x[i]
        fi_1, fi = y[i-1], y[i]

        Logger.print_subsection(f"Інтервал [{xi_1:.2f}, {xi:.2f}]")

        denom = xi - xi_1

        Logger.print_step(1, 2, "Обчислення коефіцієнта a (кутовий коефіцієнт)")
        a = (fi - fi_1) / denom
        Logger.print_calculation(
            f"a = (f_{i} - f_{i-1}) / (x_{i} - x_{i-1}) = ({fi:.6f} - {fi_1:.6f}) / ({xi:.2f} - {xi_1:.2f})",
            f"{a:.6f}"
        )

        Logger.print_step(2, 2, "Обчислення коефіцієнта b (вільний член)")
        b = (fi_1 * xi - fi * xi_1) / denom
        Logger.print_calculation(
            f"b = (f_{i-1}·x_{i} - f_{i}·x_{i-1}) / (x_{i} - x_{i-1})",
            f"{b:.6f}"
        )
        Logger.print_calculation(
            f"b = ({fi_1:.6f}·{xi:.2f} - {fi:.6f}·{xi_1:.2f}) / {denom:.2f}",
            f"{b:.6f}"
        )

        sign = "+" if b >= 0 else ""
        polynomial_str = f"L1^{i}(x) = {a:.6f}·x {sign}{b:.6f}"

        Logger.print_result("Результат", polynomial_str)


def piecewise_quadratic_poly(x, y):
    n = len(x) - 1
    h = x[1] - x[0]

    Logger.print_section("КУСКОВО-КВАДРАТИЧНА ІНТЕРПОЛЯЦІЯ")
    Logger.print_info(f"Крок інтерполяції: h = {h:.2f}")
    Logger.print_info("Формула Лагранжа для трьох точок:")
    Logger.print_info("L2^i(x) = f_{i-1}·(x-x_i)·(x-x_{i+1})/(2h²) - f_i·(x-x_{i-1})·(x-x_{i+1})/h² + f_{i+1}·(x-x_{i-1})·(x-x_i)/(2h²)")

    for i in range(1, n):
        xi_1, xi, xi1 = x[i-1], x[i], x[i+1]
        fi_1, fi, fi1 = y[i-1], y[i], y[i+1]

        Logger.print_subsection(f"Інтервал [{xi_1:.2f}, {xi1:.2f}] (охоплює 3 вузли)")

        denom = 2 * h**2

        Logger.print_step(1, 4, "Розкриття першого терма")
        term1 = np.poly1d([1, -xi - xi1, xi * xi1]) * (fi_1 / denom)
        Logger.print_calculation(
            f"term1 = f_{i-1}·(x-{xi:.2f})·(x-{xi1:.2f})/(2h²)",
            f"{fi_1 / denom:.8f}·[(x-{xi:.2f})·(x-{xi1:.2f})]"
        )

        Logger.print_step(2, 4, "Розкриття другого терма")
        term2 = np.poly1d([1, -xi_1 - xi1, xi_1 * xi1]) * (-fi / (h**2))
        Logger.print_calculation(
            f"term2 = -f_{i}·(x-{xi_1:.2f})·(x-{xi1:.2f})/h²",
            f"{-fi / (h**2):.8f}·[(x-{xi_1:.2f})·(x-{xi1:.2f})]"
        )

        Logger.print_step(3, 4, "Розкриття третього терма")
        term3 = np.poly1d([1, -xi_1 - xi, xi_1 * xi]) * (fi1 / denom)
        Logger.print_calculation(
            f"term3 = f_{i+1}·(x-{xi_1:.2f})·(x-{xi:.2f})/(2h²)",
            f"{fi1 / denom:.8f}·[(x-{xi_1:.2f})·(x-{xi:.2f})]"
        )

        Logger.print_step(4, 4, "Сумування та спрощення")
        poly = term1 + term2 + term3
        coeffs = poly.coeffs
        sign_1 = "+" if coeffs[1] >= 0 else ""
        sign_0 = "+" if coeffs[2] >= 0 else ""

        polynomial_str = (
            f"L2^{i}(x) = {coeffs[0]:.6f}·x² {sign_1}{coeffs[1]:.6f}·x {sign_0}{coeffs[2]:.6f}"
        )

        Logger.print_result("Спрощений поліном", polynomial_str)

        Logger.print_info("Розподіл коефіцієнтів:")
        Logger.print_calculation(f"  a₂ (при x²)", f"{coeffs[0]:.8f}")
        Logger.print_calculation(f"  a₁ (при x¹)", f"{coeffs[1]:.8f}")
        Logger.print_calculation(f"  a₀ (вільний)", f"{coeffs[2]:.8f}")