# Actually, this script compares normal and log-space sampling

smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesCond.txt", header = TRUE)
#smp2 = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesLog.txt", header = TRUE)

#par(mfrow = c(1, 2))
#plot(pnorm(smp$X1), pnorm(smp$X2), main = "Regular Sampling")
#plot(pnorm(smp2$X1), pnorm(smp2$X2), main = "Sampling from Log-space")
#plot(pnorm(x[, 1]), pnorm(x[, 2]), main = "Samples from R")
#par(mfrow = c(1, 1))

n = 4
rho = 0.75
Sigma = diag(n) + rho * (matrix(1, n, n) - diag(n))
Sigma11 = Sigma[c(1, 2), c(1, 2)]
Sigma12 = Sigma[c(1, 2), c(3, 4)]
Sigma21 = Sigma[c(3, 4), c(1, 2)]
Sigma22 = Sigma[c(3, 4), c(3, 4)]
a = c(0.5, -0.2)

hist(smp$X1)
print(c("mean", mean(smp$X1), mean(smp$X4)))
print(Sigma12 %*% solve(Sigma22) %*% a)

print(cov(cbind(smp$X1, smp$X4)))
print(Sigma11 - Sigma12 %*% solve(Sigma22) %*% Sigma21)