from numpy import *
import matplotlib.pyplot as plt

counter = dict()
tmp = 0
with open("total.data", "r") as file:
    for line in file:
        str = line.split(" ")
        if len(str) > 1:
            counter.update({str[0]: int(str[1])})

values = sorted(counter.values(), reverse=True)
len_values = len(values)
ids = [i for i in range(len_values)]

t = linspace(1, len_values, len_values)
a = values[0] / t
b = power(10, 7.2) * power(t + 1, -1.05)

fig = plt.figure()
ex = fig.add_subplot(111)
ex.plot(ids, list(values), 'r', label="Частота")
ex.plot(t, a, 'b', label="Закон Ципфа")
ex.plot(t, b, 'g', label="Формула Мандельброта")
ex.set_yscale("log")
ex.set_xscale("log")
ex.legend()
plt.show()