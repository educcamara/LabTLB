"""
Script para plotar um arquivo CSV com o número de páginas e o tempo médio de acesso para cada
"""
import matplotlib.pyplot as plt
import pandas as pd
import sys

if len(sys.argv) != 2:
    print("usage: python plot.py <path_to_csv>")
    sys.exit(1)

path = sys.argv[1]
csv = pd.read_csv(path)

n_pages = csv["n_pages"]
agv_times = csv["time"]


# Plotting the graph
plt.plot(n_pages, agv_times)
plt.xlabel("Número de Páginas")
plt.ylabel("Tempo médio de acesso (s)")

plt.show()