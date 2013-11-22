learningData = read.table("cmar.csv", sep = ",", header = TRUE)

# Create indices
#whereNormal = learningData$copula == "normal"
#whereClayton = learningData$copula == "clayton"
#whereLoop = learningData$model == "loop"
#whereChain = learningData$model == "chain"
#whereGrid = learningData$model == "grid"
#whereTree = learningData$model == "tree"
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

whereMissing100 = learningData$missing == 1.0
whereMissing075 = learningData$missing == 0.75
whereMissing050 = learningData$missing == 0.5
whereMissing025 = learningData$missing == 0.25
whereMissing010 = learningData$missing == 0.1

mseNormalChain2missing100 = learningData$mse[whereSize2 & whereMissing100]
mseNormalChain3missing100 = learningData$mse[whereSize3 & whereMissing100]
mseNormalChain4missing100 = learningData$mse[whereSize4 & whereMissing100]
mseNormalChain5missing100 = learningData$mse[whereSize5 & whereMissing100]
mseNormalChain6missing100 = learningData$mse[whereSize6 & whereMissing100]
mseNormalChain7missing100 = learningData$mse[whereSize7 & whereMissing100]
mseNormalChain8missing100 = learningData$mse[whereSize8 & whereMissing100]
mseNormalChain9missing100 = learningData$mse[whereSize9 & whereMissing100]
mseNormalChain10missing100 = learningData$mse[whereSize10 & whereMissing100]
mseNormalChain11missing100 = learningData$mse[whereSize11 & whereMissing100]
mseNormalChain12missing100 = learningData$mse[whereSize12 & whereMissing100]
mseNormalChain13missing100 = learningData$mse[whereSize13 & whereMissing100]
mseNormalChain14missing100 = learningData$mse[whereSize14 & whereMissing100]
mseNormalChain15missing100 = learningData$mse[whereSize15 & whereMissing100]

mseNormalChain2missing075 = learningData$mse[whereSize2 & whereMissing075]
mseNormalChain3missing075 = learningData$mse[whereSize3 & whereMissing075]
mseNormalChain4missing075 = learningData$mse[whereSize4 & whereMissing075]
mseNormalChain5missing075 = learningData$mse[whereSize5 & whereMissing075]
mseNormalChain6missing075 = learningData$mse[whereSize6 & whereMissing075]
mseNormalChain7missing075 = learningData$mse[whereSize7 & whereMissing075]
mseNormalChain8missing075 = learningData$mse[whereSize8 & whereMissing075]
mseNormalChain9missing075 = learningData$mse[whereSize9 & whereMissing075]
mseNormalChain10missing075 = learningData$mse[whereSize10 & whereMissing075]
mseNormalChain11missing075 = learningData$mse[whereSize11 & whereMissing075]
mseNormalChain12missing075 = learningData$mse[whereSize12 & whereMissing075]
mseNormalChain13missing075 = learningData$mse[whereSize13 & whereMissing075]
mseNormalChain14missing075 = learningData$mse[whereSize14 & whereMissing075]
mseNormalChain15missing075 = learningData$mse[whereSize15 & whereMissing075]

mseNormalChain2missing050 = learningData$mse[whereSize2 & whereMissing050]
mseNormalChain3missing050 = learningData$mse[whereSize3 & whereMissing050]
mseNormalChain4missing050 = learningData$mse[whereSize4 & whereMissing050]
mseNormalChain5missing050 = learningData$mse[whereSize5 & whereMissing050]
mseNormalChain6missing050 = learningData$mse[whereSize6 & whereMissing050]
mseNormalChain7missing050 = learningData$mse[whereSize7 & whereMissing050]
mseNormalChain8missing050 = learningData$mse[whereSize8 & whereMissing050]
mseNormalChain9missing050 = learningData$mse[whereSize9 & whereMissing050]
mseNormalChain10missing050 = learningData$mse[whereSize10 & whereMissing050]
mseNormalChain11missing050 = learningData$mse[whereSize11 & whereMissing050]
mseNormalChain12missing050 = learningData$mse[whereSize12 & whereMissing050]
mseNormalChain13missing050 = learningData$mse[whereSize13 & whereMissing050]
mseNormalChain14missing050 = learningData$mse[whereSize14 & whereMissing050]
mseNormalChain15missing050 = learningData$mse[whereSize15 & whereMissing050]

mseNormalChain2missing025 = learningData$mse[whereSize2 & whereMissing025]
mseNormalChain3missing025 = learningData$mse[whereSize3 & whereMissing025]
mseNormalChain4missing025 = learningData$mse[whereSize4 & whereMissing025]
mseNormalChain5missing025 = learningData$mse[whereSize5 & whereMissing025]
mseNormalChain6missing025 = learningData$mse[whereSize6 & whereMissing025]
mseNormalChain7missing025 = learningData$mse[whereSize7 & whereMissing025]
mseNormalChain8missing025 = learningData$mse[whereSize8 & whereMissing025]
mseNormalChain9missing025 = learningData$mse[whereSize9 & whereMissing025]
mseNormalChain10missing025 = learningData$mse[whereSize10 & whereMissing025]
mseNormalChain11missing025 = learningData$mse[whereSize11 & whereMissing025]
mseNormalChain12missing025 = learningData$mse[whereSize12 & whereMissing025]
mseNormalChain13missing025 = learningData$mse[whereSize13 & whereMissing025]
mseNormalChain14missing025 = learningData$mse[whereSize14 & whereMissing025]
mseNormalChain15missing025 = learningData$mse[whereSize15 & whereMissing025]

mseNormalChain2missing010 = learningData$mse[whereSize2 & whereMissing010]
mseNormalChain3missing010 = learningData$mse[whereSize3 & whereMissing010]
mseNormalChain4missing010 = learningData$mse[whereSize4 & whereMissing010]
mseNormalChain5missing010 = learningData$mse[whereSize5 & whereMissing010]
mseNormalChain6missing010 = learningData$mse[whereSize6 & whereMissing010]
mseNormalChain7missing010 = learningData$mse[whereSize7 & whereMissing010]
mseNormalChain8missing010 = learningData$mse[whereSize8 & whereMissing010]
mseNormalChain9missing010 = learningData$mse[whereSize9 & whereMissing010]
mseNormalChain10missing010 = learningData$mse[whereSize10 & whereMissing010]
mseNormalChain11missing010 = learningData$mse[whereSize11 & whereMissing010]
mseNormalChain12missing010 = learningData$mse[whereSize12 & whereMissing010]
mseNormalChain13missing010 = learningData$mse[whereSize13 & whereMissing010]
mseNormalChain14missing010 = learningData$mse[whereSize14 & whereMissing010]
mseNormalChain15missing010 = learningData$mse[whereSize15 & whereMissing010]

medianNormalChainmissing100 = c(median(mseNormalChain2missing100),
                                median(mseNormalChain3missing100),
                                median(mseNormalChain4missing100),
                                median(mseNormalChain5missing100),
                                median(mseNormalChain6missing100),
                                median(mseNormalChain7missing100),
                                median(mseNormalChain8missing100),
                                median(mseNormalChain9missing100),
                                median(mseNormalChain10missing100),
                                median(mseNormalChain11missing100),
                                median(mseNormalChain12missing100),
                                median(mseNormalChain13missing100),
                                median(mseNormalChain14missing100),
                                median(mseNormalChain15missing100))

medianNormalChainmissing075 = c(median(mseNormalChain2missing075),
                                median(mseNormalChain3missing075),
                                median(mseNormalChain4missing075),
                                median(mseNormalChain5missing075),
                                median(mseNormalChain6missing075),
                                median(mseNormalChain7missing075),
                                median(mseNormalChain8missing075),
                                median(mseNormalChain9missing075),
                                median(mseNormalChain10missing075),
                                median(mseNormalChain11missing075),
                                median(mseNormalChain12missing075),
                                median(mseNormalChain13missing075),
                                median(mseNormalChain14missing075),
                                median(mseNormalChain15missing075))

medianNormalChainmissing050 = c(median(mseNormalChain2missing050),
                                median(mseNormalChain3missing050),
                                median(mseNormalChain4missing050),
                                median(mseNormalChain5missing050),
                                median(mseNormalChain6missing050),
                                median(mseNormalChain7missing050),
                                median(mseNormalChain8missing050),
                                median(mseNormalChain9missing050),
                                median(mseNormalChain10missing050),
                                median(mseNormalChain11missing050),
                                median(mseNormalChain12missing050),
                                median(mseNormalChain13missing050),
                                median(mseNormalChain14missing050),
                                median(mseNormalChain15missing050))

medianNormalChainmissing025 = c(median(mseNormalChain2missing025),
                                median(mseNormalChain3missing025),
                                median(mseNormalChain4missing025),
                                median(mseNormalChain5missing025),
                                median(mseNormalChain6missing025),
                                median(mseNormalChain7missing025),
                                median(mseNormalChain8missing025),
                                median(mseNormalChain9missing025),
                                median(mseNormalChain10missing025),
                                median(mseNormalChain11missing025),
                                median(mseNormalChain12missing025),
                                median(mseNormalChain13missing025),
                                median(mseNormalChain14missing025),
                                median(mseNormalChain15missing025))

medianNormalChainmissing010 = c(median(mseNormalChain2missing010),
                                median(mseNormalChain3missing010),
                                median(mseNormalChain4missing010),
                                median(mseNormalChain5missing010),
                                median(mseNormalChain6missing010),
                                median(mseNormalChain7missing010),
                                median(mseNormalChain8missing010),
                                median(mseNormalChain9missing010),
                                median(mseNormalChain10missing010),
                                median(mseNormalChain11missing010),
                                median(mseNormalChain12missing010),
                                median(mseNormalChain13missing010),
                                median(mseNormalChain14missing010),
                                median(mseNormalChain15missing010))

errorNormalChainmissing100 = (cbind(2:15, rbind(quantile((mseNormalChain2missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain3missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain4missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain5missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain6missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain7missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain8missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain9missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain10missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain11missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain12missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain13missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain14missing100), c(0.25, 0.75)),
                                                quantile((mseNormalChain15missing100), c(0.25, 0.75)))))

errorNormalChainmissing075 = (cbind(2:15, rbind(quantile((mseNormalChain2missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain3missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain4missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain5missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain6missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain7missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain8missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain9missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain10missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain11missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain12missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain13missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain14missing075), c(0.25, 0.75)),
                                                quantile((mseNormalChain15missing075), c(0.25, 0.75)))))

errorNormalChainmissing050 = (cbind(2:15, rbind(quantile((mseNormalChain2missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain3missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain4missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain5missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain6missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain7missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain8missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain9missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain10missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain11missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain12missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain13missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain14missing050), c(0.25, 0.75)),
                                                quantile((mseNormalChain15missing050), c(0.25, 0.75)))))

errorNormalChainmissing025 = (cbind(2:15, rbind(quantile((mseNormalChain2missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain3missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain4missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain5missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain6missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain7missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain8missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain9missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain10missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain11missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain12missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain13missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain14missing025), c(0.25, 0.75)),
                                                quantile((mseNormalChain15missing025), c(0.25, 0.75)))))

errorNormalChainmissing010 = (cbind(2:15, rbind(quantile((mseNormalChain2missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain3missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain4missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain5missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain6missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain7missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain8missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain9missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain10missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain11missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain12missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain13missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain14missing010), c(0.25, 0.75)),
                                                quantile((mseNormalChain15missing010), c(0.25, 0.75)))))