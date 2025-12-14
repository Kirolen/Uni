import numpy as np
from datetime import datetime
import time
import matplotlib.pyplot as plt
import os

# ANSI кольори для гарного виводу
class Colors:
    HEADER = "\033[95m"
    BLUE = "\033[94m"
    CYAN = "\033[96m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    RED = "\033[91m"
    BOLD = "\033[1m"
    END = "\033[0m"

def read_file(file_name):
    """Зчитування даних із файлу"""
    with open(file_name, 'r') as file:
        lines = file.readlines()
        input_data = []
        for line in lines:
            values = line.strip().split()
            row = [float(value) for value in values]
            input_data.append(row)
    return np.array(input_data).T

def finite_diff(y_vec_func, b_vec, b_values, delta=1e-5):
    """Чисельне обчислення похідних (градієнтів)"""
    n = len(y_vec_func(b_values))
    m = len(b_vec)
    deriv_matrix = np.zeros((n, m))
    
    for j in range(m):
        original_value = b_values[b_vec[j]]
        b_values[b_vec[j]] = original_value + delta
        y_plus = y_vec_func(b_values)
        b_values[b_vec[j]] = original_value - delta
        y_minus = y_vec_func(b_values)
        b_values[b_vec[j]] = original_value
        deriv_matrix[:, j] = (y_plus - y_minus) / (2 * delta)
    
    return deriv_matrix

def get_u_matr(a_matr, b_matr, u_matr, h):
    """Метод Рунге–Кутта для допоміжної матриці U"""
    b_arrayed = np.array(b_matr)
    k1 = h * (np.dot(a_matr, u_matr) + b_arrayed)
    k2 = h * (np.dot(a_matr, u_matr + k1 / 2) + b_arrayed)
    k3 = h * (np.dot(a_matr, u_matr + k2 / 2) + b_arrayed)
    k4 = h * (np.dot(a_matr, u_matr + k3) + b_arrayed)
    return u_matr + (k1 + 2 * k2 + 2 * k3 + k4) / 6

def get_y(a_matr, y_cur, h):
    """Метод Рунге–Кутта для стану системи"""
    k1 = h * np.dot(a_matr, y_cur)
    k2 = h * np.dot(a_matr, y_cur + k1 / 2)
    k3 = h * np.dot(a_matr, y_cur + k2 / 2)
    k4 = h * np.dot(a_matr, y_cur + k3)
    return y_cur + (k1 + 2 * k2 + 2 * k3 + k4) / 6

def init_matr(params):
    """Формування матриці системи"""
    c1, c2, c3, c4, m1, m2, m3 = params['c1'], params['c2'], params['c3'], params['c4'], params['m1'], params['m2'], params['m3']
    matr = [
        [0, 1, 0, 0, 0, 0],
        [-(c2 + c1) / m1, 0, c2 / m1, 0, 0, 0],
        [0, 0, 0, 1, 0, 0],
        [c2 / m2, 0, -(c2 + c3) / m2, 0, c3 / m2, 0],
        [0, 0, 0, 0, 0, 1],
        [0, 0, c3 / m3, 0, -(c4 + c3) / m3, 0]
    ]
    return np.array(matr)

def get_model_solution(params, y0, t_points, h=0.2):
    """Обчислення розв’язку моделі"""
    a_matrix = init_matr(params)
    y_current = y0
    y_solution = [y0]
    
    for _ in range(len(t_points) - 1):
        y_current = get_y(a_matrix, y_current, h)
        y_solution.append(y_current)
    
    return np.array(y_solution)

def approximate(y_matr, params, beta_symbols, beta_values, eps, h=0.2):
    """Ідентифікація параметрів з простим кольоровим логом українською"""
    start_time = time.time()
    print(f"\033[95m--- Початок ідентифікації параметрів ---\033[0m")
    print(f"Цільова точність: \033[93m{eps}\033[0m\n")

    all_params = {**params, **beta_values}
    beta_vector = np.array([beta_values[s] for s in beta_symbols])
    iteration = 0

    def y_vec_func(b_values):
        all_params.update(b_values)
        a_matrix = init_matr(all_params)
        return a_matrix @ y_approximation

    while True:
        all_params.update(beta_values)
        a_complete = init_matr(all_params)
        
        u_matr = np.zeros((6, 3))
        quality_degree = 0
        integral_part_inverse = np.zeros((3, 3))
        integral_part_mult = np.zeros((1, 3))
        y_approximation = y_matr[0]

        for i in range(len(y_matr)):
            b_derivative_matr = finite_diff(y_vec_func, beta_symbols, beta_values)
            integral_part_inverse += u_matr.T @ u_matr
            integral_part_mult += u_matr.T @ (y_matr[i] - y_approximation)
            quality_degree += (y_matr[i] - y_approximation).T @ (y_matr[i] - y_approximation)
            
            u_matr = get_u_matr(a_complete, b_derivative_matr, u_matr, h)
            y_approximation = get_y(a_complete, y_approximation, h)

        integral_part_inverse *= h
        integral_part_mult *= h
        quality_degree *= h

        delta_beta = np.linalg.inv(integral_part_inverse) @ integral_part_mult.flatten()
        beta_vector += delta_beta
        beta_values = {beta_symbols[i]: beta_vector[i] for i in range(3)}

        # Короткий та зрозумілий лог
        print(f"\033[94mІтерація {iteration+1}:\033[0m "
              f"m2={beta_vector[0]:.4f}, c3={beta_vector[1]:.4f}, m3={beta_vector[2]:.4f} "
              f"| Якість=\033[92m{quality_degree:.6e}\033[0m")

        if quality_degree < eps:
            end_time = time.time()
            execution_time = end_time - start_time
            print(f"\n\033[92m--- Ідентифікацію завершено ---\033[0m")
            print(f"Кількість ітерацій: {iteration+1}")
            print(f"Час виконання: {execution_time:.2f} с")
            print(f"Показник якості: {quality_degree:.6e}\n")
            print("\033[96mВизначені параметри:\033[0m")
            for param, value in beta_values.items():
                print(f"  {param} = {value:.6f}")
            return beta_values, iteration+1, execution_time, quality_degree

        iteration += 1



def plot_results(measured_data, model_solution, t_points):
    """Візуалізація результатів"""
    variables = ['x₁', 'dx₁/dt', 'x₂', 'dx₂/dt', 'x₃', 'dx₃/dt']
    fig, axes = plt.subplots(3, 2, figsize=(15, 12))
    axes = axes.flatten()
    
    for i, (ax, var) in enumerate(zip(axes, variables)):
        ax.plot(t_points, measured_data[:, i], 'r.', label='Виміряно', markersize=4)
        ax.plot(t_points, model_solution[:, i], 'b-', label='Модель')
        ax.set_title(f'{var}')
        ax.set_xlabel('Час')
        ax.set_ylabel('Значення')
        ax.grid(True)
        ax.legend()
    
    plt.tight_layout()
    plt.show()
    
def main():
    # Зчитування даних
    input_data = read_file('./y4.txt')
    t_points = np.arange(0, 0.2 * len(input_data), 0.2)
    
    # Початкові параметри
    params = {'c1': 0.14, 'c2': 0.3, 'c4': 0.12, 'm1': 12}
    to_approx = {'m2': 21, 'c3': 0.15, 'm3': 11}
    
    # Об'єднуємо всі параметри для виводу
    all_initial_params = {**params, **to_approx}
    
    print(f"{Colors.BOLD}{Colors.HEADER}Початкові параметри:{Colors.END}")
    for k, v in all_initial_params.items():
        print(f"  {k}: {v}")
    print()

    # Ідентифікація
    result, iterations, exec_time, quality = approximate(
        input_data, params, ['m2', 'c3', 'm3'], to_approx, 1e-6
    )
    
    # Обчислення моделі з новими параметрами
    final_params = {**params, **result}
    model_solution = get_model_solution(final_params, input_data[0], t_points)
    
    # Візуалізація
    plot_results(input_data, model_solution, t_points)


if __name__ == "__main__":
    main()
