# This script tests learning when there is a single trivariate normal factor

library(mnormt)

# Function to calculate normal density and CDF of some variables given others
partialDeriv = function(x, s, sigma)
{
  # If empty subset, then simply return CDF
  if (length(s) == 0) {
    return(apply(x, MARGIN = 1, pmnorm, mean = rep(0, dim(x)[2]), varcov = sigma))
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
    
    sigma = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
    
    if (class(x[,-s]) == "matrix") {
      return(dmnorm(x[,s], rep(0, length(s)), sigma22) * apply(x[,-s], MARGIN = 1, pmnorm, mean = mu, varcov = sigma))
    }
    
    else {
      return(dmnorm(x[,s], rep(0, length(s)), sigma22) * pmnorm(x[,-s], mu, sigma))
    }    
  }
}

# Initial rho from program
prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/parameters.txt", header = TRUE)
rho1 = prm$rho0

#rho1 = -0.965215

n = 3
cov1 = diag(n) + rho1 * (matrix(1, n, n) - diag(n))

# Load sample
smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/trifactor.txt", header = TRUE)
X1 = smp$X1
X2 = smp$X2
X3 = smp$X3

U1 = pnorm(X1)
U2 = pnorm(X2)
U3 = pnorm(X2)

V1 = U1
V2 = U2
V3 = U3

W1 = qnorm(V1)
W2 = qnorm(V2)
W3 = qnorm(V3)

dW1dV1 = 1 / dnorm(qnorm(V1))
dW2dV2 = 1 / dnorm(qnorm(V2))
dW3dV3 = 1 / dnorm(qnorm(V3))

dV1dU1 = 1
dV2dU2 = 1
dV3dU3 = 1

dU1dX1 = dnorm(X1)
dU2dX2 = dnorm(X2)
dU3dX3 = dnorm(X3)

C = dW1dV1 * dW2dV2 * dW3dV3 * dV1dU1 * dV2dU2 * dV3dU3 * dU1dX1 * dU2dX2 * dU3dX3

F = partialDeriv(cbind(W1, W2), c(), cov1)
f = partialDeriv(cbind(W1, W2), c(1, 2, 3), cov1)
#dF1dW1 = partialDeriv(cbind(W1, W2), c(1), cov1)
#dF1dW2 = partialDeriv(cbind(W1, W2), c(2), cov1)

#f = C * (f1 * F2 + dF1dW1 * dF2dW2 + dF1dW2 * dF2dW1 + F1 * f2)
#den = f1 * F2 + dF1dW1 * dF2dW2 + dF1dW2 * dF2dW1 + F1 * f2

#precisionZ1 = solve(cov1) %*% rbind(W1, W2)

#df1dW1dW2 = f1 * (precisionZ1[1,] * precisionZ1[2,] - solve(cov1)[1,2])

#df1dW1 = f1 * -precisionZ1[1,]
#df1dW2 = f1 * -precisionZ1[2,]

#dfdRho1 = C * (df1dW1dW2 * F2 + df1dW1 * dF2dW2 + df1dW2 * dF2dW1 + f1 * f2)

#dlnfdRho1 = dfdRho1 / f

#grad = cbind(df1dW1dW2 * F2 + df1dW1 * dF2dW2 + df1dW2 * dF2dW1 + f1 * f2, f1 * f2 + dF1dW1 * df2dW2 + dF1dW2 * df2dW1 + F1 * df2dW1dW2)
#logGrad = c(sum(dlnfdRho1), sum(dlnfdRho2))

#print(c("logLike", sum(log(den) / length(X1)), "logGrad", logGrad / length(X1)))
print(c("logLike", sum(log(f) / length(X1))))