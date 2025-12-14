from logger import logger
from gauss_method import run_example as run_gauss
from thomas_method import run_example as run_thomas
from seidel_method import run_gauss_seidel_example as run_gauss_seidel

def display_menu():
    """Display the numerical method selection menu"""
    print("\n" + "="*60)
    print("NUMERICAL METHODS FOR SOLVING LINEAR SYSTEMS")
    print("="*60)
    print("1. Gaussian elimination with partial pivoting")
    print("2. Thomas algorithm (tridiagonal method)")
    print("3. Gauss-Seidel iterative method")
    print("0. Exit")
    print("="*60)

def main():
    """Main function with method selection menu"""
    logger.info("Program started")
    
    while True:
        display_menu()
        choice = input("\nSelect a method (0-3): ").strip()
        
        if choice == "1":
            logger.info("User selected: Gaussian elimination")
            try:
                run_gauss()
                logger.info("Gaussian elimination completed successfully")
            except Exception as e:
                logger.error(f"Error during Gaussian elimination: {e}")
        
        elif choice == "2":
            logger.info("User selected: Thomas algorithm")
            try:
                run_thomas()
                logger.info("Thomas algorithm completed successfully")
            except Exception as e:
                logger.error(f"Error during Thomas algorithm: {e}")
        
        elif choice == "3":
            logger.info("User selected: Gauss-Seidel method")
            try:
                run_gauss_seidel()
                logger.info("Gauss-Seidel method completed successfully")
            except Exception as e:
                logger.error(f"Error during Gauss-Seidel method: {e}")
        
        elif choice == "0":
            logger.info("User exited the program")
            print("\nGoodbye!")
            break
        
        else:
            logger.warning(f"Invalid choice: {choice}")
            print("Error: Invalid choice. Please try again.")
    
    logger.info("Program finished")

if __name__ == "__main__":
    main()
