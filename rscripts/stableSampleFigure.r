# Make figure that demonstrates stable sampling
library(copula)

plotRsample = function(r) {
  x <- rCopula(2000, claytonCopula(param = r))
  plot(x[,1], x[,2], xlab = "u", ylab = "v", main = bquote(theta == .(r)))
}

plotCsample = function(r, fileName) {
  smp = read.table(paste("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/experiments/", fileName, sep = ""), header = TRUE)
  plot(smp$X1, smp$X2, xlab = "u", ylab = "v", main = bquote(theta == .(r)))
}

# Do first figure for R output
par(mfcol = c(1, 3), oma = c(0, 0, 2.5, 0))
plotRsample(300)
plotRsample(1000)
plotRsample(10000)
mtext("R", outer = TRUE, cex = 1.5, line = 0)
par(mfcol = c(1, 1), oma = c(0, 0, 0, 0))

# Do second figure for C++ output
par(mfcol = c(1, 3), oma = c(0, 0, 2.5, 0))
plotCsample(300, "samplesClayton300.txt")
plotCsample(1000, "samplesClayton1000.txt")
plotCsample(10000, "samplesClayton10000.txt")
mtext("C++", outer = TRUE, cex = 1.5, line = 0)
par(mfcol = c(1, 1), oma = c(0, 0, 0, 0))