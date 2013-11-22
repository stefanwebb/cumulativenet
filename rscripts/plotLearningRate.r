# Experimenting with learning rate
x = 1:50
#rate = 0.5 / (1 + x)
#rate = 0.5 ^ x
rate = exp(-x/1)
plot(x, rate)