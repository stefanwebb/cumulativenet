## Make plots for sampling example for AI talk
library(copula)

par(mfrow=c(1, 2))

smps = rCopula(500, claytonCopula(param = 5))

plot(qnorm(smps[, 1]), qexp(smps[, 2]), xlab = "x", ylab = "y", main = "Joint Distribution")

plot(smps[, 1], smps[, 2], xlab = "u", ylab = "v", main = "Copula Distribution")

hist(qnorm(smps[, 1]), xlab = "x", main = "Marginal of X")
hist(qexp(smps[, 2]), xlab = "y", main = "Marginal of Y")

par(mfrow=c(1, 1))