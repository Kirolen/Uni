class Logger:
    COLORS = {
        'HEADER': '\033[95m',
        'BLUE': '\033[94m',
        'CYAN': '\033[96m',
        'GREEN': '\033[92m',
        'YELLOW': '\033[93m',
        'RED': '\033[91m',
        'BOLD': '\033[1m',
        'UNDERLINE': '\033[4m',
        'END': '\033[0m'
    }

    @staticmethod
    def print_header(text):
        width = 80
        print("\n" + "=" * width)
        print(f"{Logger.COLORS['BOLD']}{Logger.COLORS['CYAN']}{text.center(width)}{Logger.COLORS['END']}")
        print("=" * width + "\n")

    @staticmethod
    def print_section(text):
        print(f"\n{Logger.COLORS['BOLD']}{Logger.COLORS['YELLOW']}{'─' * 60}{Logger.COLORS['END']}")
        print(f"{Logger.COLORS['BOLD']}{Logger.COLORS['YELLOW']}{text}{Logger.COLORS['END']}")
        print(f"{Logger.COLORS['YELLOW']}{'─' * 60}{Logger.COLORS['END']}\n")

    @staticmethod
    def print_subsection(text):
        print(f"\n{Logger.COLORS['CYAN']}{Logger.COLORS['BOLD']}▸ {text}{Logger.COLORS['END']}")

    @staticmethod
    def print_step(step_num, total_steps, description):
        print(f"{Logger.COLORS['GREEN']}[Крок {step_num}/{total_steps}]{Logger.COLORS['END']} {description}")

    @staticmethod
    def print_calculation(formula, result=None):
        if result is not None:
            print(f"  {Logger.COLORS['BLUE']}{formula}{Logger.COLORS['END']} = {Logger.COLORS['GREEN']}{result}{Logger.COLORS['END']}")
        else:
            print(f"  {Logger.COLORS['BLUE']}{formula}{Logger.COLORS['END']}")

    @staticmethod
    def print_result(label, value):
        print(f"  {Logger.COLORS['BOLD']}{label}:{Logger.COLORS['END']} {Logger.COLORS['GREEN']}{value}{Logger.COLORS['END']}")

    @staticmethod
    def print_table_header(headers):
        header_line = " | ".join(f"{h:^12}" for h in headers)
        separator = "─" * len(header_line)
        print(f"\n{Logger.COLORS['BOLD']}{header_line}{Logger.COLORS['END']}")
        print(separator)

    @staticmethod
    def print_table_row(values):
        row = " | ".join(f"{str(v):^12}" for v in values)
        print(row)

    @staticmethod
    def print_polynomial(interval, polynomial_str):
        print(f"\n  {Logger.COLORS['CYAN']}Інтервал {interval}:{Logger.COLORS['END']}")
        print(f"  {Logger.COLORS['BOLD']}{polynomial_str}{Logger.COLORS['END']}")

    @staticmethod
    def print_info(text):
        print(f"{Logger.COLORS['BLUE']}ℹ{Logger.COLORS['END']} {text}")

    @staticmethod
    def print_success(text):
        print(f"{Logger.COLORS['GREEN']}✓{Logger.COLORS['END']} {text}")

    @staticmethod
    def print_array(name, array):
        values = ", ".join(f"{v:.6f}" if isinstance(v, float) else str(v) for v in array)
        print(f"  {Logger.COLORS['BOLD']}{name}:{Logger.COLORS['END']} [{values}]")