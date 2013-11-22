# This script draws surface plots of Archimedean copulae
library(copula)

# Grid
x = seq(0, 1, 0.01)
g = expand.grid(x, x)

# Plots for upper and lower bounds

plotClaytonCopula = function(r) {
  val = matrix(apply(as.matrix(g), 1, function(x) {pCopula(x, copula = claytonCopula(param = r))}), length(x), length(x))
  contour(x, x, val, method = "flattest", lty = 2, levels = c(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = bquote(theta == .(r)))
}

plotClaytonSamples = function(r) {
  smps = rCopula(400, claytonCopula(param = r))
  plot(smps[, 1], smps[, 2], xlab = "u", ylab = "v")
}

plotFrankCopula = function(r) {
  val = matrix(apply(as.matrix(g), 1, function(x) {pCopula(x, copula = frankCopula(param = r))}), length(x), length(x))
  contour(x, x, val, method = "flattest", lty = 2, levels = c(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = bquote(theta == .(r)))
}

plotFrankSamples = function(r) {
  smps = rCopula(400, frankCopula(param = r))
  plot(smps[, 1], smps[, 2], xlab = "u", ylab = "v")
}

plotGumbelCopula = function(r) {
  val = matrix(apply(as.matrix(g), 1, function(x) {pCopula(x, copula = gumbelCopula(param = r))}), length(x), length(x))
  contour(x, x, val, method = "flattest", lty = 2, levels = c(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = bquote(theta == .(r)))
}

plotGumbelSamples = function(r) {
  smps = rCopula(400, gumbelCopula(param = r))
  plot(smps[, 1], smps[, 2], xlab = "u", ylab = "v")
}

# # First plot
 par(mfcol = c(2, 3), oma = c(0, 0, 2.5, 0))
# 
# plotGumbelCopula(1)
# plotGumbelSamples(1)
# 
# plotGumbelCopula(5)
# plotGumbelSamples(5)
# 
# plotGumbelCopula(100)
# plotGumbelSamples(100)
# 
# mtext("Gumbel", outer = TRUE, cex = 1.5, line = 0)
# 
# plotClaytonCopula(0.1)
# plotClaytonSamples(0.1)
# 
# plotClaytonCopula(7.5)
# plotClaytonSamples(7.5)
# 
# plotClaytonCopula(100)
# plotClaytonSamples(100)
# 
# mtext("Clayton", outer = TRUE, cex = 1.5, line = 0)

plotFrankCopula(-30)
plotFrankSamples(-30)

plotFrankCopula(-10)
plotFrankSamples(-10)

plotFrankCopula(-0.1)
plotFrankSamples(-0.1)

mtext("Frank", outer = TRUE, cex = 1.5, line = 0)

plotFrankCopula(7.5)
plotFrankSamples(7.5)

plotFrankCopula(15)
plotFrankSamples(15)

plotFrankCopula(30)
plotFrankSamples(30)

par(mfcol = c(1, 1), oma = c(0, 0, 0, 0))