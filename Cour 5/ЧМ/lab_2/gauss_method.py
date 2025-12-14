from utils import copy_matrix, copy_vector
from logger import logger

def format_row(row):
    return "[" + ", ".join(f"{x:9.5f}" for x in row) + "]"

def print_augmented(A, b, A_inv=None):
    n = len(A)
    rows = []
    for i in range(n):
        if A_inv is not None:
            inv_part = "  ||  " + format_row(A_inv[i])
        else:
            inv_part = ""
        rows.append(format_row(A[i]) + "  |  " + f"{b[i]:9.5f}" + inv_part)
    return "\n".join(rows)

def forward_elimination(A_in, b_in, A_inv_in=None, verbose=True):
    A = copy_matrix(A_in)
    b = copy_vector(b_in)
    n = len(A)
    
    if A_inv_in is None:
        A_inv = [[1.0 if i == j else 0.0 for j in range(n)] for i in range(n)]
    else:
        A_inv = copy_matrix(A_inv_in)
    
    det = 1.0
    sign = 1  
    
    if verbose:
        logger.info("Initial augmented matrix (A|b||I):")
        logger.info("\n" + print_augmented(A, b, A_inv))
    
    for k in range(n-1):
        # Partial pivoting: choose the largest absolute value in column k
        pivot_row = max(range(k, n), key=lambda i: abs(A[i][k]))
        pivot_val = A[pivot_row][k]
        
        logger.info(f"Step {k+1}: working column = {k}, looking for max |element| in rows {k}..{n-1}")
        logger.info(f"Largest |element| in column {k} is in row {pivot_row} (value = {pivot_val:.6f})")
        
        # Swap rows if needed
        if pivot_row != k:
            A[k], A[pivot_row] = A[pivot_row], A[k]
            b[k], b[pivot_row] = b[pivot_row], b[k]
            A_inv[k], A_inv[pivot_row] = A_inv[pivot_row], A_inv[k]
            sign *= -1  
            logger.info(f"Swapping rows {k} and {pivot_row}. After swap:")
            logger.info("\n" + print_augmented(A, b, A_inv))
        else:
            logger.info("No swap needed (largest element already in position).")
        
        # Check for zero pivot
        if abs(A[k][k]) < 1e-15:
            logger.error(f"Pivot A[{k}][{k}] ≈ 0 (={A[k][k]}). Cannot proceed with standard elimination.")
            return A, b, A_inv, None  
        
        det *= A[k][k]
        
        # Eliminate rows below
        for i in range(k+1, n):
            if abs(A[i][k]) < 1e-15:
                multiplier = 0.0
            else:
                multiplier = A[i][k] / A[k][k]
            logger.info(f"For row {i}: multiplier = A[{i}][{k}] / A[{k}][{k}] = {A[i][k]:.6f} / {A[k][k]:.6f} = {multiplier:.6f}")
            
            for j in range(k, n):
                A[i][j] -= multiplier * A[k][j]
                if abs(A[i][j]) < 1e-12:
                    A[i][j] = 0.0
            b[i] -= multiplier * b[k]
            if abs(b[i]) < 1e-12:
                b[i] = 0.0
            
            for j in range(n):
                A_inv[i][j] -= multiplier * A_inv[k][j]
                if abs(A_inv[i][j]) < 1e-12:
                    A_inv[i][j] = 0.0
            
            logger.info(f"After subtracting {multiplier:.6f}*row({k}) from row {i}:")
            logger.info("\n" + print_augmented(A, b, A_inv))
        
        logger.info(f"End of step {k+1}. Matrix after elimination in column {k}:")
        logger.info("\n" + print_augmented(A, b, A_inv))
    
    if abs(A[n-1][n-1]) < 1e-15:
        logger.error(f"A[{n-1}][{n-1}] ≈ 0. Matrix is singular.")
        det = 0.0
    else:
        det *= A[n-1][n-1]
    
    det *= sign 
    
    logger.info("Forward elimination complete. Upper-triangular form (augmented A|b||I'):")
    logger.info("\n" + print_augmented(A, b, A_inv))
    logger.info(f"Determinant (product of diagonals × sign): det = {det:.10f}")
    
    return A, b, A_inv, det

def back_substitution(A, b, A_inv, verbose=True):
    n = len(A)
    x = [0.0] * n
    
    if verbose:
        logger.info("Back substitution (solving system and computing inverse):")
    
    # Solve the system x
    for i in range(n-1, -1, -1):
        if abs(A[i][i]) < 1e-15:
            logger.error(f"A[{i}][{i}] ≈ 0, cannot compute x[{i}]")
            x[i] = float('nan')
            continue
        
        sum_ax = sum(A[i][j] * x[j] for j in range(i+1, n))
        x[i] = (b[i] - sum_ax) / A[i][i]
        
        if verbose:
            logger.info(f"x[{i}] = (b[{i}] - Σ) / A[{i}][{i}] = ({b[i]:.5f} - {sum_ax:.5f}) / {A[i][i]:.5f} = {x[i]:.10f}")
    
    logger.info(f"Solution x: {format_row(x)}")
    
    # Compute inverse matrix
    if verbose:
        logger.info("Computing inverse matrix (back substitution for each column):")
    
    for col in range(n):
        for i in range(n-1, -1, -1):
            if abs(A[i][i]) < 1e-15:
                A_inv[i][col] = float('nan')
                continue
            
            sum_val = sum(A[i][j] * A_inv[j][col] for j in range(i+1, n))
            A_inv[i][col] = (A_inv[i][col] - sum_val) / A[i][i]
            
            if abs(A_inv[i][col]) < 1e-12:
                A_inv[i][col] = 0.0
    
    if verbose:
        logger.info("Inverse matrix A⁻¹:")
        for row in A_inv:
            logger.info(format_row(row))
    
    return x, A_inv

def verify_inverse(A_original, A_inv):
    n = len(A_original)
    product = [[0.0 for _ in range(n)] for _ in range(n)]
    
    for i in range(n):
        for j in range(n):
            product[i][j] = sum(A_original[i][k] * A_inv[k][j] for k in range(n))
            if abs(product[i][j]) < 1e-10:
                product[i][j] = 0.0
    
    logger.info("Check: A × A⁻¹ =")
    for row in product:
        logger.info(format_row(row))

def run_example():
    logger.info("=" * 60)
    logger.info("RUN: Gaussian elimination with partial pivoting")
    logger.info("=" * 60)
    
    A = [
        [4.0, 3.0, 1.0, 0.0],
        [-2.0, 2.0, 6.0, 1.0],
        [0.0, 5.0, 2.0, 3.0],
        [0.0, 1.0, 2.0, 7.0],
    ]
    b = [14.0, 31.0, 33.0, 45.0]
    
    # Save original matrix for verification
    A_original = copy_matrix(A)
    
    # Forward elimination
    A_upper, b_upper, A_inv_partial, det = forward_elimination(A, b)
    
    if det is None or abs(det) < 1e-15:
        logger.error("Matrix is singular, inverse does not exist.")
        return 1
    
    # Back substitution
    x, A_inv = back_substitution(A_upper, b_upper, A_inv_partial)
    
    # Verify inverse
    verify_inverse(A_original, A_inv)
    
    logger.info(f"SUMMARY:")
    logger.info(f"Determinant det(A) = {det:.10f}")
    logger.info(f"Solution x = {format_row(x)}")
    logger.info("=" * 60)
    
    return 0
