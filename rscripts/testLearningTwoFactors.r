library(mnormt)

# Function to calculate normal density and CDF of some variables given others
partialDeriv = function(x, s, sigma)
{
  if (class(x) == "matrix") {
    if (length(s) == 0) {
      return(apply(x, MARGIN = 1, pmnorm, mean = rep(0, dim(x)[2]), varcov = sigma))
    }
    
    else if (length(s) == dim(x)[2]) {
      return(dmnorm(x, rep(0, dim(x)[2]), sigma))
    }
    
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
  
  else {
    if (length(s) == 0) {
      return(pmnorm(x, rep(0, length(x)), sigma)[1])
    }
    
    else if (length(s) == length(x)) {
      return(dmnorm(x, rep(0, length(x)), sigma))
    }
    
    else {
      sigma12 = sigma[-s, s]
      sigma11 = sigma[-s, -s]
      sigma22 = sigma[s, s]
      
      mu = sigma12 %*% solve(sigma22) %*% x[s]
      
      sigma = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
      
      return(dmnorm(x[s], rep(0, length(s)), sigma22) * pmnorm(x[-s], mu, sigma))
    }
  }
}

#smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/factor.txt", header = TRUE)
#U1 = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/u1.txt", header = TRUE)$U1
#realFirstTerms = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/firstTerms.txt", header = TRUE)$firstTerms
#realSecondTerms = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/secondTerms.txt", header = TRUE)$secondTerms

# Initial rho from program
#prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/parameters.txt", header = TRUE)
#rho1 = prm$rho0
#rho2 = prm$rho1

rho1 = -0.967133
rho2 = 0.920703

n = 2

# Load sample
smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesChain.txt", header = TRUE)
X1 = smp$X1
X2 = smp$X2
X3 = smp$X3

U1 = pnorm(X1, mean(X1), sd(X1))
U2 = pnorm(X2, mean(X2), sd(X2))
U3 = pnorm(X3, mean(X3), sd(X3))

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

C = dW1dV1 * dW2dV2 * dW3dV3 * dV1dU1 * dV2dU2 * dV3dU3 * dU1dX1 * dU2dX2 * dU3dX3

logLikelihood = function(rho1, rho2)
{
  cov1 = diag(n) + rho1 * (matrix(1, n, n) - diag(n))
  cov2 = diag(n) + rho2 * (matrix(1, n, n) - diag(n))
  
  f1 = partialDeriv(cbind(W1, W2), c(1, 2), cov1)

  dF1dW1 = partialDeriv(cbind(W1, W2), c(1), cov1)

  dF2dW3 = partialDeriv(cbind(W2, W3), c(2), cov2)

  f2 = partialDeriv(cbind(W2, W3), c(1, 2), cov2)

  f = C * (f1 * dF2dW3 + dF1dW1 * f2)
  den = (f1 * dF2dW3 + dF1dW1 * f2)
  
#   print(length(f1))
#   print(length(f2))
#   print(length(dF2dW3))
#   print(length(dF1dW1))
#   print(length(f))
  #print(-log(den[1]) / length(X1))

  precisionZ1 = solve(cov1) %*% rbind(W1, W2)
  precisionZ2 = solve(cov2) %*% rbind(W2, W3)

  df1dW1dW2 = f1 * (precisionZ1[1,] * precisionZ1[2,] - solve(cov1)[1,2])
  df2dW2dW3 = f2 * (precisionZ2[1,] * precisionZ2[2,] - solve(cov2)[1,2])
                  
  df1dW1 = f1 * -precisionZ1[1,]
  df2dW3 = f2 * -precisionZ2[2,]

  dfdRho1 = C * (df1dW1dW2 * dF2dW3 + df1dW1 * f2)
  dfdRho2 = C * (f1 * df2dW3 + dF1dW1 * df2dW2dW3)

  grad = cbind((df1dW1dW2 * dF2dW3 + df1dW1 * f2), (f1 * df2dW3 + dF1dW1 * df2dW2dW3))
  
  dlnfdRho1 = dfdRho1 / f
  dlnfdRho2 = dfdRho2 / f

  logGrad = -c(sum(dlnfdRho1), sum(dlnfdRho2)) / length(X1)
  logLike = -sum(log(den)) / length(X1)
  
  #print(c("logLike", logLike, "logGrad", logGrad))
  
  return (c(logLike, logGrad))
}

justLogLikelihood = function(rho1, rho2)
{
  cov1 = diag(n) + rho1 * (matrix(1, n, n) - diag(n))
  cov2 = diag(n) + rho2 * (matrix(1, n, n) - diag(n))
  
  f1 = partialDeriv(cbind(W1, W2), c(1, 2), cov1)
  
  dF1dW1 = partialDeriv(cbind(W1, W2), c(1), cov1)
  
  dF2dW3 = partialDeriv(cbind(W2, W3), c(2), cov2)
  
  f2 = partialDeriv(cbind(W2, W3), c(1, 2), cov2)
  
  den = (f1 * dF2dW3 + dF1dW1 * f2)
  
  logLike = -sum(log(den)) / length(X1)
  
  return (logLike)
}

rhoGrid = function(n)
{
  p = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  #q = seq(1, -1, 2 / (n - 1))
  
  return (expand.grid(p, p))
}

z = scan(file = "C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/gridDebug.txt")
z = matrix(z, sqrt(length(z)), sqrt(length(z)))

plotContours = function(n)
{
  x = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  y = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  contour(x, y, z, levels = c(3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4, 4.1, 4.2, 4.3, 4.5, 4.7, 5, 6, 10), cex = 2.0)
  #filled.contour(x, y, z)
  
  #print(x)
  #print(y)
  #print(z)
}

printSolution = function(fileName, colour = "red")
{
  prm = read.table(fileName, header = TRUE)
  rho1 = prm$rho0
  rho2 = prm$rho1
  
  s = as.character(1:length(rho1))
  
  #for (i in 1:length(rho1)) {
  #  text(rho1[i], rho2[i], s[i], col = colour)
  #}
  
  lines(rho1, rho2, type = "o", lty = 2, col = colour)
}

printArrows = function(pointsFile, directionFile, colour = "blue")
{
  prm = read.table(pointsFile, header = TRUE)
  rho1 = prm$rho0
  rho2 = prm$rho1
  
  srch = read.table(directionFile, header = TRUE)
  srch = t(scale(t(srch), center = FALSE))
  srchX = -srch[,1]/5
  srchY = -srch[,2]/5
  
  arrows(rho1, rho2, rho1 + srchX, rho2 + srchY, col = colour)
}

plotContours(sqrt(length(z)))
printSolution("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/solnChainBFGS.txt")
printArrows("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/solnChainBFGS.txt", "C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/gradChainBFGS.txt")

prm = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/debugLearning.txt", header = TRUE)

compareLogGrad = function(i) {
  print(c(prm$obj[i], prm$dRho1[i], prm$dRho2[i]) - logLikelihood(prm$rho1[i], prm$rho2[i]))
}



#for (i in 1:length(prm$obj)) {
#  compareLogGrad(i)
#}