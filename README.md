# tp-2022-1c-ElProtocoloMesi
Personal Access Token (1ra entrega): `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`

## Instrucciones para la entrega
1. Crear el directorio `~/workspace` y moverse adentro con `cd`. En este directorio se van a instalar el TP y las commons.
2. Estando en `~/workspace` clonar el repositorio del TP, usando como username `echisaidman` y como password el PAT que esta en el README (u otro username y PAT).
3. Moverse adentro del repositorio con `cd` y ejecutar `./deploy.sh` para instalar las commons y compilar el TP.
4. Ejecutar el script para cambiar las IPs de cada modulo, haciendo `python3 ./helpers/cambiar_ips.py <modulo_a_cambiar> <nueva_ip>`.
    * El `modulo_a_cambiar` tiene que ser **kernel**, **cpu** o **memoria**.
5. Hacer `cd` al modulo que corresponda (cpu, memoria, ...) y ejecutar `./exec_<prueba>` para cada prueba.
