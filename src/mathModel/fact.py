"""
Функция вычисления факториала числа. Подключается калькулятором, написана в целях тестов
"""

from typing import List


def fact(a: List[float]) -> float:
    if a[0] == 1: return a[0]
    else: return a[0] * fact([a[0] - 1])
