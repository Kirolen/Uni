import time
import numpy as np
import matplotlib.pyplot as plt
import psutil
import os

from PseudoInverseMatrixMethods import pseudoInverseMatrix_Greville, pseudoInverseMatrix_MoorePenrose
from OperationsCounter import count_operations_greville, count_operations_moore_penrose
from ImageUtils import readImage


def calculateError(Y, Y_pred):
    error = np.linalg.norm(Y - Y_pred, ord=1)
    mse = np.mean((Y - Y_pred)**2)
    rmse = np.sqrt(mse)
    return error, mse, rmse


def calculateOperator(X, Y, inversion_func, op_counter, eps=1e-6, delta=1000):
    start_time = time.time()
    start_mem = psutil.Process(os.getpid()).memory_info().rss / 1024 / 1024

    print(f"  Розміри: X={X.shape}, Y={Y.shape}")

    # Знаходимо псевдообернену матрицю для X
    X_pinv, iters = inversion_func(X, eps=eps, delta=delta)
    print(f"  X_pinv shape: {X_pinv.shape}")

    # Обчислюємо оператор A
    A = Y @ X_pinv
    print(f"  A shape: {A.shape}")

    # Обчислюємо результат
    Y_pred = A @ X
    print(f"  Y_pred shape: {Y_pred.shape}")

    # Обрізаємо значення до допустимого діапазону [0, 1]
    Y_pred = np.clip(Y_pred, 0, 1)

    end_time = time.time()
    end_mem = psutil.Process(os.getpid()).memory_info().rss / 1024 / 1024

    time_used = end_time - start_time
    memory_used = end_mem - start_mem
    ops = op_counter(X.shape[0], X.shape[1], iters if iters is not None else 1)
    error, mse, rmse = calculateError(Y, Y_pred)

    return Y_pred, time_used, memory_used, ops, error, mse, rmse


def main():
    X, Y = readImage()

    print(f"Розміри матриць: X={X.shape}, Y={Y.shape}")

    print("\n=== Обчислення методом Мура—Пенроуза ===")
    Y_MP, time_MP, mem_MP, ops_MP, err_MP, mse_MP, rmse_MP = calculateOperator(
        X, Y, pseudoInverseMatrix_MoorePenrose, count_operations_moore_penrose
    )
    print(f"Час: {time_MP:.4f}s, Пам'ять: {mem_MP:.2f}MB, Операцій: {ops_MP}, RMSE: {rmse_MP:.6f}")

    print("\n=== Обчислення методом Гревілля ===")
    Y_G, time_G, mem_G, ops_G, err_G, mse_G, rmse_G = calculateOperator(
        X, Y, pseudoInverseMatrix_Greville, count_operations_greville
    )
    print(f"Час: {time_G:.4f}s, Пам'ять: {mem_G:.2f}MB, Операцій: {ops_G}, RMSE: {rmse_G:.6f}")

    print("\n=== Порівняння результатів ===")
    print(f"Різниця у часі: {abs(time_MP - time_G):.4f}s ({('Moore-Penrose' if time_MP < time_G else 'Greville')} швидше)")
    print(f"Різниця у пам'яті: {abs(mem_MP - mem_G):.2f}MB")
    print(f"Різниця у RMSE: {abs(rmse_MP - rmse_G):.6f}")

    # Візуалізація результатів
    fig = plt.figure(figsize=(18, 11))

    # Оригінальні зображення
    plt.subplot(2, 5, 1)
    plt.imshow(X, cmap='gray', vmin=0, vmax=1)
    plt.title('Вхідне зображення X')
    plt.axis('off')
    plt.colorbar(fraction=0.046)

    plt.subplot(2, 5, 2)
    plt.imshow(Y, cmap='gray', vmin=0, vmax=1)
    plt.title('Очікуване зображення Y')
    plt.axis('off')
    plt.colorbar(fraction=0.046)

    # Результати методів
    plt.subplot(2, 5, 3)
    plt.imshow(Y_MP, cmap='gray', vmin=0, vmax=1)
    plt.title(f'Moore-Penrose\nRMSE: {rmse_MP:.4f}')
    plt.axis('off')
    plt.colorbar(fraction=0.046)

    plt.subplot(2, 5, 4)
    plt.imshow(Y_G, cmap='gray', vmin=0, vmax=1)
    plt.title(f'Greville\nRMSE: {rmse_G:.4f}')
    plt.axis('off')
    plt.colorbar(fraction=0.046)

    # Різниця між очікуваним і отриманим
    plt.subplot(2, 5, 5)
    diff_MP = np.abs(Y - Y_MP)
    plt.imshow(diff_MP, cmap='hot', vmin=0, vmax=diff_MP.max())
    plt.title(f'Різниця MP\nMax: {diff_MP.max():.4f}')
    plt.axis('off')
    plt.colorbar(fraction=0.046)

    # Порівняння метрик
    methods = ['Moore-Penrose', 'Greville']
    times = [time_MP, time_G]
    memories = [mem_MP, mem_G]
    rmses = [rmse_MP, rmse_G]
    operations = [ops_MP, ops_G]

    plt.subplot(2, 5, 6)
    bars = plt.bar(methods, times, color=['#3498db', '#2ecc71'])
    plt.title('Час виконання')
    plt.ylabel('Секунди')
    plt.grid(axis='y', alpha=0.3)
    for i, bar in enumerate(bars):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{times[i]:.3f}s',
                ha='center', va='bottom', fontsize=9)

    plt.subplot(2, 5, 7)
    bars = plt.bar(methods, memories, color=['#3498db', '#2ecc71'])
    plt.title('Використання пам\'яті')
    plt.ylabel('Мегабайти')
    plt.grid(axis='y', alpha=0.3)
    for i, bar in enumerate(bars):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{memories[i]:.1f}MB',
                ha='center', va='bottom', fontsize=9)

    plt.subplot(2, 5, 8)
    bars = plt.bar(methods, rmses, color=['#3498db', '#2ecc71'])
    plt.title('RMSE похибка')
    plt.ylabel('RMSE')
    plt.grid(axis='y', alpha=0.3)
    for i, bar in enumerate(bars):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{rmses[i]:.5f}',
                ha='center', va='bottom', fontsize=9)

    plt.subplot(2, 5, 9)
    bars = plt.bar(methods, operations, color=['#3498db', '#2ecc71'])
    plt.title('Кількість операцій')
    plt.ylabel('Операції')
    plt.grid(axis='y', alpha=0.3)
    for i, bar in enumerate(bars):
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{operations[i]:,}',
                ha='center', va='bottom', fontsize=8, rotation=0)

    plt.subplot(2, 5, 10)
    diff_G = np.abs(Y - Y_G)
    plt.imshow(diff_G, cmap='hot', vmin=0, vmax=diff_G.max())
    plt.title(f'Різниця Greville\nMax: {diff_G.max():.4f}')
    plt.axis('off')
    plt.colorbar(fraction=0.046)

    plt.tight_layout()
    plt.savefig('comparison_results.png', dpi=150, bbox_inches='tight')
    print("\nГрафік збережено як 'comparison_results.png'")
    plt.show()


if __name__ == "__main__":
    main()