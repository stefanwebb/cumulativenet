## Analyse learning experiment
path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"
setwd(path)

# Load data set, and indices of experiments
source("..\\..\\rscripts\\cmarLoadData.r")

par(mfrow=c(1, 1), omi = c(0, 0, 0, 0), mai = c(1, 1.0, 0.25, 0.25))

# ### Plots for MSEs
# 
normalChainBounds = c(0, max(errorNormalChainmissing010[,3]))
# 
# Normal Chains #
# plot(2:15, medianNormalChainmissing100, xlab = "n", ylab = "median mean squared error") #, ylim = normalChainBounds)
# apply(errorNormalChainmissing100, 1, function(x) { lines(rep(x[1],2), x[2:3]) })
# 
# plot(2:15, medianNormalChainmissing075, xlab = "n", ylab = "median mean squared error") #, ylim = normalChainBounds)
# apply(errorNormalChainmissing075, 1, function(x) { lines(rep(x[1],2), x[2:3]) })
# 
# plot(2:15, medianNormalChainmissing050, xlab = "n", ylab = "median mean squared error") #, ylim = normalChainBounds)
# apply(errorNormalChainmissing050, 1, function(x) { lines(rep(x[1],2), x[2:3]) })
# 
# plot(2:15, medianNormalChainmissing025, xlab = "n", ylab = "median mean squared error") #, ylim = normalChainBounds)
# apply(errorNormalChainmissing025, 1, function(x) { lines(rep(x[1],2), x[2:3]) })
# 
# plot(2:15, medianNormalChainmissing010, xlab = "n", ylab = "median mean squared error") #, ylim = normalChainBounds)
# apply(errorNormalChainmissing010, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

## Trying on log scale

normalChainBounds = c(min(log(errorNormalChainmissing100[,2])), max(log(errorNormalChainmissing010[,3])))

plot(2:15, log(medianNormalChainmissing100), xlab = "n", ylab = "ln(mean squared error)", ylim = normalChainBounds)
apply(errorNormalChainmissing100, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(2:15, log(medianNormalChainmissing075), xlab = "n", ylab = "", ylim = normalChainBounds)
apply(errorNormalChainmissing075, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(2:15, log(medianNormalChainmissing050), xlab = "n", ylab = "", ylim = normalChainBounds)
apply(errorNormalChainmissing050, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(2:15, log(medianNormalChainmissing025), xlab = "n", ylab = "ln(mean squared error)", ylim = normalChainBounds)
apply(errorNormalChainmissing025, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(2:15, log(medianNormalChainmissing010), xlab = "n", ylab = "", ylim = normalChainBounds)
apply(errorNormalChainmissing010, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

# Load data set, and indices of experiments
source("..\\..\\rscripts\\learningLoadData.r")
# 
# Calculate data set indices for chains
source("..\\..\\rscripts\\learningIndicesChain.r")

errorNormalChainSamples100Restart = cbind(2:15, rbind(quantile(mseNormalChain2samples100restart, c(0.25, 0.75)), 
                                                      quantile(mseNormalChain3samples100restart, c(0.25, 0.75)), 
                                                      quantile(mseNormalChain4samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain5samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain6samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain7samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain8samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain9samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain10samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain11samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain12samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain13samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain14samples100restart, c(0.25, 0.75)),
                                                      quantile(mseNormalChain15samples100restart, c(0.25, 0.75))))

plot(2:15, log(mediansNormalChainSamples100Restart), xlab = "n", ylab = "", ylim = normalChainBounds)
apply(errorNormalChainSamples100Restart, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

#points(2:15, mediansNormalChainSamples10000GradDescent, pch = 5, col = "Blue")