library(mnormt)

# Function to calculate normal density and CDF of some variables given others
partialDeriv = function(x, s, sigma)
{
  # If empty subset, then simply return CDF
  if (length(s) == 0) {
    return(pmnorm(x, rep(0, length(x)), sigma))
  }
  
  # If full subset, then return density
  else if (length(s) == length(x)) {
    return(dmnorm(x, rep(0, length(x)), sigma)[1])
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
    
    return(dmnorm(x[,s], rep(0, length(s)), sigma22) * pmnorm(x[,-s], mu, sigma))
  }
}

smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/factor.txt", header = TRUE)
U1 = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/u1.txt", header = TRUE)$U1
realFirstTerms = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/firstTerms.txt", header = TRUE)$firstTerms
realSecondTerms = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/secondTerms.txt", header = TRUE)$secondTerms

# Initial rho from program
rho = 0.9 #0.0173924
n = 2
cov = diag(n) + rho * (matrix(1, n, n) - diag(n))

# Generate our first variables
#U1 = runif(1000)
W1 = qnorm(U1)
U2 = rep(0.5, length(U1))
dW1dU1 = 1 / dnorm(qnorm(U1))

#ks = runif(1000)
ks = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/ks.txt", header = TRUE)$ks

for (i in 1:15) {
  W2 = qnorm(U2)
  dW2dU2 = 1 / dnorm(qnorm(U2))
  firstTerms = dW1dU1 * partialDeriv(cbind(W1, W2), c(1), cov)
  denom = dW1dU1 * dnorm(W1)
  secondTerms = dW1dU1 * dW2dU2 * dmnorm(cbind(W1, W2), rep(0, 2), cov)

  U2 = U2 - (firstTerms - ks * denom) / secondTerms
  U2 = ifelse(U2 > 1, 1 - 1e-5, U2)
  U2 = ifelse(U2 < 0, 1e-5, U2)
}
W2 = qnorm(U2)