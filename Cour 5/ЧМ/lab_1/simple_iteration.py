import numpy as np
from logger import setup_logger, log_iterations, log_convergence_check, log_result
from utils import find_root_interval, plot_functions, print_iterations

logger = setup_logger("simple_iteration")

def f(x):
    """Target function: f(x) = x³ + 3x² - x - 3"""
    return x**3 + 3*x**2 - x - 3

def phi(x):
    """Iteration function: φ(x) = √((-x³ + x + 3) / 3)"""
    return np.sqrt((-x**3 + x + 3) / 3)

def dphi(x):
    """Derivative of iteration function"""
    return (1 - 3*x**2) / (2 * np.sqrt(3) * np.sqrt(3 + x - x**3))

def plot_simple_iteration_functions(x_min=-2, x_max=5):
    """Plot f(x) and φ(x) for simple iteration method"""
    plot_functions(f,phi, dphi, x_min=x_min, x_max=x_max, title="Simple Iteration: f(x)")

def check_convergence(phi, dphi, a, b, x0):
    """Check convergence conditions for simple iteration method"""
    logger.info(f"Checking convergence conditions for interval [{a}, {b}], x0 = {x0}")
    
    delta = max(abs(x0 - a), abs(b - x0))
    xs = np.linspace(a, b, 500)
    q = np.max(np.abs(dphi(xs)))
    converges = q < 1
    
    logger.info(f"Maximum |φ'(x)| = {q:.6f}")
    logger.info(f"Delta = {delta:.6f}")
    
    log_convergence_check(logger, "Simple Iteration", converges, q)
    return converges, q, delta

def simple_iteration(a, b, eps):
    logger.info(f"Starting simple iteration method with interval [{a}, {b}], eps = {eps}")
    
    has_root, a, b = find_root_interval(f, a, b)
    if not has_root:
        logger.error("No root found in the given interval")
        return None

    plot_simple_iteration_functions()

    x0 = (a + b) / 2
    logger.info(f"Initial approximation: x0 = {x0}")
    
    converges = check_convergence(phi, dphi, a, b, x0)
    if not converges:
        logger.error("Convergence conditions are NOT satisfied")
        return None

    iterations = []
    x = x0
    max_iterations = 100
    
    iterations.append((0, x, f(x)))
    
    for k in range(1, max_iterations + 1):
        x_next = phi(x)
        f_val = f(x_next)
        iterations.append((k, x_next, f_val))
        
        if abs(x_next - x) < eps:
            logger.info(f"Convergence achieved after {k} iterations")
            print_iterations(iterations, type='iteration')
            log_result(logger, "Simple Iteration", x_next, len(iterations))
            return x_next
            
        x = x_next
    else:
        logger.warning(f"Maximum iterations ({max_iterations}) reached without convergence")
        log_iterations(logger, iterations, "Simple Iteration")
    
    return None
