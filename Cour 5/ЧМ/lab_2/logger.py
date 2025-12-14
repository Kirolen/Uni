import logging

log_format = '%(asctime)s - %(filename)s - %(levelname)s - %(message)s'
date_format = '%Y-%m-%d %H:%M:%S'

def setup_logger(name="app_logger", level=logging.INFO):
    logger = logging.getLogger(name)
    
    if logger.handlers:
        for handler in logger.handlers:
            logger.removeHandler(handler)
    
    logger.setLevel(level)
    
    console_handler = logging.StreamHandler()
    console_handler.setLevel(level)
    console_formatter = logging.Formatter(log_format, datefmt=date_format)
    console_handler.setFormatter(console_formatter)
    
    console_handler.stream = open(1, "w", encoding="utf-8", closefd=False)
    
    logger.addHandler(console_handler)
    
    return logger

logger = setup_logger()
