from logger import setup_logger, log_iterations, log_convergence_check, log_result
from utils import find_root_interval, plot_functions, print_iterations
# Set up logger
logger = setup_logger("newton_method")

def f(x):
    """Target function: f(x) = x³ - 6x² + 5x + 12"""
    return x**3 - 6*x**2 + 5*x + 12

def df(x):
    """First derivative: f'(x) = 3x² - 12x + 5"""
    return 3*x**2 - 12*x + 5

def ddf(x):
    """Second derivative: f''(x) = 6x - 12"""
    return 6*x - 12

def plot_newton_functions(x_min=-2, x_max=5):
    """Plot f(x), f'(x), and f''(x) for Newton's method"""
    plot_functions(f, df, ddf, x_min, x_max, "Newton's Method: f(x), f'(x), f''(x)")

def check_convergence(x0, x_star, a, b):
    """Check Newton's method convergence conditions"""
    logger.info(f"Checking convergence conditions for x0 = {x0}")
    
    f_val = f(x0)
    f2_val = ddf(x0)
    M2 = max(abs(ddf(a)), abs(ddf(b)))
    m1 = min(abs(df(a)), abs(df(b)))
    q = M2 * abs(x0 - x_star) / (2 * m1)
    condition_sign = f_val * f2_val > 0
    condition_q = q < 1

    logger.info(f"f(x0) = {f_val:.6f}, f''(x0) = {f2_val:.6f}")
    logger.info(f"M2 = {M2:.6f}, m1 = {m1:.6f}")
        
    if condition_sign and condition_q:
             logger.info(f"✓ Newton's Method: Convergence conditions satisfied "
                    f"(sign and q < 1, q = {q:.6f})")
             return True, q
    else:
        if not condition_sign:
            logger.error("✗ Condition f(x0)*f''(x0) > 0 is NOT satisfied — wrong direction of iteration.")
        if not condition_q:
            logger.error(f"✗ Theoretical convergence condition q < 1 is NOT satisfied (q = {q:.6f}).")
        logger.error("⛔ Newton's method terminated — convergence conditions failed.")
        exit(1)   # ← завершує програму повністю

    log_convergence_check(logger, "Newton's Method", condition, q)
    return condition, q

def newton_method(a, b, eps, x_star):    
    logger.info(f"Starting Newton's method with interval [{a}, {b}], x* = {x_star}, eps = {eps}")
    
    has_root, a, b = find_root_interval(f, a, b)
    if not has_root:
        logger.error("No root found in the given interval")
        return None
    
    plot_newton_functions()
    
    x0 = (a + b) / 2
    logger.info(f"Initial approximation: x0 = {x0}")

    converges, q = check_convergence(x0, x_star, a, b)
    if not converges:
        logger.error("Convergence conditions are NOT satisfied")
        return None

    iterations = []
    x = x0
    max_iterations = 100
    
    for n in range(max_iterations):
        fx = f(x)
        dfx = df(x)
        
        if dfx == 0:
            logger.error("Derivative equals zero! Method cannot continue.")
            break
            
        x_new = x - fx / dfx
        iterations.append((n, x, fx))
        
        if abs(x_new - x) <= eps:
            iterations.append((n+1, x_new, f(x_new)))
            logger.info(f"Convergence achieved after {n+1} iterations")
            break
            
        x = x_new
    else:
        logger.warning(f"Maximum iterations ({max_iterations}) reached without convergence")
    
    print_iterations(iterations)
    log_result(logger, "Newton's Method", x_new, len(iterations))
    
    return x_new, iterations, q
