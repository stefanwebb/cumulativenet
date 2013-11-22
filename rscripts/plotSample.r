library(copula)

#smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/test.txt", header = TRUE)
#smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cn2/test.txt", header = TRUE)

smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesClayton.txt", header = TRUE)
#par(mfrow = c(1, 2))
plot(pnorm(smp$X1), pnorm(smp$X2), main = "Sample from C++")

#x <- rCopula(2000, claytonCopula(param = 1000))
#plot(x[, 1], x[, 2], main = "Samples from R")

#par(mfrow = c(1, 1))

# Trying root solving by hand to see why num
k = 0.51953080273233354
u1 = 4.4316984713077545e-006

#g = function(u2) { pCopula(c(u1, u2), claytonCopula(param = 300)) / u1 }

#print(g(0.1))

#print(cor(pnorm(smp$X1), pnorm(smp$X2)))

# Marginals of copula distribution
#par(mfrow = c(1, 2))
#hist(pnorm(smp$X1))
#hist(pnorm(smp$X2))
#par(mfrow = c(1, 1))

# Plot our output next to "copula" package output
#x = seq(0, 1, 0.01)
#g = expand.grid(x, x)
#copVals = scan(file = "C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/copulaValues.txt")
#copVals = matrix(copVals, sqrt(length(copVals)), sqrt(length(copVals)))
#trueCopVals = apply(as.matrix(g), 1, pCopula, copula = claytonCopula(param = 50))

#par(mfrow = c(1, 2))
#contour(x, x, copVals, method = "flattest", lty = 2, levels = c(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v")
#contour(x, x, matrix(trueCopVals, length(x), length(x)), lty = 2)
#par(mfrow = c(1, 1))

#denVals = scan(file = "C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/copulaDenValues.txt")
#trueDenVals = apply(as.matrix(g), 1, dCopula, copula = gumbelCopula(param = 100))

# Locate 
#print(which(abs(pnorm(smp$X2) - pnorm(smp$X1)) > 0.4))

