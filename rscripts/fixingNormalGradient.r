# Fixing gradient normal copula
n = 3
rho = 0.5
sigma = diag(n) + rho * (matrix(1, n, n) - diag(n))

x = qnorm(c(0.2, 0.6, 0.1, 1.0))
s = c(1, 2)

sigma12 = sigma[-s, s]
sigma11 = sigma[-s, -s]
sigma22 = sigma[s, s]

mu = sigma12 %*% solve(sigma22) %*% x[s]
sigmaNew = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12

print(mu)
print(sigmaNew)