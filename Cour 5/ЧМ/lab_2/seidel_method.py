import numpy as np
from logger import logger  # your UTF-8 logger

def is_symmetric_positive_definite(A):
    if not np.allclose(A, A.T):
        logger.warning("Matrix is not symmetric: A != A^T")
        return False
    
    n = A.shape[0]
    for k in range(1, n+1):
        minor = A[:k, :k]
        det_minor = np.linalg.det(minor)
        if det_minor <= 0:
            logger.warning(f"Leading principal minor {k}x{k} has det={det_minor:.6f} <= 0")
            return False
    logger.info("Matrix is symmetric and positive definite: Gauss-Seidel method converges")
    return True

def has_strict_diagonal_dominance(A):
    n = A.shape[0]
    for i in range(n):
        sum_off_diag = sum(abs(A[i,j]) for j in range(n) if j != i)
        if abs(A[i,i]) < sum_off_diag:
            logger.warning(f"Row {i}: |a_ii| = {abs(A[i,i])} < Σ|a_ij| = {sum_off_diag}")
            return False
    logger.info("Matrix has strict diagonal dominance: Gauss-Seidel method converges")
    return True

def check_spectral_radius(A):
    n = A.shape[0]
    D = np.diag(np.diag(A))
    L = np.tril(A, -1)
    U = np.triu(A, 1)
    
    G = -np.linalg.inv(D + L) @ U
    eigenvalues = np.linalg.eigvals(G)
    spectral_radius = max(abs(eigenvalues))
    
    logger.info(f"Eigenvalues of the iteration matrix G: {eigenvalues}")
    logger.info(f"Spectral radius: {spectral_radius:.6f}")
    
    if spectral_radius < 1:
        logger.info("Necessary and sufficient convergence condition satisfied: method converges for any x0")
        return True
    else:
        logger.warning("Condition not satisfied: method may not converge for some x0")
        return False

def gauss_seidel(A, b, x0=None, eps=1e-3, max_iter=100):
    n = len(b)
    x = np.zeros(n) if x0 is None else x0.copy()
    
    logger.info("=== Starting Gauss-Seidel method ===")
    logger.info(f"Initial guess: x0 = {x}")
    
    for k in range(1, max_iter+1):
        x_old = x.copy()
        logger.info(f"Iteration {k}:")
        
        for i in range(n):
            sum1 = sum(A[i,j]/A[i,i] * x[j] for j in range(i))         # updated x[j]
            sum2 = sum(A[i,j]/A[i,i] * x_old[j] for j in range(i+1,n))  # old x[j]
            x[i] = (b[i]/A[i,i]) - sum1 - sum2
            logger.info(f"x_{i}^{k} = {x[i]:.6f}")
        
        norm_diff = np.linalg.norm(x - x_old, ord=np.inf)
        logger.info(f"||x^{k} - x^{k-1}||_∞ = {norm_diff:.6f}")
        
        if norm_diff <= eps:
            logger.info(f"Stopping: target accuracy ε = {eps} reached")
            break
    else:
        logger.warning("Reached max_iter without convergence")
    
    logger.info(f"Solution x = {x}")
    return x

def run_gauss_seidel_example():
    A = np.array([
        [4, 0, 1, 0],
        [0, 3, 0, 2],
        [1, 0, 5, 1],
        [0, 2, 1, 4]
    ], dtype=float)
    b = np.array([7, 14, 20, 23], dtype=float)
    
    cond1 = has_strict_diagonal_dominance(A)
    cond2 = is_symmetric_positive_definite(A)
    cond3 = check_spectral_radius(A)  
    
    if cond1 or cond2 or cond3:
        x0 = np.zeros(len(b))
        x = gauss_seidel(A, b, x0=x0, eps=0.001)
        logger.info(f"Approximate solution of the system: {x}")
    else:
        logger.warning("None of the sufficient convergence conditions are satisfied")

if __name__ == "__main__":
    run_gauss_seidel_example()
