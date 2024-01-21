import random


# Функция для генерации n строк с случайными числами
def generate_random_data(n, filename='data.txt'):
    with open(filename, 'w') as file:
        for _ in range(n):
            # Генерация трех случайных вещественных чисел и одного целого
            float1 = random.uniform(0, 100)
            float2 = random.uniform(0, 100)
            float3 = random.uniform(0, 100)
            int_num = random.randint(0, 2)

            # Запись чисел в файл, форматирование их в строки
            file.write(f'{float1:.2f} {float2:.2f} {float3:.2f} {int_num}\n')


# Вызов функции для создания файла с n случайными строками
n = 10000  # Количество рядов данных, которое вы хотите сгенерировать
generate_random_data(n)