# Sample from a bivariate normal distribution without using the iterative method
rho = 0.0173924
n = 100000
X1 = rnorm(n)
X2 = rnorm(n, rho * X1, rep(1 - rho^2, n))
print(c(cor(X1, X2) * (n - 1) / n, cov(X1, X2) * (n - 1) / n))