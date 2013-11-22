# This script draws surface plots of copulae

library(mnormt)

# Upper (lower?) bound copula
M2 = function(u) { min(u[1], u[2]) }

# Lower bound copula
W2 = function(u) { max(u[1] + u[2] - 1, 0)}

# Independence copula
uv = function(u) { u[1] * u[2] }

# Plots for upper and lower bounds

par(mfrow = c(2, 3))

# Grid
x = seq(0, 1, 0.05)
g = expand.grid(x, x)

val = matrix(apply(as.matrix(g), 1, W2), length(x), length(x))
contour(x, x, val, method = "flattest", lty = 2, levels = c(0.0000001, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = "Lower Bound")

# Grid
x = seq(0, 1, 0.01)
g = expand.grid(x, x)

val = matrix(apply(as.matrix(g), 1, uv), length(x), length(x))
contour(x, x, val, method = "flattest", lty = 2, levels = c(0.0000001, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = "Independence Copula")

# Grid (need finer grid so get straight lines)
x = seq(0, 1, 0.001)
g = expand.grid(x, x)

val = matrix(apply(as.matrix(g), 1, M2), length(x), length(x))
contour(x, x, val, levels = c(0.0000001, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = "Upper Bound", lty = 2)

u = runif(100)
plot(u, 1 - u, xlab = "u", ylab = "v")

u1 = runif(500)
v1 = runif(500)
plot(u1, v1, xlab = "u", ylab = "v")

u = runif(100)
plot(u, u, xlab = "u", ylab = "v")

par(mfrow = c(1, 1))

# Normal copula rho = 1
#rho = 0
#n = 2
#cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
#copNorm = function(u) { pmnorm(c(qnorm(u[1]), qnorm(u[2])), varcov = cov) }

# Plot for normal copula
#val = matrix(apply(as.matrix(g), 1, copNorm), length(x), length(x))
#contour(x, x, val, method = "flattest", lty = 2, levels = c(0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1), labels = c("0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1"), xlab = "u", ylab = "v", main = "Normal Copula")

#val = matrix(apply(as.matrix(g), 1, W2), length(x), length(x))
#val = matrix(apply(as.matrix(g), 1, copNorm), length(x), length(x))

#persp(x, x, val, theta = -30, phi = 25, shade = 0.3)
