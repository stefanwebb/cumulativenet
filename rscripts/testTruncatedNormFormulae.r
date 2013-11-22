# I wrote this script to test formulae for the truncated normal distribution against the tmvtnorm package
library(tmvtnorm)
library(mnormt)

# Set parameters
mu = c(1, 6)
sigma = matrix(c(3, 0.5, 0.5, 1.5), 2, 2)
a = c(2, 5)
x <- seq(-5, 5, by=0.01)

# My own implementation of marginal of truncated normal
truncNormalMarg = function(xn, n = 1, mean = rep(0, nrow(sigma)), sigma = diag(length(mean)), upper = rep( Inf, length = length(mean)))
{
  den = dmnorm(xn, mean[n], varcov = sigma[n,n])
  den = ifelse(xn <= upper[n], den, 0)
  
  skewNumerator = function(x)
  {
    pmnorm(upper[-n], mean = mean[-n] + sigma[-n, n] * (x - mean[n]) / sigma[n,n], varcov = solve(solve(sigma)[-n, -n]))
  }
  
  skew = sapply(xn, skewNumerator) / pmnorm(upper, mean = mean, varcov = sigma)
  
  # The marginals are the normal density multiplied by "skew factor"
  return(den * skew)
}

myDtmvnorm.marginal = function (xn, n = 1, mean = rep(0, nrow(sigma)), sigma = diag(length(mean)), lower = rep(-Inf, length = length(mean)), upper = rep(Inf, length = length(mean)), log = FALSE)
{
  if (NROW(sigma) != NCOL(sigma)) {
    stop("sigma must be a square matrix")
  }
  if (length(mean) != NROW(sigma)) {
    stop("mean and sigma have non-conforming size")
  }
  k <- length(mean)
  if (n < 1 || n > length(mean) || !is.numeric(n) || length(n) > 
        1 || !n %in% 1:length(mean)) {
    stop("n must be a integer scalar in 1..length(mean)")
  }
  if (k == 1) {
    prob <- pnorm(upper, mean = mean, sd = sqrt(sigma)) - pnorm(lower, mean = mean, sd = sqrt(sigma))
    density <- ifelse(lower[1] <= xn & xn <= upper[1], dnorm(xn, mean = mean, sd = sqrt(sigma))/prob, 0)
    if (log == TRUE) {
      return(log(density))
    }
    else {
      return(density)
    }
  }
  C <- sigma
  A <- solve(sigma)
  A_1 <- A[-n, -n]
  A_1_inv <- solve(A_1)
  C_1 <- C[-n, -n]
  c_nn <- C[n, n]
  c <- C[-n, n]
  mu <- mean
  mu_1 <- mean[-n]
  mu_n <- mean[n]
  p <- pmnorm(upper, mean = mu, varcov = C)
  f_xn <- c()
  skews = c()  # debug
  for (i in 1:length(xn)) {
    if (!(lower[n] <= xn[i] && xn[i] <= upper[n]) || is.infinite(xn[i])) {
      f_xn[i] <- 0
      next
    }
    m <- mu_1 + (xn[i] - mu_n) * c/c_nn
    #f_xn[i] <- exp(-0.5 * (xn[i] - mu_n)^2/c_nn) * pmnorm(upper[-n], mean = m, varcov = A_1_inv)
    f_xn[i] <- dnorm(xn[i], mu_n, c_nn) * pmnorm(upper[-n], mean = m, varcov = A_1_inv)
    #skews = c(skews, pmvnorm(lower = lower[-n], upper = upper[-n], mean = m, sigma = A_1_inv)[1])
  }
  #print(skews)
  
  #density <- 1/p * 1/sqrt(2 * pi * c_nn) * f_xn
  density <- 1/p * f_xn
  if (log == TRUE) {
    return(log(density))
  }
  else {
    return(density)
  }
}

# Test package function for marginal of truncated normal
fx <- myDtmvnorm.marginal(x, n = 1, mean = mu, sigma = sigma, upper = a)

#par(mfrow=c(1,2))
#plot(x, fx, type = "l", col="red")

#mtmvnorm
truncNormalMu = function(mean = rep(0, nrow(sigma)), sigma = diag(length(mean)), upper = rep(Inf, length = length(mean)))
{
  # Calculate densities of marginals evaluated at upper bounds
  den = rep(0, length(mean))
  for (i in 1:length(mean)) {
    den[i] = truncNormalMarg(upper[i], i, mean, sigma, upper)
  }
  
  return(-sigma %*% den + mean)
}

#fx2 <- truncNormalMarg(x, n = 1, mean = mu, sigma = sigma, upper = a)
#plot(x, fx2, type = "l", col="red")
#par(mfrow=c(1,1))

#print(truncNormalMu(mu, sigma, a))
#print(mtmvnorm(as.vector(mu), sigma, upper = a, doComputeVariance = FALSE)$tmean)

# My own implementation of marginal of truncated normal
truncNormalBiMarg = function(xn, n = c(1,2), mean = rep(0, nrow(sigma)), sigma = diag(length(mean)), upper = rep( Inf, length = length(mean)))
{
  den = dmnorm(xn, mean[n], varcov = sigma[n,n])
  den = ifelse(xn <= upper[n], den, 0)
  
  skewNumerator = function(x)
  {
    pmnorm(upper[-n], mean = mean[-n] + sigma[-n, n] * (x - mean[n]) / sigma[n,n], varcov = solve(solve(sigma)[-n, -n]))
  }
  
  skew = sapply(xn, skewNumerator) / pmnorm(upper, mean = mean, varcov = sigma)
  
  # The marginals are the normal density multiplied by "skew factor"
  return(den * skew)
}

# mu, sigma, a, x
biResults = mtmvnorm(mu, sigma, upper = a)
biResults$tvar = biResults$tvar + cbind(c(biResults$tmean[1]^2, prod(biResults$tmean)), c(prod(biResults$tmean), biResults$tmean[2]^2))
print(biResults)
#print(cbind(c(biResults$tmean[1]^2, prod(biResults$tmean)), c(prod(biResults$tmean), biResults$tmean[2]^2)))

p = pmnorm(a, mu, varcov = sigma)

rho = sigma[1,2] / sqrt(sigma[1,1] * sigma[2,2])

h = (a[1] - mu[1]) / sqrt(sigma[1, 1])
k = (a[2] - mu[2]) / sqrt(sigma[2, 2])

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

means = c(m10, m01) * sqrt(diag(sigma)) + mu
#vars = matrix(c((m20 - m10^2) * sigma[1,1], (m11 - m10 * m01) * sqrt(prod(diag(sigma))), (m11 - m10 * m01) * sqrt(prod(diag(sigma))), (m02 - m01^2) * sigma[2,2]), 2, 2)
vars = matrix(c((m20 - m10^2) * sigma[1,1], (m11 - m10 * m01) * sqrt(prod(diag(sigma))), (m11 - m10 * m01) * sqrt(prod(diag(sigma))), (m02 - m01^2) * sigma[2,2]), 2, 2)
vars = vars + cbind(c(means[1]^2, means[1] * means[2]), c(means[1] * means[2], means[2]^2))

results = list(mean = means, var = vars)
print(results)

## Univariate results

# E[X]
m1 = mu[1] - sqrt(sigma[1,1]) * dnorm(h) / pnorm(h)

# E[X^2]
m2 = sigma[1,1] + mu[1]^2 - (sigma[1,1] * h + 2 * mu[1] * sqrt(sigma[1,1])) * dnorm(h) / pnorm(h)

uniResults = mtmvnorm(mu[1], sigma[1,1], upper = a[1])

print(c(uniResults$tmean, uniResults$tvar + uniResults$tmean^2))
print(c(m1, m2))
