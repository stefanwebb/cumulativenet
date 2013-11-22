path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"

## Read Data

normalChain2 = read.table(paste(path, "inferenceNormalChain2_1.txt", sep =""))
normalChain3 = read.table(paste(path, "inferenceNormalChain3.txt", sep =""))
normalChain4 = read.table(paste(path, "inferenceNormalChain4.txt", sep =""))
normalChain5 = read.table(paste(path, "inferenceNormalChain5.txt", sep =""))
normalChain6 = read.table(paste(path, "inferenceNormalChain6.txt", sep =""))
normalChain7 = read.table(paste(path, "inferenceNormalChain7.txt", sep =""))
normalChain8 = read.table(paste(path, "inferenceNormalChain8.txt", sep =""))
normalChain9 = read.table(paste(path, "inferenceNormalChain9.txt", sep =""))
normalChain10 = read.table(paste(path, "inferenceNormalChain10.txt", sep =""))
normalChain11 = read.table(paste(path, "inferenceNormalChain11.txt", sep =""))
normalChain12 = read.table(paste(path, "inferenceNormalChain12.txt", sep =""))
normalChain13 = read.table(paste(path, "inferenceNormalChain13.txt", sep =""))
normalChain14 = read.table(paste(path, "inferenceNormalChain14.txt", sep =""))
normalChain15 = read.table(paste(path, "inferenceNormalChain15.txt", sep =""))

normalChainMeans = c(mean(normalChain2[,1]), mean(normalChain3[,1]), mean(normalChain4[,1]), mean(normalChain5[,1]), mean(normalChain6[,1]), mean(normalChain7[,1]), mean(normalChain8[,1]), mean(normalChain9[,1]), mean(normalChain10[,1]), mean(normalChain11[,1]), mean(normalChain12[,1]), mean(normalChain13[,1]), mean(normalChain14[,1]), mean(normalChain15[,1]))
normalChainMeansLogSpace = c(mean(normalChain2[,2]), mean(normalChain3[,2]), mean(normalChain4[,2]), mean(normalChain5[,2]), mean(normalChain6[,2]), mean(normalChain7[,2]), mean(normalChain8[,2]), mean(normalChain9[,2]), mean(normalChain10[,2]), mean(normalChain11[,2]), mean(normalChain12[,2]), mean(normalChain13[,2]), mean(normalChain14[,2]), mean(normalChain15[,2]))

normalLoop3 = read.table(paste(path, "inferenceNormalLoop3.txt", sep =""))
normalLoop4 = read.table(paste(path, "inferenceNormalLoop4.txt", sep =""))
normalLoop5 = read.table(paste(path, "inferenceNormalLoop5.txt", sep =""))
normalLoop6 = read.table(paste(path, "inferenceNormalLoop6.txt", sep =""))
normalLoop7 = read.table(paste(path, "inferenceNormalLoop7.txt", sep =""))
normalLoop8 = read.table(paste(path, "inferenceNormalLoop8.txt", sep =""))
normalLoop9 = read.table(paste(path, "inferenceNormalLoop9.txt", sep =""))
normalLoop10 = read.table(paste(path, "inferenceNormalLoop10.txt", sep =""))
normalLoop11 = read.table(paste(path, "inferenceNormalLoop11.txt", sep =""))
normalLoop12 = read.table(paste(path, "inferenceNormalLoop12.txt", sep =""))
normalLoop13 = read.table(paste(path, "inferenceNormalLoop13.txt", sep =""))
normalLoop14 = read.table(paste(path, "inferenceNormalLoop14.txt", sep =""))
normalLoop15 = read.table(paste(path, "inferenceNormalLoop15.txt", sep =""))

normalLoopMeans = c(mean(normalLoop3[,1]), mean(normalLoop4[,1]), mean(normalLoop5[,1]), mean(normalLoop6[,1]), mean(normalLoop7[,1]), mean(normalLoop8[,1]), mean(normalLoop9[,1]), mean(normalLoop10[,1]), mean(normalLoop11[,1]), mean(normalLoop12[,1]), mean(normalLoop13[,1]), mean(normalLoop14[,1]), mean(normalLoop15[,1]))
normalLoopMeansLogSpace = c(mean(normalLoop3[,2]), mean(normalLoop4[,2]), mean(normalLoop5[,2]), mean(normalLoop6[,2]), mean(normalLoop7[,2]), mean(normalLoop8[,2]), mean(normalLoop9[,2]), mean(normalLoop10[,2]), mean(normalLoop11[,2]), mean(normalLoop12[,2]), mean(normalLoop13[,2]), mean(normalLoop14[,2]), mean(normalLoop15[,2]))

normalTree3 = read.table(paste(path, "inferenceNormalTree3.txt", sep =""))
normalTree4 = read.table(paste(path, "inferenceNormalTree4.txt", sep =""))
normalTree5 = read.table(paste(path, "inferenceNormalTree5.txt", sep =""))
normalTree6 = read.table(paste(path, "inferenceNormalTree6.txt", sep =""))
normalTree7 = read.table(paste(path, "inferenceNormalTree7.txt", sep =""))
normalTree8 = read.table(paste(path, "inferenceNormalTree8.txt", sep =""))
normalTree9 = read.table(paste(path, "inferenceNormalTree9.txt", sep =""))
normalTree10 = read.table(paste(path, "inferenceNormalTree10.txt", sep =""))
normalTree11 = read.table(paste(path, "inferenceNormalTree11.txt", sep =""))
normalTree12 = read.table(paste(path, "inferenceNormalTree12.txt", sep =""))
normalTree13 = read.table(paste(path, "inferenceNormalTree13.txt", sep =""))

normalTreeMeans = c(mean(normalTree3[,1]), mean(normalTree4[,1]), mean(normalTree5[,1]), mean(normalTree6[,1]), mean(normalTree7[,1]), mean(normalTree8[,1]), mean(normalTree9[,1]), mean(normalTree10[,1]), mean(normalTree11[,1]), mean(normalTree12[,1]), mean(normalTree13[,1]))
normalTreeMeansLogSpace = c(mean(normalTree3[,2]), mean(normalTree4[,2]), mean(normalTree5[,2]), mean(normalTree6[,2]), mean(normalTree7[,2]), mean(normalTree8[,2]), mean(normalTree9[,2]), mean(normalTree10[,2]), mean(normalTree11[,2]), mean(normalTree12[,2]), mean(normalTree13[,2]))

normalGrid2 = read.table(paste(path, "inferenceNormalGrid2.txt", sep =""))
normalGrid3 = read.table(paste(path, "inferenceNormalGrid3.txt", sep =""))
normalGrid4 = read.table(paste(path, "inferenceNormalGrid4.txt", sep =""))
normalGrid5 = read.table(paste(path, "inferenceNormalGrid5.txt", sep =""))
normalGrid6 = read.table(paste(path, "inferenceNormalGrid6.txt", sep =""))
normalGrid7 = read.table(paste(path, "inferenceNormalGrid7.txt", sep =""))
normalGrid8 = read.table(paste(path, "inferenceNormalGrid8.txt", sep =""))
normalGrid9 = read.table(paste(path, "inferenceNormalGrid9.txt", sep =""))
normalGrid10 = read.table(paste(path, "inferenceNormalGrid10.txt", sep =""))

normalGridMeans = c(mean(normalGrid2[,1]), mean(normalGrid3[,1]), mean(normalGrid4[,1]), mean(normalGrid5[,1]), mean(normalGrid6[,1]), mean(normalGrid7[,1]), mean(normalGrid8[,1]), mean(normalGrid9[,1]), mean(normalGrid10[,1]))
normalGridMeansLogSpace = c(mean(normalGrid2[,2]), mean(normalGrid3[,2]), mean(normalGrid4[,2]), mean(normalGrid5[,2]), mean(normalGrid6[,2]), mean(normalGrid7[,2]), mean(normalGrid8[,2]), mean(normalGrid9[,2]), mean(normalGrid10[,2]))

claytonChain2 = read.table(paste(path, "inferenceClaytonChain2_1.txt", sep =""))
claytonChain3 = read.table(paste(path, "inferenceClaytonChain3_1.txt", sep =""))
claytonChain4 = read.table(paste(path, "inferenceClaytonChain4_1.txt", sep =""))
claytonChain5 = read.table(paste(path, "inferenceClaytonChain5.txt", sep =""))
claytonChain6 = read.table(paste(path, "inferenceClaytonChain6.txt", sep =""))
claytonChain7 = read.table(paste(path, "inferenceClaytonChain7.txt", sep =""))
claytonChain8 = read.table(paste(path, "inferenceClaytonChain8.txt", sep =""))
claytonChain9 = read.table(paste(path, "inferenceClaytonChain9.txt", sep =""))
claytonChain10 = read.table(paste(path, "inferenceClaytonChain10.txt", sep =""))
claytonChain11 = read.table(paste(path, "inferenceClaytonChain11.txt", sep =""))
claytonChain12 = read.table(paste(path, "inferenceClaytonChain12.txt", sep =""))
claytonChain13 = read.table(paste(path, "inferenceClaytonChain13.txt", sep =""))
claytonChain14 = read.table(paste(path, "inferenceClaytonChain14.txt", sep =""))
claytonChain15 = read.table(paste(path, "inferenceClaytonChain15.txt", sep =""))

claytonChainMeans = c(mean(claytonChain2[,1]), mean(claytonChain3[,1]), mean(claytonChain4[,1]), mean(claytonChain5[,1]), mean(claytonChain6[,1]), mean(claytonChain7[,1]), mean(claytonChain8[,1]), mean(claytonChain9[,1]), mean(claytonChain10[,1]), mean(claytonChain11[,1]), mean(claytonChain12[,1]), mean(claytonChain13[,1]), mean(claytonChain14[,1]), mean(claytonChain15[,1]))
claytonChainMeansLogSpace = c(mean(claytonChain2[,2]), mean(claytonChain3[,2]), mean(claytonChain4[,2]), mean(claytonChain5[,2]), mean(claytonChain6[,2]), mean(claytonChain7[,2]), mean(claytonChain8[,2]), mean(claytonChain9[,2]), mean(claytonChain10[,2]), mean(claytonChain11[,2]), mean(claytonChain12[,2]), mean(claytonChain13[,2]), mean(claytonChain14[,2]), mean(claytonChain15[,2]))

claytonLoop3 = read.table(paste(path, "inferenceClaytonLoop3_2.txt", sep =""))
claytonLoop4 = read.table(paste(path, "inferenceClaytonLoop4_2.txt", sep =""))
claytonLoop5 = read.table(paste(path, "inferenceClaytonLoop5_1.txt", sep =""))
claytonLoop6 = read.table(paste(path, "inferenceClaytonLoop6_1.txt", sep =""))
claytonLoop7 = read.table(paste(path, "inferenceClaytonLoop7_1.txt", sep =""))
claytonLoop8 = read.table(paste(path, "inferenceClaytonLoop8_1.txt", sep =""))
claytonLoop9 = read.table(paste(path, "inferenceClaytonLoop9_1.txt", sep =""))
claytonLoop10 = read.table(paste(path, "inferenceClaytonLoop10_1.txt", sep =""))
claytonLoop11 = read.table(paste(path, "inferenceClaytonLoop11_1.txt", sep =""))
claytonLoop12 = read.table(paste(path, "inferenceClaytonLoop12_1.txt", sep =""))
claytonLoop13 = read.table(paste(path, "inferenceClaytonLoop13_1.txt", sep =""))
claytonLoop14 = read.table(paste(path, "inferenceClaytonLoop14_1.txt", sep =""))
claytonLoop15 = read.table(paste(path, "inferenceClaytonLoop15_1.txt", sep =""))

claytonLoopMeans = c(mean(claytonLoop3[,1]), mean(claytonLoop4[,1]), mean(claytonLoop5[,1]), mean(claytonLoop6[,1]), mean(claytonLoop7[,1]), mean(claytonLoop8[,1]), mean(claytonLoop9[,1]), mean(claytonLoop10[,1]), mean(claytonLoop11[,1]), mean(claytonLoop12[,1]), mean(claytonLoop13[,1]), mean(claytonLoop14[,1]), mean(claytonLoop15[,1]))
claytonLoopMeansLogSpace = c(mean(claytonLoop3[,2]), mean(claytonLoop4[,2]), mean(claytonLoop5[,2]), mean(claytonLoop6[,2]), mean(claytonLoop7[,2]), mean(claytonLoop8[,2]), mean(claytonLoop9[,2]), mean(claytonLoop10[,2]), mean(claytonLoop11[,2]), mean(claytonLoop12[,2]), mean(claytonLoop13[,2]), mean(claytonLoop14[,2]), mean(claytonLoop15[,2]))

claytonTree3 = read.table(paste(path, "inferenceClaytonTree3.txt", sep =""))
claytonTree4 = read.table(paste(path, "inferenceClaytonTree4.txt", sep =""))
claytonTree5 = read.table(paste(path, "inferenceClaytonTree5.txt", sep =""))
claytonTree6 = read.table(paste(path, "inferenceClaytonTree6.txt", sep =""))
claytonTree7 = read.table(paste(path, "inferenceClaytonTree7.txt", sep =""))
claytonTree8 = read.table(paste(path, "inferenceClaytonTree8.txt", sep =""))
claytonTree9 = read.table(paste(path, "inferenceClaytonTree9.txt", sep =""))
claytonTree10 = read.table(paste(path, "inferenceClaytonTree10.txt", sep =""))
claytonTree11 = read.table(paste(path, "inferenceClaytonTree11.txt", sep =""))
claytonTree12 = read.table(paste(path, "inferenceClaytonTree12.txt", sep =""))
claytonTree13 = read.table(paste(path, "inferenceClaytonTree13.txt", sep =""))

claytonTreeMeans = c(mean(claytonTree3[,1]), mean(claytonTree4[,1]), mean(claytonTree5[,1]), mean(claytonTree6[,1]), mean(claytonTree7[,1]), mean(claytonTree8[,1]), mean(claytonTree9[,1]), mean(claytonTree10[,1]), mean(claytonTree11[,1]), mean(claytonTree12[,1]), mean(claytonTree13[,1]))
claytonTreeMeansLogSpace = c(mean(claytonTree3[,2]), mean(claytonTree4[,2]), mean(claytonTree5[,2]), mean(claytonTree6[,2]), mean(claytonTree7[,2]), mean(claytonTree8[,2]), mean(claytonTree9[,2]), mean(claytonTree10[,2]), mean(claytonTree11[,2]), mean(claytonTree12[,2]), mean(claytonTree13[,2]))

claytonGrid2 = read.table(paste(path, "inferenceClaytonGrid2.txt", sep =""))
claytonGrid3 = read.table(paste(path, "inferenceClaytonGrid3.txt", sep =""))
claytonGrid4 = read.table(paste(path, "inferenceClaytonGrid4.txt", sep =""))
claytonGrid5 = read.table(paste(path, "inferenceClaytonGrid5.txt", sep =""))
claytonGrid6 = read.table(paste(path, "inferenceClaytonGrid6.txt", sep =""))
claytonGrid7 = read.table(paste(path, "inferenceClaytonGrid7.txt", sep =""))
claytonGrid8 = read.table(paste(path, "inferenceClaytonGrid8.txt", sep =""))
claytonGrid9 = read.table(paste(path, "inferenceClaytonGrid9.txt", sep =""))
claytonGrid10 = read.table(paste(path, "inferenceClaytonGrid10.txt", sep =""))

claytonGridMeans = c(mean(claytonGrid2[,1]), mean(claytonGrid3[,1]), mean(claytonGrid4[,1]), mean(claytonGrid5[,1]), mean(claytonGrid6[,1]), mean(claytonGrid7[,1]), mean(claytonGrid8[,1]), mean(claytonGrid9[,1]), mean(claytonGrid10[,1]))
claytonGridMeansLogSpace = c(mean(claytonGrid2[,2]), mean(claytonGrid3[,2]), mean(claytonGrid4[,2]), mean(claytonGrid5[,2]), mean(claytonGrid6[,2]), mean(claytonGrid7[,2]), mean(claytonGrid8[,2]), mean(claytonGrid9[,2]), mean(claytonGrid10[,2]))

maxMean = max(c(normalChainMeans, normalLoopMeans))
maxMean2 = max(c(normalGridMeans, normalTreeMeans))

maxClayMean = max(c(claytonChainMeans, claytonLoopMeans))
maxClayMean2 = max(c(claytonGridMeans, claytonTreeMeans))

par(mar = c(5.1, 4.1, 2.1, 2.1))

## Normal Chains

pdf(paper="special", file="infNormalChains.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot(rep(1:length(normalChainMeans), 2) + 1, 1 / c(normalChainMeans, normalChainMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points((1:length(normalChainMeans)) + 1, 1 / normalChainMeans,  type = "b", lty = 2)
points((1:length(normalChainMeansLogSpace)) + 1, 1 / normalChainMeansLogSpace, pch = 4, type = "b", lty = 2)
dev.off()

## Normal Loops

pdf(paper="special", file="infNormalLoops.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot(rep(1:length(normalLoopMeans) + 2, 2), 1 / c(normalLoopMeans, normalLoopMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points((1:length(normalLoopMeans)) + 2, 1 / normalLoopMeans,  type = "b", lty = 2)
points((1:length(normalLoopMeansLogSpace)) + 2, 1 / normalLoopMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

## Normal Trees

pdf(paper="special", file="infNormalTrees.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot(2 ^ (rep(1:length(normalTreeMeans), 2) + 2) - 1, 1 / c(normalTreeMeans, normalTreeMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points(2 ^ ((1:length(normalTreeMeans)) + 2) - 1, 1 / normalTreeMeans,  type = "b", lty = 2)
points(2 ^ ((1:length(normalTreeMeansLogSpace)) + 2) - 1, 1 / normalTreeMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

## Normal Grids

pdf(paper="special", file="infNormalGrids.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot((rep(1:length(normalGridMeans), 2) + 1)^2, 1 / c(normalGridMeans, normalGridMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points(((1:length(normalGridMeans)) + 1)^2, 1 / normalGridMeans,  type = "b", lty = 2)
points(((1:length(normalGridMeansLogSpace)) + 1)^2, 1 / normalGridMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

## Clayton Chains

pdf(paper="special", file="infClaytonChains.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot(rep(1:length(claytonChainMeans), 2) + 1, 1/ c(claytonChainMeans, claytonChainMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points((1:length(claytonChainMeans)) + 1, 1 / claytonChainMeans,  type = "b", lty = 2)
points((1:length(claytonChainMeansLogSpace)) + 1, 1 / claytonChainMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

# Clayton Loops

pdf(paper="special", file="infClaytonLoops.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot(rep(1:length(claytonLoopMeans) + 2, 2), 1/ c(claytonLoopMeans, claytonLoopMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points((1:length(claytonLoopMeans)) + 2, 1 / claytonLoopMeans,  type = "b", lty = 2)
points((1:length(claytonLoopMeansLogSpace)) + 2, 1 / claytonLoopMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

## Clayton Trees

pdf(paper="special", file="infClaytonTrees.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot(2^(rep(1:length(claytonTreeMeans), 2) + 2) - 1, 1 / c(claytonTreeMeans, claytonTreeMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points(2^((1:length(claytonTreeMeans)) + 2) - 1, 1 / claytonTreeMeans,  type = "b", lty = 2)
points(2^((1:length(claytonTreeMeansLogSpace)) + 2) - 1, 1 / claytonTreeMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

## Clayton Grids

pdf(paper="special", file="infClaytonGrids.pdf", height = 6, width = 6)
par(pin=c(6, 6), mai=c(1,0.85,0.4,0.5), cex.axis = 1.35, cex.lab = 1.35)
plot((rep(1:length(claytonGridMeans), 2) + 1)^2, 1 / c(claytonGridMeans, claytonGridMeansLogSpace), type = "n", xlab = "variables", ylab = "seconds per sample")
points(((1:length(claytonGridMeans)) + 1)^2, 1 / claytonGridMeans,  type = "b", lty = 2)
points(((1:length(claytonGridMeansLogSpace)) + 1)^2, 1 / claytonGridMeansLogSpace, pch = 4,  type = "b", lty = 2)
dev.off()

par(mar = c(5.1, 4.1, 4.1, 2.1))

#plot(1:length(normalChainMeans) + 1, (normalChainMeansLogSpace - normalChainMeans) / normalChainMeans, type = "b", ylab = "relative difference", xlab = "n")
#plot(1:length(normalLoopMeans) + 2, (normalLoopMeansLogSpace - normalLoopMeans) / normalLoopMeans, type = "b", ylab = "relative difference", xlab = "n")
#plot(1:length(normalTreeMeans) + 2, (normalTreeMeansLogSpace - normalTreeMeans) / normalTreeMeans, type = "b", ylab = "relative difference", xlab = "n")
#plot(1:length(normalGridMeans) + 1, (normalGridMeansLogSpace - normalGridMeans) / normalGridMeans, type = "b", ylab = "relative difference", xlab = "n")