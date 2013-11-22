# Debugging calculateTruncMoments function

library(mnormt)
library(tmvtnorm)

#u = c(0.2, 0.6, 0.4)
u = c(0.2, 0.6, 0.4, 0.1)
x = qnorm(u)

rho = 0.5
#n = 3
n = 4
sigma = diag(n) + rho * (matrix(1, n, n) - diag(n))

s = c(1, 2)

sigma12 = sigma[-s, s]
sigma11 = sigma[-s, -s]
sigma22 = sigma[s, s]

mu = sigma12 %*% solve(sigma22) %*% x[s]

sigmaNew = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12

print(mtmvnorm(as.vector(mu), sigmaNew, upper = x[-s], doComputeVariance = TRUE))

##

a = x[-s]

p = pmnorm(a, mu, varcov = sigmaNew)

rho = sigmaNew[1,2] / sqrt(sigmaNew[1,1] * sigmaNew[2,2])

h = (a[1] - mu[1]) / sqrt(sigmaNew[1, 1])
k = (a[2] - mu[2]) / sqrt(sigmaNew[2, 2])

c = 1 / sqrt(1 - rho^2)

## Bivariate results

# E[X]
term1 = dnorm(h) * pnorm((k - rho * h) * c)
term2 = rho * dnorm(k) * pnorm((h - rho * k) * c)
m10 = -(term1 + term2) / p #* sqrt(sigma[1,1]) + mu[1]

# E[Y]
term1 = dnorm(k) * pnorm((h - rho * k) * c)
term2 = rho * dnorm(h) * pnorm((k - rho * h) * c)
m01 = -(term1 + term2) / p #* sqrt(sigma[2,2]) + mu[2]

# E[X^2]
term1 = h * dnorm(h) * pnorm((k - h * rho) * c)
term2 = rho^2 * k * dnorm(k) * pnorm((h - k * rho) * c)
term3 = rho / c / sqrt(2 * pi) * dnorm(sqrt(h^2 - 2 * rho * h * k + k^2) * c)
m20 = (p - term1 - term2 + term3) / p

# E[Y^2]
term1 = k * dnorm(k) * pnorm((h - k * rho) * c)
term2 = rho^2 * h * dnorm(h) * pnorm((k - h * rho) * c)
term3 = rho / c / sqrt(2 * pi) * dnorm(sqrt(k^2 - 2 * rho * k * h + h^2) * c)
m02 = (p - term1 - term2 + term3) / p

# E[XY]
term1 = rho * h * dnorm(h) * pnorm((k - rho * h) * c)
term2 = rho * k * dnorm(k) * pnorm((h - rho * k) * c)
term3 = 1 / c / sqrt(2 * pi) * dnorm(-sqrt(h^2 - 2 * h * k * rho + k^2) * c)
m11 = (rho * p - term1 - term2 + term3) / p

means = c(m10, m01) * sqrt(diag(sigmaNew)) + mu
#vars = matrix(c((m20 - m10^2) * sigma[1,1], (m11 - m10 * m01) * sqrt(prod(diag(sigma))), (m11 - m10 * m01) * sqrt(prod(diag(sigma))), (m02 - m01^2) * sigma[2,2]), 2, 2)
vars = matrix(c((m20 - m10^2) * sigmaNew[1,1], (m11 - m10 * m01) * sqrt(prod(diag(sigmaNew))), (m11 - m10 * m01) * sqrt(prod(diag(sigmaNew))), (m02 - m01^2) * sigmaNew[2,2]), 2, 2)
vars = vars + cbind(c(means[1]^2, means[1] * means[2]), c(means[1] * means[2], means[2]^2))

results = list(mean = means, var = vars)
print(results)

#print(-0.75 * x[1]^2 + 2.25 * x[1] * x[2] + 0.25 * x[1] * x[2] - 0.75 * x[2]^2)