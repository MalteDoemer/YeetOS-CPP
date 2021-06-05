def compute_mem(start, step, size):
    mem = 0

    for i in range(start, start + step*16, step):
        mem += i * size

    return mem

def size_to_index(size: int):
    if (size <= 128):
        return 0 + (size - 1) // 8
    elif (size <= 384):
        return 16 + (size - 128 - 1) // 16
    elif (size <= 896):
        return 32 + (size - 384 - 1) // 32
    elif (size <= 1920):
        return 48 + (size - 896 - 1) // 64
    elif (size <= 3968):
        return 64 + (size - 1920 - 1) // 128
    else:
        print("size must be <= 3968")
        exit(1)


def create_csv(path: str):
    with open(path, "w") as file:
        for i in range(1, 3969):
            file.write(f"{i}, {size_to_index(i)}\n")
        
