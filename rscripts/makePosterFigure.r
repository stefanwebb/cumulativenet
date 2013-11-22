# This script creates a figure for my poster of sample marginals and learning on a contour diagram

library(mnormt)

# Function to calculate normal density and CDF of some variables given others
partialDeriv = function(x, s, sigma)
{
  # If empty subset, then simply return CDF
  if (length(s) == 0) {
    return(pmnorm(x, rep(0, dim(x)[2]), sigma))
  }
  
  # If full subset, then return density
  else if (length(s) == dim(x)[2]) {
    return(dmnorm(x, rep(0, dim(x)[2]), sigma))
  }
  
  # Else return product
  else {
    sigma12 = sigma[-s, s]
    sigma11 = sigma[-s, -s]
    sigma22 = sigma[s, s]
    
    mu = sigma12 %*% solve(sigma22) %*% t(x[,s])
    
    #print(c("mu", s, mu))
    
    sigma = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
        
    if (length(s) == 2) {
      print(c("sigma", s, sqrt(sigma)))
    }
    
    #print(dmnorm(x[,s], rep(0, length(s)), sigma22))
    
    return(dmnorm(x[,s], rep(0, length(s)), sigma22) * pmnorm(x[,-s], mu, sigma))
  }
}

#smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/factor.txt", header = TRUE)
#U1 = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/u1.txt", header = TRUE)$U1
#realFirstTerms = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/firstTerms.txt", header = TRUE)$firstTerms
#realSecondTerms = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/secondTerms.txt", header = TRUE)$secondTerms

n = 2

# Load sample
smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/twoFactors.txt", header = TRUE)
X1 = smp$X1
X2 = smp$X2
X3 = smp$X3

U1 = pnorm(X1)
U2 = pnorm(X2)
U3 = pnorm(X3)

V1 = U1
V2 = sqrt(U2)
V3 = U3

W1 = qnorm(V1)
W2 = qnorm(V2)
W3 = qnorm(V3)

dW1dV1 = 1 / dnorm(qnorm(V1))
dW2dV2 = 1 / dnorm(qnorm(V2))
dW3dV3 = 1 / dnorm(qnorm(V2))

dV1dU1 = 1
dV2dU2 = 0.5 * U2^-0.5
dV3dU3 = 1

dU1dX1 = dnorm(X1)
dU2dX2 = dnorm(X2)
dU3dX3 = dnorm(X3)

#C = dW1dV1 * dW2dV2 * dW3dV3 * dV1dU1 * dV2dU2 * dV3dU3 * dU1dX1 * dU2dX2 * dU3dX3

logLikelihood = function(x)
{
  rho1 = x[1]
  rho2 = x[2]
  
  cov1 = diag(n) + rho1 * (matrix(1, n, n) - diag(n))
  cov2 = diag(n) + rho2 * (matrix(1, n, n) - diag(n))
  
  f1 = partialDeriv(cbind(W1, W2), c(1, 2), cov1)

  dF1dW1 = partialDeriv(cbind(W1, W2), c(1), cov1)

  dF2dW3 = partialDeriv(cbind(W2, W3), c(2), cov2)

  f2 = partialDeriv(cbind(W2, W3), c(1, 2), cov2)

  #f = C * (f1 * dF2dW3 + dF1dW1 * f2)
  den = (f1 * dF2dW3 + dF1dW1 * f2)

  #precisionZ1 = solve(cov1) %*% rbind(W1, W2)
  #precisionZ2 = solve(cov2) %*% rbind(W2, W3)

  #df1dW1dW2 = f1 * (precisionZ1[1,] * precisionZ1[2,] - solve(cov1)[1,2])
  #df2dW2dW3 = f2 * (precisionZ2[1,] * precisionZ2[2,] - solve(cov2)[1,2])
                  
  #df1dW1 = f1 * -precisionZ1[1,]
  #df2dW3 = f2 * -precisionZ2[2,]

  #dfdRho1 = C * (df1dW1dW2 * dF2dW3 + df1dW1 * f2)
  #dfdRho2 = C * (f1 * df2dW3 + dF1dW1 * df2dW2dW3)

  #grad = cbind((df1dW1dW2 * dF2dW3 + df1dW1 * f2), (f1 * df2dW3 + dF1dW1 * df2dW2dW3))
  
  #dlnfdRho1 = dfdRho1 / f
  #dlnfdRho2 = dfdRho2 / f

  #logGrad = c(sum(dlnfdRho1), sum(dlnfdRho2))
  
  logLike = sum(log(den)) / length(X1)
  return (logLike)
}

# Initial rho from program
prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/parameters.txt", header = TRUE)
#rho1 = prm$rho0
#rho2 = prm$rho1

#rho1 = -0.967133
#rho2 = 0.920703

#print(c("logLike", logLikelihood(rho1, rho2)))

rhoGrid = function(n)
{
  p = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  #q = seq(1, -1, 2 / (n - 1))
  
  return (expand.grid(p, p))
}

plotContours = function(n)
{
  x = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  y = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  z = matrix(apply(as.matrix(rhoGrid(n)), 1, logLikelihood), n, n)
  #z = apply(as.matrix(rhoGrid(n)), 1, logLikelihood)
  contour(x, y, z, levels = c(-16, -10, -5, -4.5, -4.4, -4.3, -4.25, -4.2, -4.15, -4.1, -4.075, -4.05, -4.025, -4, -3.99, -3.98, -3.979), cex = 2.0)
  #filled.contour(x, y, z)
  
  #print(x)
  #print(y)
  #print(z)
}

plotContours(40)

logSpaced = function(a, b, n)
{
  return (a * exp(log(b / a) * (((1:n) - 1) / (n - 1))))
  return (((((1:n) / 5)^10 - 1) / (b - a)) + a)
}

# Print solution 1
prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/solutions1.txt", header = TRUE)
rho1 = prm$rho0
rho2 = prm$rho1

s = as.character(1:length(rho1))

for (i in 1:length(rho1)) {
  text(rho1[i], rho2[i], s[i], col = "red", cex = 1.3)
}

#lines(rho1, rho2, type = "c", lty = 2, col = "red", lwd = 3, cex = 4)

# Print solution 2
prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/solutions2.txt", header = TRUE)
rho1 = prm$rho0
rho2 = prm$rho1

s = as.character(1:length(rho1))

for (i in 1:length(rho1)) {
  text(rho1[i], rho2[i], s[i], col = "blue", cex = 1.3)
}

#lines(rho1, rho2, type = "c", lty = 2, col = "blue", lwd = 3, cex = 4)

# Print solution 3
prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/solutions3.txt", header = TRUE)
rho1 = prm$rho0
rho2 = prm$rho1

s = as.character(1:length(rho1))

for (i in 1:length(rho1)) {
  text(rho1[i], rho2[i], s[i], col = "green", cex = 1.3)
}

#lines(rho1, rho2, type = "c", lty = 2, col = "green", lwd = 3, cex = 4)

# Print solution 4
# prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/solutions4.txt", header = TRUE)
# rho1 = prm$rho0
# rho2 = prm$rho1
# 
# s = as.character(1:length(rho1))
# 
# for (i in 1:length(rho1)) {
#   text(rho1[i], rho2[i], s[i], col = "purple")
# }
# 
# lines(rho1, rho2, type = "c", lty = 2, col = "purple")

#par(mfrow=c(2,2))
#hist(qnorm(U1), freq = FALSE, main = "", xlab = "", ylab = "", cex.axis = 1.3)
#hist(qexp(U2, rate = 1), freq = FALSE, main = "", xlab = "", ylab = "", cex.axis = 1.3)
#hist(qbeta(U3, 0.5, 0.5), freq = FALSE, main = "", xlab = "", ylab = "", cex.axis = 1.3)
#pairs(data.frame(X1 = qnorm(U1)[1:1000], X2 = qexp(U2)[1:1000], X3 = qbeta(U3, 0.5, 0.5)[1:1000]), cex = 1.0)
#par(mfrow=c(1,1))