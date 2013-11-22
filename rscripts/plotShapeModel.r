## Plot marginals and factors of shape model

path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\"  # experiments\\
setwd(path)

createMarginalsPlots = function() {

  # Height and width of plot in inches
  plotHeight = 5.5
  plotWidth = 5

  # Open PDF for graph output
  pdf(paper="special", file="shapeMarginalsHeat.pdf", height = plotHeight, width = plotWidth)

  # Save plotting parameters
  op = par(no.readonly = TRUE)
  par(pin=c(plotWidth, plotHeight), mai=c(0.5,0,0,0), omi=rep(0,4), xpd = NA)

  plot.new()

  marginals = scan("shapeMarginals.txt")
  marginals = marginals[2:length(marginals)]
  marginals = matrix(marginals, length(marginals) / 2, 2, byrow = TRUE)
  pixelValues = apply(marginals, 1, function(x) { 1 - pnorm(x[1], x[2]) })

  # Define the offset of the pixel array and the size of a pixel in inches
  cornerX = 0
  cornerY = 0
  widthPixel = 1 / 32
  heightPixel = 1 / 32

  # Function that converts pixel coordinate to a box of plot coordinates
  pixelBox = function(i, j)
  {
    c(cornerX + (i-1) * widthPixel, 1 - cornerY - (j-1) * heightPixel, cornerX + i * widthPixel, 1 - cornerY - j * heightPixel)
  }

  for (j in 1:32) {
    for (i in 1:32) {
      pixel = pixelBox(i, j)
      deviat = abs(pixelValues[i + (j - 1) * 32] - 0.5) / 0.5
    
      s = 0.75
    
      if (pixelValues[i + (j - 1) * 32] > 0.5) {
        pixelCol = rgb((1 - deviat * s), (1 - deviat * s), 1)
      }
      else {
        pixelCol = rgb((1 - deviat * s), 1, (1 - deviat * s))
      }      
    
      rect(pixel[1], pixel[2], pixel[3], pixel[4], col = grey(pixelValues[i + (j - 1) * 32]), border = grey(0.75))    # pixelCol
    }
  }
  
  legend(0.5, -0.035, xjust = 0.5, legend = expression(plain(P)(X[ij] == 1) == 0, plain(P)(X[ij] == 1) == 0.5, plain(P)(X[ij] == 1) == 1), horiz = TRUE, fill = c(grey(1), grey(0.5), grey(0)))  # rgb(0, 1, 0), rgb(1, 1, 1), rgb(0, 0, 1)
  
  # Close device
  dev.off()
  
  # Open PDF for graph output
  pdf(paper="special", file="shapeMarginalsHard.pdf", height = plotHeight, width = plotWidth)
  
  # Save plotting parameters
  op = par(no.readonly = TRUE)
  par(pin=c(plotWidth, plotHeight), mai=c(0.5,0,0,0), omi=rep(0,4), xpd = NA)
  
  plot.new()
  
  for (j in 1:32) {
    for (i in 1:32) {
      pixel = pixelBox(i, j)
         
      if (pixelValues[i + (j - 1) * 32] > 0.5) {
        pixelCol = grey(1)
      }
      else {
        pixelCol = grey(0)
      }      
      
      rect(pixel[1], pixel[2], pixel[3], pixel[4], col = pixelCol, border = grey(0.75))
    }
  }
  
  legend(0.5, -0.035, xjust = 0.5, legend = expression(plain(P)(X[ij] == 1) <= 0.5, plain(P)(X[ij] == 1) > 0.5), horiz = TRUE, fill = c(rgb(1, 1, 1), rgb(0, 0, 0)))
  
  dev.off()
  
  par(op)
  
  # Open PDF for graph output
  pdf(paper="special", file="shapeMarginalsSd.pdf", height = plotHeight, width = plotWidth)
  
  # Save plotting parameters
  op = par(no.readonly = TRUE)
  par(pin=c(plotWidth, plotHeight), mai=c(0.5,0,0,0), omi=rep(0,4), xpd = NA)
  
  plot.new()
  
  pixelSd = marginals[, 2]
  
  maxSd = max(pixelSd)
  minSd = min(pixelSd)
  rangeSd = maxSd - minSd
  
  for (j in 1:32) {
    for (i in 1:32) {
      pixel = pixelBox(i, j)
      
      pixelCol = grey(1 - (pixelSd[i + (j - 1) * 32] - minSd) / rangeSd)
      
      rect(pixel[1], pixel[2], pixel[3], pixel[4], col = pixelCol, border = grey(0.75))
    }
  }
  
  legend(0.5, -0.035, xjust = 0.5, legend = eval(substitute(expression(sigma[ij] == a, sigma[ij] == b, sigma[ij] == c), list(a = round(minSd, digits = 2), b = round(minSd + 0.5 * rangeSd, digits = 4), c = round(maxSd, digits = 4)))), horiz = TRUE, fill = c(grey(1), grey(0.5), grey(0)))
  
  dev.off()
  
  par(op)
}

createFactorsPlots = function(inputFile, outputLabel, type = "normal")
{
  factors = scan(inputFile)
  numFactors = factors[1]
  
  print(numFactors)
  
  vars = factors[2:((numFactors * 3) + 1)]
  vars = matrix(vars, ncol = 3, byrow = TRUE)
  vars = vars[,c(2,3)]
  
  fPars = factors[((numFactors * 3) + 2):((numFactors * 4) + 1)]
  fLevels = factors[((numFactors * 4) + 2):((numFactors * 5) + 1)]
  
  varPars = cbind(vars, fPars)
  
  # Height and width of plot in inches
  xSep = 0.25
  ySep = 0.25
  plotHeight = 5.75
  plotWidth = 10
  
  mainWidth = 5
  mainHeight = 5
  subWidth = 2.5
  subHeight = 2.5  
  
  lineMin = 0.01
  lineMax = 3
  
  pdf(paper="special", file = paste(outputLabel, ".pdf", sep = ""), height = plotHeight, width = plotWidth)
  
  layout(matrix(c(1,2,3,1,4,5), 2, 3, byrow = TRUE), widths = c(1, 0.5, 0.5), heights = c(2.5, 3.25))
  
  # Save plotting parameters
  op = par(no.readonly = TRUE)
    
  #matrix(c(1,2,3,1,4,5), 2, 3, byrow = TRUE)
  
  # Open PDF for graph output
  plotLevel = function(l, f) {
    
    #testRows = varPars[fLevels == l, ]
    #testRows = testRows[1:3, ]
    #print(testRows)
    
    s = 1
    
    if (l == 0) {
      cornerX = 0 #mainWidth / plotWidth + xSep / plotWidth
      cornerY = 0 #subHeight / plotHeight + ySep / plotHeight
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 1) {
      cornerX = 0 #mainWidth / plotWidth + 2 * xSep / plotWidth + subWidth / plotWidth
      cornerY = 0 #subHeight / plotHeight + ySep / plotHeight
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 2) {
      cornerX = 0 #mainWidth / plotWidth + xSep / plotWidth
      cornerY = 0
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 3) {
      cornerX = 0 #mainWidth / plotWidth + 2 * xSep / plotWidth + subWidth / plotWidth
      cornerY = 0
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 4) {
      cornerX = 0
      cornerY = 0
      widthPixel = 1 / 32 #1 / 32 * mainWidth / plotWidth
      heightPixel = 1 / 32 #1 / 32 * mainHeight / plotHeight
    }
    
    # Function that converts pixel coordinate to a box of plot coordinates
    pixelBox = function(i, j)
    {
      c(cornerX + (i-1) * widthPixel, 1 - cornerY - (j-1) * heightPixel, cornerX + i * widthPixel, 1 - cornerY - j * heightPixel)
    }
    
    plotGrid = function() {
      for (j in 1:32) {
        for (i in 1:32) {
          pixel = pixelBox(i, j)
          rect(pixel[1], pixel[2], pixel[3], pixel[4], border = grey(0.75))
        }
      } 
    }
    
    plotGrid()
    
    plotClaytonFactor = function(x)
    {
      pixelA = pixelBox((x[1] %% 32) + 1, floor(x[1] / 32) + 1)
      pixelB = pixelBox((x[2] %% 32) + 1, floor(x[2] / 32) + 1)
      tau = x[3] / (2 + x[3])
      
      lines(c((pixelA[1] + pixelA[3]) / 2, (pixelB[1] + pixelB[3]) / 2), 
            c((pixelA[2] + pixelA[4]) / 2, (pixelB[2] + pixelB[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
    }
    
    plotNormalFactor = function(x)
    {
      pixelA = pixelBox((x[1] %% 32) + 1, floor(x[1] / 32) + 1)
      pixelB = pixelBox((x[2] %% 32) + 1, floor(x[2] / 32) + 1)
      
      rho = abs(x[3])
      
      if (x[3] > 0) {
        pixelCol = rgb(0, 1, 0)
      }
      else {
        pixelCol = rgb(0, 0, 1) 
      }
      
      lines(c((pixelA[1] + pixelA[3]) / 2, (pixelB[1] + pixelB[3]) / 2), 
            c((pixelA[2] + pixelA[4]) / 2, (pixelB[2] + pixelB[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
    }
    
    if (type == "clayton") {
      apply(varPars[fLevels == l, ], 1, plotClaytonFactor)
    }

    else if (type == "normal") {
      apply(varPars[fLevels == l, ], 1, plotNormalFactor)
    }
  }
  
  par(mai = c(0.75,0,0,0), omi = c(0,0,0,0), xpd = NA)
    
  for (i in 4:0) {
    if (i == 1 || i == 0) {
      par(mai = c(0.75,0,0,0), xpd = NA)
    }
    
    else if (i == 2 || i == 3) {
      par(mai = c(0,0,0,0), xpd = NA)
    }
    
    plot.new()
    plotLevel(i)
    
    if (i == 4) {
      if (type == "clayton") {
        legend(1, -0.075, xjust = 0.5, cex = 1.5, legend = expression(tau == 0, tau == 0.5, tau == 1), horiz = TRUE, lwd = c(lineMin, (lineMin + lineMax)/2, lineMax))
      }
      else if (type == "normal") {
        legend(1, -0.075, xjust = 0.5, cex = 1.5, legend = expression(rho == -1, rho == -0.25, rho == 0.25, rho == 1), horiz = TRUE, lwd = c(lineMax, (lineMin + 0.25 * (lineMax- lineMin)), (lineMin + 0.25 * (lineMax- lineMin)), lineMax), col = c(rgb(0, 0, 1), rgb(0, 0, 1), rgb(0, 1, 0), rgb(0, 1, 0)))
      }
    }
  }
  #plotLevel(4)
  
  #rect(0, 0, 0.1, 0.1, col = grey(0))
    
  dev.off() 
  par(op)
}

createFourFactorsPlots = function(inputFile, outputLabel, type = "normal")
{
  factors = scan(inputFile)
  numFactors = factors[1]
  
  print(numFactors)
  
  vars = factors[2:((numFactors * 5) + 1)]
  vars = matrix(vars, ncol = 5, byrow = TRUE)
  vars = vars[,2:5]
  
  fPars = factors[((numFactors * 5) + 2):((numFactors * 6) + 1)]
  fLevels = factors[((numFactors * 6) + 2):((numFactors * 7) + 1)]
  
  print(sum(fPars < 0))
  
  varPars = cbind(vars, fPars)
  
  # Height and width of plot in inches
  plotHeight = 5.75
  plotWidth = 10
  
  lineMin = 0.01
  lineMax = 3
  
  pdf(paper="special", file = paste(outputLabel, ".pdf", sep = ""), height = plotHeight, width = plotWidth)
  
  layout(matrix(c(1,2,3,1,4,5), 2, 3, byrow = TRUE), widths = c(1, 0.5, 0.5), heights = c(2.5, 3.25))
  
  # Save plotting parameters
  op = par(no.readonly = TRUE)
  
  #matrix(c(1,2,3,1,4,5), 2, 3, byrow = TRUE)
  
  # Open PDF for graph output
  plotLevel = function(l, f) {
    
    #testRows = varPars[fLevels == l, ]
    #testRows = testRows[1:3, ]
    #print(testRows)
    
    s = 1
    
    if (l == 0) {
      cornerX = 0 #mainWidth / plotWidth + xSep / plotWidth
      cornerY = 0 #subHeight / plotHeight + ySep / plotHeight
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 1) {
      cornerX = 0 #mainWidth / plotWidth + 2 * xSep / plotWidth + subWidth / plotWidth
      cornerY = 0 #subHeight / plotHeight + ySep / plotHeight
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 2) {
      cornerX = 0 #mainWidth / plotWidth + xSep / plotWidth
      cornerY = 0
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 3) {
      cornerX = 0 #mainWidth / plotWidth + 2 * xSep / plotWidth + subWidth / plotWidth
      cornerY = 0
      widthPixel = 1 / 32 #* subWidth / plotWidth
      heightPixel = 1 / 32 #* subHeight / plotHeight
    }
    
    else if (l == 4) {
      cornerX = 0
      cornerY = 0
      widthPixel = 1 / 32 #1 / 32 * mainWidth / plotWidth
      heightPixel = 1 / 32 #1 / 32 * mainHeight / plotHeight
    }
    
    # Function that converts pixel coordinate to a box of plot coordinates
    pixelBox = function(i, j)
    {
      c(cornerX + (i-1) * widthPixel, 1 - cornerY - (j-1) * heightPixel, cornerX + i * widthPixel, 1 - cornerY - j * heightPixel)
    }
    
    plotGrid = function() {
      for (j in 1:32) {
        for (i in 1:32) {
          pixel = pixelBox(i, j)
          rect(pixel[1], pixel[2], pixel[3], pixel[4], border = grey(0.75))
        }
      } 
    }
    
    plotGrid()
    
    plotClaytonFactor = function(x)
    {
      pixelA = pixelBox((x[1] %% 32) + 1, floor(x[1] / 32) + 1)
      pixelB = pixelBox((x[2] %% 32) + 1, floor(x[2] / 32) + 1)
      pixelC = pixelBox((x[3] %% 32) + 1, floor(x[3] / 32) + 1)
      pixelD = pixelBox((x[4] %% 32) + 1, floor(x[4] / 32) + 1)
      tau = x[5] / (2 + x[5])
      
      lines(c((pixelA[1] + pixelA[3]) / 2, (pixelB[1] + pixelB[3]) / 2), 
            c((pixelA[2] + pixelA[4]) / 2, (pixelB[2] + pixelB[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
      
      lines(c((pixelB[1] + pixelB[3]) / 2, (pixelC[1] + pixelC[3]) / 2), 
            c((pixelB[2] + pixelB[4]) / 2, (pixelC[2] + pixelC[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
      
      lines(c((pixelC[1] + pixelC[3]) / 2, (pixelD[1] + pixelD[3]) / 2), 
            c((pixelC[2] + pixelC[4]) / 2, (pixelD[2] + pixelD[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
      
      lines(c((pixelD[1] + pixelD[3]) / 2, (pixelA[1] + pixelA[3]) / 2), 
            c((pixelD[2] + pixelD[4]) / 2, (pixelA[2] + pixelA[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
      
      
      lines(c((pixelA[1] + pixelA[3]) / 2, (pixelC[1] + pixelC[3]) / 2), 
            c((pixelA[2] + pixelA[4]) / 2, (pixelC[2] + pixelC[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
      
      lines(c((pixelB[1] + pixelB[3]) / 2, (pixelD[1] + pixelD[3]) / 2), 
            c((pixelB[2] + pixelB[4]) / 2, (pixelD[2] + pixelD[4]) / 2), 
            lwd = lineMin + tau * (lineMax - lineMin))
    }
    
    plotNormalFactor = function(x)
    {
      pixelA = pixelBox((x[1] %% 32) + 1, floor(x[1] / 32) + 1)
      pixelB = pixelBox((x[2] %% 32) + 1, floor(x[2] / 32) + 1)
      pixelC = pixelBox((x[3] %% 32) + 1, floor(x[3] / 32) + 1)
      pixelD = pixelBox((x[4] %% 32) + 1, floor(x[4] / 32) + 1)
      
      rho = abs(x[5])
      
      if (x[5] > 0) {
        pixelCol = rgb(0, 1, 0)
      }
      else {
        pixelCol = rgb(0, 0, 1) 
      }
      
      lines(c((pixelA[1] + pixelA[3]) / 2, (pixelB[1] + pixelB[3]) / 2), 
            c((pixelA[2] + pixelA[4]) / 2, (pixelB[2] + pixelB[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
      
      lines(c((pixelB[1] + pixelB[3]) / 2, (pixelC[1] + pixelC[3]) / 2), 
            c((pixelB[2] + pixelB[4]) / 2, (pixelC[2] + pixelC[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
      
      lines(c((pixelC[1] + pixelC[3]) / 2, (pixelD[1] + pixelD[3]) / 2), 
            c((pixelC[2] + pixelC[4]) / 2, (pixelD[2] + pixelD[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
      
      lines(c((pixelD[1] + pixelD[3]) / 2, (pixelA[1] + pixelA[3]) / 2), 
            c((pixelD[2] + pixelD[4]) / 2, (pixelA[2] + pixelA[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
      
      
      lines(c((pixelA[1] + pixelA[3]) / 2, (pixelC[1] + pixelC[3]) / 2), 
            c((pixelA[2] + pixelA[4]) / 2, (pixelC[2] + pixelC[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
      
      lines(c((pixelB[1] + pixelB[3]) / 2, (pixelD[1] + pixelD[3]) / 2), 
            c((pixelB[2] + pixelB[4]) / 2, (pixelD[2] + pixelD[4]) / 2), 
            lwd = lineMin + rho * (lineMax - lineMin), col = pixelCol)
    }
    
    if (type == "clayton") {
      if (is.matrix(varPars[fLevels == l, ]) == FALSE) {
        plotClaytonFactor(varPars[fLevels == l, ])
      }
      
      else {
        apply(varPars[fLevels == l, ], 1, plotClaytonFactor)
      }
    }
    
    else if (type == "normal") {
      if (is.matrix(varPars[fLevels == l, ]) == FALSE) {
        plotNormalFactor(varPars[fLevels == l, ])
      }
      
      else {
        apply(varPars[fLevels == l, ], 1, plotNormalFactor)
      }
    }
  }
  
  par(mai = c(0.75,0,0,0), omi = c(0,0,0,0), xpd = NA)
  
  for (i in 4:0) {
    if (i == 1 || i == 0) {
      par(mai = c(0.75,0,0,0), xpd = NA)
    }
    
    else if (i == 2 || i == 3) {
      par(mai = c(0,0,0,0), xpd = NA)
    }
    
    plot.new()
    plotLevel(i)
    
    if (i == 4) {
      if (type == "clayton") {
        legend(1, -0.075, xjust = 0.5, cex = 1.5, legend = expression(tau == 0, tau == 0.5, tau == 1), horiz = TRUE, lwd = c(lineMin, (lineMin + lineMax)/2, lineMax))
      }
      else if (type == "normal") {
        legend(1, -0.075, xjust = 0.5, cex = 1.5, legend = expression(rho == -1, rho == -0.25, rho == 0.25, rho == 1), horiz = TRUE, lwd = c(lineMax, (lineMin + 0.25 * (lineMax- lineMin)), (lineMin + 0.25 * (lineMax- lineMin)), lineMax), col = c(rgb(0, 0, 1), rgb(0, 0, 1), rgb(0, 1, 0), rgb(0, 1, 0)))
      }
    }
  }
  #plotLevel(4)
  
  #rect(0, 0, 0.1, 0.1, col = grey(0))
  
  dev.off() 
  par(op)
}

printSamples = function(filename, outputLabel)
{
  # Define the offset of the pixel array and the size of a pixel in inches
  cornerX = 0
  cornerY = 0
  widthPixel = 1 / 32
  heightPixel = 1 / 32
  
  # Function that converts pixel coordinate to a box of plot coordinates
  pixelBox = function(i, j)
  {
    c(cornerX + (i-1) * widthPixel, 1 - cornerY - (j-1) * heightPixel, cornerX + i * widthPixel, 1 - cornerY - j * heightPixel)
  } 
  
  # Height and width of plot in inches
  plotHeight = 10
  plotWidth = 10
  
  samples = read.table(filename, header = TRUE)
  
  pdf(paper="special", file = paste(outputLabel, ".pdf", sep = ""), height = plotHeight, width = plotWidth)
  
  layout(matrix(c(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), 4, 4, byrow = TRUE), widths = rep(1, 4), heights = rep(1, 4))
  
  par(mai=c(0,0,0,0), omi=rep(0,4), xpd = NA)
  
  for (k in 1:min(nrow(samples), 16)) {
    plot.new()
    
    for (j in 1:32) {
      for (i in 1:32) {
        pixel = pixelBox(i, j)
        if (samples[k, i + (j - 1) * 32] > 0) {
          pixelCol = grey(0)
        }
        else {
          pixelCol = grey(1)
        }
        
        rect(pixel[1], pixel[2], pixel[3], pixel[4], border = NA, col = pixelCol, lwd = 0.5)  # grey(0.75)
      }
    }
  }
  
  dev.off()
}

printDatasetSamples = function(filename, outputLabel)
{
  # Define the offset of the pixel array and the size of a pixel in inches
  cornerX = 0
  cornerY = 0
  widthPixel = 1 / 32
  heightPixel = 1 / 32
  
  # Function that converts pixel coordinate to a box of plot coordinates
  pixelBox = function(i, j)
  {
    c(cornerX + (i-1) * widthPixel, 1 - cornerY - (j-1) * heightPixel, cornerX + i * widthPixel, 1 - cornerY - j * heightPixel)
  } 
  
  # Height and width of plot in inches
  plotHeight = 1
  plotWidth = 8
  
  samples = read.table(filename, header = TRUE)
  
  pdf(paper="special", file = paste(outputLabel, ".pdf", sep = ""), height = plotHeight, width = plotWidth)
  
  layout(matrix(c(1,2,3,4,5,6,7,8), 1, 8, byrow = TRUE), widths = rep(1, 8), heights = rep(1, 1))
  
  par(mai=c(0,0,0,0), omi=rep(0,4), xpd = NA)
  
  for (k in 1:min(nrow(samples), 8)) {
    plot.new()
    
    for (j in 1:32) {
      for (i in 1:32) {
        pixel = pixelBox(i, j)
        if (samples[k, i + (j - 1) * 32] > 0) {
          pixelCol = grey(0)
        }
        else {
          pixelCol = grey(1)
        }
        
        rect(pixel[1], pixel[2], pixel[3], pixel[4], border = NA, col = pixelCol, lwd = 0.5)  # grey(0.75)
      }
    }
  }
  
  dev.off()
}

# expression(plain(P)(X==x)==1)

#createMarginalsPlots()
#printDatasetSamples("shapeSamplesDataset.txt", "shapeSamplesDataset")

# ## Create sample plots for independent pixels
# printSamples("shapeIndSamples.txt", "shapeIndSamples")
# 
# ## Create sample and factor plots for mistake shape model with clayton copulae
# printSamples("shapeSamplesMistakeClayton_1.txt", "shapeSamplesMistakeClayton")
# createFactorsPlots("shapeFactorsMistakeClayton_1.txt", "shapeFactorsMistakeClayton", "clayton")
# 
# ## Create sample and factor plots for mistake shape model with normal copulae
# printSamples("shapeSamplesMistakeNormal.txt", "shapeSamplesMistakeNormal")
# createFactorsPlots("shapeFactorsMistakeNormal.txt", "shapeFactorsMistakeNormal", "normal")
# 
# ## Create sample and factor plots for square shape model with clayton copulae
# printSamples("shapeSamplesSquareClayton.txt", "shapeSamplesSquareClayton")
# createFactorsPlots("shapeFactorsSquareClayton.txt", "shapeFactorsSquareClayton", "clayton")
# 
# ## Create sample and factor plots for square shape model with normal copulae
# printSamples("shapeSamplesSquareNormal.txt", "shapeSamplesSquareNormal")
# createFactorsPlots("shapeFactorsSquareNormal.txt", "shapeFactorsSquareNormal", "normal")
#  
# ## Create sample and factor plots for diagonal shape model with clayton copulae
# printSamples("shapeSamplesDiagonalClayton.txt", "shapeSamplesDiagonalClayton")
# createFactorsPlots("shapeFactorsDiagonalClayton.txt", "shapeFactorsDiagonalClayton", "clayton")
#  
# ## Create sample and factor plots for diagonal shape model with normal copulae
printSamples("shapeSamplesDiagonalNormal_1.txt", "simpleShapeSamples")
createFactorsPlots("shapeFactorsDiagonalNormal_1.txt", "simpleShapeFactors", "normal")
# 
# ## Create sample and factor plots for four shape model with clayton copulae
# printSamples("shapeSamplesFourClayton.txt", "shapeSamplesFourClayton")
# createFourFactorsPlots("shapeFactorsFourClayton.txt", "shapeFactorsFourClayton", "clayton")

## Create sample and factor plots for four shape model with clayton copulae
#printSamples("shapeSamplesFourNormal.txt", "shapeSamplesFourNormal")
#createFourFactorsPlots("shapeFactorsFourNormal.txt", "shapeFactorsFourNormal", "normal")

#system("open shapeFactors4.pdf")