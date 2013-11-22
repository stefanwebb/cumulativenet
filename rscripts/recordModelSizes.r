# This script takes in data from drwnTestCases::recordModelSizes() and makes figures showing how maximum clique size and total number of variables increases as model size increases for test models
path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"
setwd(path)

inputVals = function(fileName) {
  vals = scan(fileName)
}

doFigure = function(maxFile, totalFile, title, offset, numYaxis = 5)
{
  maxCliques = inputVals(maxFile)
  totalSize = inputVals(totalFile)
  
  if (title == "Chains" || title == "Loops") {
    x = (1:length(totalSize) + offset)
  }
  
  else if (title == "Grids") {
    x = (1:length(totalSize) + offset)^2
  }
  
  else if (title == "Trees") {
    x = 2^(1:length(totalSize) + offset) - 1
  }

  par(mfrow = c(1, 1), oma = c(0, 0, 0, 0), mai = c(1, 1.0, 0.25, 0.25))
  plot(x, maxCliques, type = "b", xlab = "variables", ylab = "largest clique", lty = 2) # , yaxt = "n"
    
  #axis(1, at=seq(1, length(maxCliques), 2) + offset, labels=seq(1, length(maxCliques), 2) + offset)
  #axis(2, at=round(seq(from = min(maxCliques), to = max(maxCliques), length.out = 5)), labels=round(seq(from = min(maxCliques), to = max(maxCliques), length.out = 5)))
  
  #plot(1:length(totalSize) + offset, totalSize, lty = 2, pch = 1, type = "b", xlab = "n", ylab = "total size", xaxt = "n", yaxt = "n")
  #axis(1, at=seq(1, length(totalSize), 2) + offset, labels=seq(1, length(totalSize), 2) + offset)
  #axis(2, at=seq(from = min(totalSize), to = max(totalSize), length.out = numYaxis), labels = seq(from = min(totalSize), to = max(totalSize), length.out = numYaxis))

  #mtext(title, outer = TRUE, cex = 1.5, line = -1.5)

  par(mfrow = c(1, 1), oma = c(0, 0, 0, 0))
}

doFigure("chainCliques.txt", "chainTotal.txt", "Chains", 1)
doFigure("loopCliques.txt", "loopTotal.txt", "Loops", 2)
doFigure("treeCliques.txt", "treeTotal.txt", "Trees", 1, 3)
doFigure("gridCliques.txt", "gridTotal.txt", "Grids", 1)