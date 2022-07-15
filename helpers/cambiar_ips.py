from os import listdir
from os.path import dirname, isfile, join
import sys


def cambiar_ip_de_modulo_en_modulo(ruta_modulo: str, modulo_a_cambiar: str, nueva_ip: str):
    ruta_configs = join(ruta_modulo, "cfg")
    nombres_archivos = [f for f in listdir(
        ruta_configs) if isfile(join(ruta_configs, f))]
    for nombre_archivo in nombres_archivos:
        with open(f"{ruta_configs}/{nombre_archivo}", "r") as f:
            lineas = f.readlines()
        with open(f"{ruta_configs}/{nombre_archivo}", "w") as f:
            for linea in lineas:
                modulo_upper = modulo_a_cambiar.upper()
                if f"IP_{modulo_upper}" in linea:
                    f.write(f"IP_{modulo_upper}={nueva_ip}\n")
                else:
                    f.write(linea)


def main() -> int:
    modulos = ["consola", "cpu", "kernel", "memoria"]
    modulo_a_cambiar = sys.argv[1]
    nueva_ip = sys.argv[2]
    ruta_workspace = join(dirname(__file__), "..")
    for modulo in modulos:
        ruta_modulo = join(ruta_workspace, modulo)
        cambiar_ip_de_modulo_en_modulo(ruta_modulo, modulo_a_cambiar, nueva_ip)
    return 0


if __name__ == "__main__":
    main()
