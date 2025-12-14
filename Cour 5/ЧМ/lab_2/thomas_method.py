import numpy as np
from logger import logger

def format_row(row):
    return "[" + ", ".join(f"{x:9.5f}" for x in row) + "]"

def check_tridiagonal_stability(A):
    """Check tridiagonality and stability of the Thomas algorithm"""
    n = A.shape[0]
    is_tridiagonal = True
    logger.info("=== Checking tridiagonality ===")
    
    for i in range(n):
        for j in range(n):
            if abs(i-j) > 1 and A[i,j] != 0:
                logger.error(f"Non-zero element outside tridiagonal: A[{i},{j}] = {A[i,j]}")
                is_tridiagonal = False
    logger.info(f"Matrix is tridiagonal: {is_tridiagonal}")
    
    if not is_tridiagonal:
        logger.error("Thomas algorithm not applicable (matrix is not tridiagonal).")
        return False
    
    a = np.zeros(n)
    b = np.zeros(n)
    c = np.zeros(n)
    for i in range(n):
        c[i] = A[i,i]
        if i > 0: 
            a[i] = A[i,i-1]
        if i < n-1: 
            b[i] = A[i,i+1]
    
    all_ge = True
    exists_strict = False
    logger.info("=== Checking stability conditions ===")
    for i in range(n):
        left = abs(c[i])
        right = abs(a[i]) + abs(b[i])
        logger.info(f"i={i}: |c_i| = {left:.6f}, |a_i| + |b_i| = {right:.6f}")
        if left < right: 
            all_ge = False
        if left > right: 
            exists_strict = True
    
    if all_ge:
        logger.info("Condition |c_i| >= |a_i| + |b_i| holds for all i (possible borderline stability).")
    else:
        logger.warning("Condition |c_i| >= |a_i| + |b_i| does not hold for all i.")
        
    if exists_strict:
        logger.info("There exists i where |c_i| > |a_i| + |b_i|, Thomas algorithm is stable")
        return True
    else:
        logger.warning("No i found where |c_i| > |a_i| + |b_i|, method may be unstable.")
        return False

def thomas_algorithm_signed(A, f):
    """Thomas algorithm for solving a tridiagonal system"""
    n = len(f)
    a = np.zeros(n)
    b = np.zeros(n)
    c = np.zeros(n)
    
    for i in range(n):
        c[i] = -A[i,i]
        if i > 0: 
            a[i] = A[i,i-1]
        if i < n-1: 
            b[i] = A[i,i+1]
    
    logger.info("=== Forward sweep (transformation) ===")
    
    alpha = np.zeros(n)
    beta = np.zeros(n)
    alpha[0] = b[0]/c[0]
    beta[0] = -f[0]/c[0]
    logger.info(f"i=0: α_0 = {alpha[0]:.6f}, β_0 = {beta[0]:.6f}, z_0 = {c[0]:.6f}")
    
    for i in range(1, n):
        z = c[i] - a[i]*alpha[i-1]
        if abs(z) < 1e-15:
            logger.error(f"Zero denominator at step i={i}")
            return None
        alpha[i] = b[i]/z if i < n-1 else 0
        beta[i] = (-f[i] + a[i]*beta[i-1])/z
        logger.info(f"i={i}: α_{i} = {alpha[i]:.6f}, β_{i} = {beta[i]:.6f}, z_{i} = {z:.6f}")
    
    logger.info("=== Backward substitution (solving) ===")
    
    y = np.zeros(n)
    y[-1] = beta[-1]
    logger.info(f"y_{n-1} = {y[-1]:.6f}")
    
    for i in range(n-2, -1, -1):
        y[i] = alpha[i]*y[i+1] + beta[i]
        logger.info(f"y_{i} = α_{i}*y_{i+1} + β_{i} = {y[i]:.6f}")
    
    return y

def run_example():
    """Run Thomas algorithm example"""
    logger.info("=" * 60)
    logger.info("RUN: Thomas algorithm (tridiagonal method)")
    logger.info("=" * 60)
    
    logger.info("\n===== TEST 1 =====")
    A1 = np.array([[1, 2, 0],
                   [2, 2, 3],
                   [0, 3, 2]], dtype=float)
    b1 = np.array([5, 15, 12], dtype=float)
    
    logger.info(f"Matrix A:\n{A1}")
    logger.info(f"Vector b: {format_row(b1)}")
    
    is_stable_1 = check_tridiagonal_stability(A1)
    if is_stable_1:
        y1 = thomas_algorithm_signed(A1, b1)
        if y1 is not None:
            logger.info(f"Solution y1: {format_row(y1)}")
        else:
            logger.error("Failed to solve system 1")
    
    logger.info("\n===== TEST 2 =====")
    A2 = np.array([[1, 1, 0],
                   [1, 3, 2],
                   [0, 1, 2]], dtype=float)
    b2 = np.array([1, 1, 1], dtype=float)
    
    logger.info(f"Matrix A:\n{A2}")
    logger.info(f"Vector b: {format_row(b2)}")
    
    is_stable_2 = check_tridiagonal_stability(A2)
    if is_stable_2:
        y2 = thomas_algorithm_signed(A2, b2)
        if y2 is not None:
            logger.info(f"Solution y2: {format_row(y2)}")
        else:
            logger.error("Failed to solve system 2")
    
    logger.info("=" * 60)
