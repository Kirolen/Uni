import numpy as np
from logger import Logger
from spline_utils import cubic_spline, print_spline_summary, print_spline_polynomials
from piecewise_interpolation import piecewise_linear_poly, piecewise_quadratic_poly


def print_initial_data(x, y, function_str):
    Logger.print_subsection("Вхідні дані")
    Logger.print_info(f"Функція: f(x) = {function_str}")
    Logger.print_array("Вузлові точки x", x)
    Logger.print_array("Значення функції y", y)

    print()
    for i, (xi, yi) in enumerate(zip(x, y)):
        Logger.print_result(f"f(x[{i}]) = f({xi:.2f})", f"{yi:.6f}")


def natural_spline_demo(x, y):
    Logger.print_header("ЧАСТИНА 1: ПРИРОДНИЙ КУБІЧНИЙ СПЛАЙН")
    Logger.print_info("Граничні умови: s''(x₀) = 0, s''(xₙ) = 0")

    print_initial_data(x, y, "x⁶ + 2x⁵ + 3x⁴ - 3")

    a, b, c, d, h = cubic_spline(x, y)

    print_spline_summary(x, y, a, b, c, d)
    print_spline_polynomials(x, a, b, c, d)

    Logger.print_success("Природний сплайн успішно побудовано!")


def constrained_spline_demo(x, y):
    Logger.print_header("ЧАСТИНА 2: СПЛАЙН З ЗАДАНИМИ ДРУГИМИ ПОХІДНИМИ")

    def f_second_derivative(x):
        return 30 * x**4 + 40 * x**3 + 36 * x**2

    d2_start = f_second_derivative(x[0])
    d2_end = f_second_derivative(x[-1])

    Logger.print_info("Друга похідна функції: f''(x) = 30x⁴ + 40x³ + 36x²")
    Logger.print_subsection("Обчислення граничних умов")
    Logger.print_calculation(
        f"f''(x[0]) = f''({x[0]:.2f}) = 30·{x[0]:.2f}⁴ + 40·{x[0]:.2f}³ + 36·{x[0]:.2f}²",
        f"{d2_start:.6f}"
    )
    Logger.print_calculation(
        f"f''(x[n]) = f''({x[-1]:.2f}) = 30·{x[-1]:.2f}⁴ + 40·{x[-1]:.2f}³ + 36·{x[-1]:.2f}²",
        f"{d2_end:.6f}"
    )

    print_initial_data(x, y, "x⁶ + 2x⁵ + 3x⁴ - 3")

    a, b, c, d, h = cubic_spline(x, y, d2_start=d2_start, d2_end=d2_end)

    print_spline_summary(x, y, a, b, c, d)
    print_spline_polynomials(x, a, b, c, d)

    Logger.print_success("Сплайн з граничними умовами успішно побудовано!")


def piecewise_interpolation_demo(x, y):
    Logger.print_header("ЧАСТИНА 3: КУСКОВО-ПОЛІНОМІАЛЬНА ІНТЕРПОЛЯЦІЯ")

    piecewise_linear_poly(x, y)

    Logger.print_success("Кусково-лінійну інтерполяцію виконано!")

    piecewise_quadratic_poly(x, y)

    Logger.print_success("Кусково-квадратичну інтерполяцію виконано!")


def main():
    Logger.print_header("ЛАБОРАТОРНА РОБОТА: ПОБУДОВА КУБІЧНИХ СПЛАЙНІВ")
    print(f"{Logger.COLORS['BOLD']}Тема:{Logger.COLORS['END']} Кубічні сплайни та кусково-поліноміальна інтерполяція")
    print(f"{Logger.COLORS['BOLD']}Мета:{Logger.COLORS['END']} Дослідити методи побудови кубічних сплайнів з різними граничними умовами\n")

    def f(x):
        return x**6 + 2*x**5 + 3*x**4 - 3

    x_lab = np.array([3.0, 5.0, 7.0])
    y_lab = f(x_lab)

    try:
        natural_spline_demo(x_lab, y_lab)

        constrained_spline_demo(x_lab, y_lab)

        piecewise_interpolation_demo(x_lab, y_lab)

        Logger.print_header("ВИКОНАННЯ ЗАВЕРШЕНО")
        Logger.print_success("Всі обчислення виконано успішно!")
        Logger.print_info("Результати готові до аналізу")

    except Exception as e:
        Logger.print_section("ПОМИЛКА")
        print(f"{Logger.COLORS['RED']}Виникла помилка під час виконання:{Logger.COLORS['END']}")
        print(f"{Logger.COLORS['RED']}{str(e)}{Logger.COLORS['END']}")
        raise


if __name__ == "__main__":
    main()