import pandas as pd

def f(x):
    return x**6 + 2*x**5 + 3*x**4 - 3

def compute_table(nodes):
    print("\n=== КРОК 2: Значення функції у вузлах ===")
    values = [f(x) for x in nodes]
    df = pd.DataFrame({"x_k": nodes, "f(x_k)": values})
    print(df.to_string(index=False))
    print()
    return values