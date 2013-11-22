# Actually, this script compares normal and log-space sampling

smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesNormal.txt", header = TRUE)
smp2 = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesLog.txt", header = TRUE)

par(mfrow = c(1, 2))
plot(pnorm(smp$X1), pnorm(smp$X2), main = "Regular Sampling")
plot(pnorm(smp2$X1), pnorm(smp2$X2), main = "Sampling from Log-space")
#plot(pnorm(x[, 1]), pnorm(x[, 2]), main = "Samples from R")
par(mfrow = c(1, 1))