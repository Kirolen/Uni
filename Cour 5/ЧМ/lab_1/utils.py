import numpy as np
import matplotlib.pyplot as plt
from logger import setup_logger

# Set up logger for utils
logger = setup_logger("utils")

def print_iterations(iterations, type = 'newton'):
    """Print iterations in a formatted table (legacy function)"""
    if (type == 'newton'):
        print(f"{'Step':<6}{'x_n':<14}{'f(x_n)':<14}")
        print("-" * 34)
        for i, x, fx in iterations:
            print(f"{i:<6}{x:<14.8f}{fx:<14.8f}")
    elif type == 'iteration':
        print(f"{'Iter':<6}{'x_n':<14}{'x_n+1':<14}{'f(x_n+1)':<14}")
        print("-" * 50)
        for i in range(len(iterations) - 1):
            x_n = float(iterations[i][1])
            x_next = float(iterations[i + 1][1])
            fx_next = float(iterations[i + 1][2])
            print(f"{i:<6}{x_n:<14.8f}{x_next:<14.8f}{fx_next:<14.8f}")


def format_number(num: float, precision: int = 6) -> str:
    """Format number with consistent precision"""
    return f"{num:.{precision}f}"

def validate_input(value: str, input_type: str = "float") -> bool:
    """Validate user input"""
    try:
        if input_type == "float":
            float(value)
        elif input_type == "int":
            int(value)
        return True
    except ValueError:
        return False

def find_root_interval(f, a, b, tol=1e-2):
    """Find interval containing a root using bisection approach"""
    logger.info(f"Searching for root interval in [{a}, {b}]")
    
    f_a, f_b = f(a), f(b)
    logger.info(f"f({a}) = {f_a:.6f}, f({b}) = {f_b:.6f}")
    
    if f_a * f_b < 0:
        logger.info(f"Root interval found: [{a}, {b}]")
        return True, a, b
        
    if abs(b - a) < tol:
        logger.warning(f"No root found in interval [{a}, {b}]")
        return False, None, None
        
    mid = (a + b) / 2
    right = find_root_interval(f, mid, b, tol)
    if right[0]:
        return right
    return find_root_interval(f, a, mid, tol)

def plot_functions(f, df=None, ddf=None, x_min=-2, x_max=5, title="Function Plot"):
    """Plot function and its derivatives"""
    logger.info(f"Plotting functions with title: {title}")
    
    x = np.linspace(x_min, x_max, 400)
    y = f(x)

    plt.figure(figsize=(10, 6))
    plt.plot(x, y, label="f(x)", color="blue", linewidth=2)
    
    if df is not None:
        y1 = df(x)
        plt.plot(x, y1, label="φ(x)", color="green", linestyle="--")
    
    if ddf is not None:
        y2 = ddf(x)
        plt.plot(x, y2, label="φ'(x)", color="red", linestyle="-.")
    
    plt.axhline(0, color="black", linewidth=0.8)
    plt.grid(True)
    plt.legend()
    plt.title(title)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.show()
    
    logger.info("Function plot displayed successfully")
