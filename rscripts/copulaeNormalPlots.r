# This script draws surface plots of copulae

library(mnormt)

# Grid
x = seq(0, 1, 0.01)
g = expand.grid(x, x)

# Plots for upper and lower bounds

plotNormalCopula = function(r) {
  # Normal copula
  #rho = -0.9
  n = 2
  cov = diag(n) + r * (matrix(1, n, n) - diag(n))
  copNorm = function(u) { pmnorm(c(qnorm(u[1]), qnorm(u[2])), varcov = cov) }

  # Plot for normal copula
  val = matrix(apply(as.matrix(g), 1, copNorm), length(x), length(x))
  contour(x, x, val, method = "flattest", lty = 2, levels = c(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = bquote(rho == .(r)))
}

plotNormalSamples = function(r) {
  n = 2
  cov = diag(n) + r * (matrix(1, n, n) - diag(n))
  smps = pnorm(rmnorm(400, varcov = cov))
  plot(smps[, 1], smps[, 2], xlab = "u", ylab = "v")
}

# First plot
par(mfcol = c(2, 3), oma = c(0, 0, 2.5, 0))

plotNormalCopula(-0.99)
plotNormalSamples(-0.99)

plotNormalCopula(-0.7)
plotNormalSamples(-0.7)

plotNormalCopula(0)
plotNormalSamples(0)

mtext("Normal", outer = TRUE, cex = 1.5, line = 0)

# Second plot
plotNormalCopula(0.5)
plotNormalSamples(0.5)

plotNormalCopula(0.7)
plotNormalSamples(0.7)

plotNormalCopula(0.99)
plotNormalSamples(0.99)

par(mfcol = c(1, 1), oma = c(0, 0, 0, 0))
