"""
Script para rodar o executável dos que testa a paginação.
O script recebe um argumento, sendo ele o caminho para o executável.
É importante que esse programa retorne o tempo médio de cada execução,
de forma que o script possa coletar esses dados e armazená-los em um arquivo CSV, com 3 colunas: número de páginas, número de iterações e o tempo médio de cada acesso
"""
import subprocess
import sys

output_file = "output.csv"
n_iterations = 10_000
max_pages = 5_000
stride = 50

if len(sys.argv) != 2:
    print("usage: python script.py <path_to_executable>")
    sys.exit(1)

path = sys.argv[1]

with open(output_file, "w") as file:
    file.write("n_pages,n_iterations,time\n")
    for n_pages in range(1, max_pages + 1, stride):
        result = subprocess.run(
            [path, str(n_pages), str(n_iterations)],
            capture_output=True,
            text=True
        )
        avg_time = result.stdout.strip()
        file.write(f"{n_pages},{n_iterations},{avg_time}\n")
        print("Finished with n_pages:", n_pages)


