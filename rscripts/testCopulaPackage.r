# Playing around with copula package
library(copula)

#copula = gumbelCopula(param = 100)

copula = claytonCopula(param = 50)

# Plot random sample to get idea of dependence relation
par(mfrow = c(1, 1))
x <- rCopula(2000, copula)
plot(x[,1], x[,2])
#print(cor(x[,1], x[,2]))

# Plot contour plot of copula
#contour(gumbelCopula(100), pCopula, lty = 2)

# Plot contour plot of copula density
#contour(gumbelCopula(100), pCopula, lty = 2)

# Marginals of copula distribution
#par(mfrow = c(1, 2))
#hist(x[,1])
#hist(x[,2])
#par(mfrow = c(1, 1))

x0 = 0.99997823871672153
x1 = 1.0
#print(dCopula(c(x0, x1), copula))

#u1 = 4.4316984713077545e-006
print(dCopula(c(0.1, 0.1), claytonCopula(param = 300, dim = 2)))
print(pCopula(c(0.1, 0.1), claytonCopula(param = 300, dim = 2)))  # 