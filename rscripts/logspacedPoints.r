b = 10
a = 10000
n = 9
x = seq(0, 1, 1 / (n - 1))

plot(1:n, b * (a / b)^x, type = "b")
print(round(b * (a / b)^x))