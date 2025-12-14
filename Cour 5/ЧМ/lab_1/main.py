from newton import newton_method
from simple_iteration import simple_iteration
from utils import  validate_input
from logger import setup_logger

# Set up main logger
logger = setup_logger("main")

def main():
    """Main function to run numerical methods"""
    logger.info("Starting numerical methods application")
    
    print("=== Choose Method ===")
    print("1. Newton's Method")
    print("2. Simple Iteration Method")
    
    while True:
        choice = input("Your choice (1 or 2): ").strip()
        if choice in ["1", "2"]:
            break
        print("Invalid choice! Please enter 1 or 2.")

    if choice == "1":
        logger.info("Newton's method selected")
        
        while True:
            eps_input = input("Enter tolerance epsilon: ").strip()
            if validate_input(eps_input, "float") and float(eps_input) > 0:
                eps = float(eps_input)
                break
            print("Invalid input! Please enter a positive number.")
        
        a, b = -2.5, -0.5
        x_star = -1
        
        root, iters, q = newton_method(a, b, eps, x_star)
        
        print(f"\n=== Newton's Method Results ===")
        print(f"Root found: x ≈ {root:.8f}")
        print(f"Convergence rate: q ≈ {q:.6f}")
        print(f"Total iterations: {len(iters)}")

    elif choice == "2":
        logger.info("Simple iteration method selected")
        
        a, b = 0.75, 1.3
        
        while True:
            eps_input = input("Enter tolerance epsilon: ").strip()
            if validate_input(eps_input, "float") and float(eps_input) > 0:
                eps = float(eps_input)
                break
            print("Invalid input! Please enter a positive number.")
        
        logger.info(f"Parameters: interval=[{a}, {b}], eps={eps}")
        
        root = simple_iteration(a, b, eps)
        
        if root:
            print(f"\n=== Simple Iteration Results ===")
            print(f"Root found: x ≈ {root:.8f}")
        else:
            print("Method did not converge.")
    
    logger.info("Application finished")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n Program interrupted by user")
        logger.info("Program interrupted by user")
    except Exception as e:
        print(f"\n An error occurred: {e}")
        logger.error(f"Unexpected error: {e}")
