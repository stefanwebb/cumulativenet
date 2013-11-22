learningData = read.table("learning.csv", sep = ",", header = TRUE)

# Create indices
whereNormal = learningData$copula == "normal"
whereClayton = learningData$copula == "clayton"
whereLoop = learningData$model == "loop"
whereChain = learningData$model == "chain"
whereGrid = learningData$model == "grid"
whereTree = learningData$model == "tree"
whereSize2 = learningData$size == 2
whereSize3 = learningData$size == 3
whereSize4 = learningData$size == 4
whereSize5 = learningData$size == 5
whereSize6 = learningData$size == 6
whereSize7 = learningData$size == 7
whereSize8 = learningData$size == 8
whereSize9 = learningData$size == 9
whereSize10 = learningData$size == 10
whereSize11 = learningData$size == 11
whereSize12 = learningData$size == 12
whereSize13 = learningData$size == 13
whereSize14 = learningData$size == 14
whereSize15 = learningData$size == 15
where100 = learningData$samples == 100
where1000 = learningData$samples == 1000
where10000 = learningData$samples == 10000
whereGradDescent = learningData$method == "graddescent"
whereRestart = learningData$method == "lbfgsrestart"
whereBarrier = learningData$method == "lbfgsbarrier"