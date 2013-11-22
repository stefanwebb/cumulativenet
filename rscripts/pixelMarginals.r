## Test what pixel marginals could look like
## NOTE: We need continuous marginals for a copula model, so I am using a normal distribution, 
## with >= 0 equal to 1, and < 0 equal to 0

p = 0.5
x = rbinom(1000, 1, p) * 2 - 1
tempX = seq(-3, 3, 0.001)
plot(tempX, dnorm(tempX, mean(x), sd(x)), type = "l")
#plot(tempX, dt(tempX, 50, mean(x)), type = "l")

# Calculate probabilities of being a black or white pixel
pBlack = pnorm(0, mean(x), sd(x))
#pBlack = pt(0, 5, mean(x))
pWhite = 1 - pBlack
print(c(pBlack, pWhite))
print(c(mean(x), sd(x)))