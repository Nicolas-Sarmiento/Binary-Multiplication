def to_binary(num, bits):
    """
    Convierte un número entero (positivo o negativo) a su representación
    binaria en complemento a dos con un número fijo de bits.
    """
    if num < 0:
        num = (1 << bits) + num  
    return format(num, '0{}b'.format(bits))

def from_binary(bin_str):
    """
    Convierte un binario en complemento a dos a su valor entero.
    """
    bits = len(bin_str)
    num = int(bin_str, 2)
    if bin_str[0] == '1':  
        num -= (1 << bits)
    return num

def booth(multiplicando, multiplicador, bits=8):
    """
    Algoritmo de Booth para multiplicar dos enteros (positivos o negativos).
    Retorna el producto en decimal y binario.
    """

    A = to_binary(multiplicando, bits)
    S = to_binary(multiplicador, bits)


    P = '0' * bits           
    Q = S                    
    Q_1 = '0'               
    count = bits

    print("Estado inicial:")
    print(f"P = {P}, Q = {Q}, Q-1 = {Q_1}\n")


    while count > 0:
        print(f"Iteración {bits - count + 1}:")
        pair = Q[-1] + Q_1   

        if pair == "01":
            print("Caso 01: P = P + A")
            P = to_binary(from_binary(P) + multiplicando, bits)

        elif pair == "10":
            print("Caso 10: P = P - A")
            P = to_binary(from_binary(P) - multiplicando, bits)

        print(f"Antes del shift: P = {P}, Q = {Q}, Q-1 = {Q_1}")

        total = P + Q + Q_1
        sign_bit = total[0]
        total = sign_bit + total[:-1]   
        P = total[:bits]
        Q = total[bits:2*bits]
        Q_1 = total[-1]

        print(f"Después del shift: P = {P}, Q = {Q}, Q-1 = {Q_1}\n")

        count -= 1


    result_bin = P + Q
    result_dec = from_binary(result_bin)

    print("Resultado final:")
    print(f"Binario: {result_bin}")
    print(f"Decimal: {result_dec}")

    return result_dec, result_bin


if __name__ == "__main__":
    m = int(input("Ingrese el multiplicando: "))
    q = int(input("Ingrese el multiplicador: "))
    bits = 6   
    booth(m, q, bits)