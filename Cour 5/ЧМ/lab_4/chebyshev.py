import math
import numpy as np

def get_chebyshev_nodes(a: float, b: float, num_nodes: int) -> np.ndarray:
    print("\n=== КРОК 1: Обчислення вузлів Чебишева ===")
    nodes = []
    for k in range(num_nodes):
        t_k = math.cos((2 * k + 1) * math.pi / (2 * num_nodes))
        x_k = (a + b) / 2 + (b - a) / 2 * t_k
        print(f"k={k}: t_k={t_k:.6f},  x_k={x_k:.6f}")
        nodes.append(x_k)
    nodes = np.sort(np.array(nodes))
    print("\nВідсортовані вузли:", nodes, "\n")
    return nodes