import logging
from typing import List, Tuple

def setup_logger(name: str = "numerical_methods") -> logging.Logger:
    """Set up logger with consistent formatting"""
    logger = logging.getLogger(name)
    logger.setLevel(logging.INFO)
    
    # Remove existing handlers to avoid duplicates
    for handler in logger.handlers[:]:
        logger.removeHandler(handler)
    
    # Create console handler
    handler = logging.StreamHandler()
    formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
        datefmt='%H:%M:%S'
    )
    handler.setFormatter(formatter)
    logger.addHandler(handler)
    
    return logger

def log_iterations(logger: logging.Logger, iterations: List[Tuple], method_name: str):
    """Log iteration results in a formatted table"""
    logger.info(f"=== {method_name} Iterations ===")
    logger.info(f"{'Step':<6}{'x_n':<14}{'f(x_n)':<14}")
    logger.info("-" * 34)
    
    for step, x_val, f_val in iterations:
        logger.info(f"{step:<6}{x_val:<14.8f}{f_val:<14.8f}")

def log_convergence_check(logger: logging.Logger, method_name: str, converges: bool, q: float = None):
    """Log convergence condition results"""
    if converges:
        logger.info(f"✓ {method_name}: Convergence conditions satisfied")
        if q is not None:
            logger.info(f"  Convergence rate q ≈ {q:.6f}")
    else:
        logger.warning(f"⚠ {method_name}: Convergence conditions NOT satisfied")
        if q is not None:
            logger.warning(f"  Convergence rate q ≈ {q:.6f}")

def log_result(logger: logging.Logger, method_name: str, root: float, iterations_count: int):
    """Log final result"""
    logger.info(f"=== {method_name} Result ===")
    logger.info(f"Root found: x ≈ {root:.8f}")
    logger.info(f"Total iterations: {iterations_count}")