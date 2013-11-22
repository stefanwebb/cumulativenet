## Analyse learning experiment
path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"
setwd(path)

# Load data set, and indices of experiments
source("..\\..\\rscripts\\learningLoadData.r")
# 
# # Calculate data set indices for chains
# source("..\\..\\rscripts\\learningIndicesChain.r")
# 
# Calculate data set indices for loops
source("..\\..\\rscripts\\learningIndicesLoop.r")

#par(mfrow=c(1, 3), omi = c(0.5, 0, 0.5, 0))

# ### Plots for MSEs
# 
# normalChainBounds = c(min(mediansNormalChainSamples100GradDescent), max(mediansNormalChainSamples100GradDescent))
# 
# # Normal Chains
# plot(2:15, mediansNormalChainSamples100GradDescent, ylim = normalChainBounds, main = "Gradient Descent", xlab = "n", ylab = "median mean squared error", col = "Red")
# points(2:15, mediansNormalChainSamples1000GradDescent, pch = 4, col = "Green")
# points(2:15, mediansNormalChainSamples10000GradDescent, pch = 5, col = "Blue")
# 
# plot(2:15, mediansNormalChainSamples100Restart, ylim = normalChainBounds, main = "L-BFGS with Restart", xlab = "n", ylab = "", col = "Red")
# points(2:15, mediansNormalChainSamples1000Restart, pch = 4, col = "Green")
# points(2:15, mediansNormalChainSamples10000Restart, pch = 5, col = "Blue")
# 
# plot(2:15, mediansNormalChainSamples100Barrier, ylim = normalChainBounds, main = "L-BFGS with Barrier", xlab = "n", ylab = "", col = "Red")
# points(2:15, mediansNormalChainSamples1000Barrier, pch = 4, col = "Green")
# points(2:15, mediansNormalChainSamples10000Barrier, pch = 5, col = "Blue")
# 
# legend(-8.54692, -0.016, xjust = 0.5, legend = c("100 samples", "1000 samples", "10000 samples"), horiz = TRUE, pch = c(1, 4, 5), xpd = NA, col = c("Red", "Green", "Blue"))
# 
# title(main = "Normal Chains", outer = TRUE, cex.main = 1.5)
# 
#par(mfrow=c(2, 3), omi = c(0.5, 0, 0.5, 0))

#par(mfrow=c(1, 1), omi = c(0, 0, 0, 0))
par(mfrow = c(1, 1), oma = c(0, 0, 0, 0), mai = c(1, 1.0, 0.25, 0.25))

# 
# # Clayton Chains
# plot(2:15, mediansClaytonChainSamples100GradDescent, main = "Gradient Descent", xlab = "n", ylab = "median mean squared error", col = "Red")
# points(2:15, mediansClaytonChainSamples1000GradDescent, pch = 4, col = "Green")
# points(2:15, mediansClaytonChainSamples10000GradDescent, pch = 5, col = "Blue")
# 
# plot(2:15, mediansClaytonChainSamples100Restart, main = "L-BFGS with Restart", xlab = "n", ylab = "", col = "Red")
# points(2:15, mediansClaytonChainSamples1000Restart, pch = 4, col = "Green")
# points(2:15, mediansClaytonChainSamples10000Restart, pch = 5, col = "Blue")
# 
# plot(2:15, mediansClaytonChainSamples100Barrier, main = "L-BFGS with Barrier", xlab = "n", ylab = "", col = "Red")
# points(2:15, mediansClaytonChainSamples1000Barrier, pch = 4, col = "Green")
# points(2:15, mediansClaytonChainSamples10000Barrier, pch = 5, col = "Blue")
# 
# title(main = "Clayton Chains", outer = TRUE, cex.main = 1.5)
# 

lowerNormalLoopSamples100GradDescent = cbind(3:15, rbind(quantile(mseNormalLoop3samples100gradDescent, c(0.25, 0.75)), 
                                             quantile(mseNormalLoop4samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop5samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop6samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop7samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop8samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop9samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop10samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop11samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop12samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop13samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop14samples100gradDescent, c(0.25, 0.75)),
                                             quantile(mseNormalLoop15samples100gradDescent, c(0.25, 0.75))))

lowerNormalLoopSamples1000GradDescent = cbind(3:15, rbind(quantile(mseNormalLoop3samples1000gradDescent, c(0.25, 0.75)), 
                                                         quantile(mseNormalLoop4samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop5samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop6samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop7samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop8samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop9samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop10samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop11samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop12samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop13samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop14samples1000gradDescent, c(0.25, 0.75)),
                                                         quantile(mseNormalLoop15samples1000gradDescent, c(0.25, 0.75))))

lowerNormalLoopSamples10000GradDescent = cbind(3:15, rbind(quantile(mseNormalLoop3samples10000gradDescent, c(0.25, 0.75)), 
                                                          quantile(mseNormalLoop4samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop5samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop6samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop7samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop8samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop9samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop10samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop11samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop12samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop13samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop14samples10000gradDescent, c(0.25, 0.75)),
                                                          quantile(mseNormalLoop15samples10000gradDescent, c(0.25, 0.75))))

lowerNormalLoopSamples100Restart = cbind(3:15, rbind(quantile(mseNormalLoop3samples100restart, c(0.25, 0.75)), 
                                                     quantile(mseNormalLoop4samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop5samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop6samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop7samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop8samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop9samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop10samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop11samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop12samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop13samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop14samples100restart, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop15samples100restart, c(0.25, 0.75))))

lowerNormalLoopSamples1000Restart = cbind(3:15, rbind(quantile(mseNormalLoop3samples1000restart, c(0.25, 0.75)), 
                                                      quantile(mseNormalLoop4samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop5samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop6samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop7samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop8samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop9samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop10samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop11samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop12samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop13samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop14samples1000restart, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop15samples1000restart, c(0.25, 0.75))))

lowerNormalLoopSamples10000Restart = cbind(3:15, rbind(quantile(mseNormalLoop3samples10000restart, c(0.25, 0.75)), 
                                                       quantile(mseNormalLoop4samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop5samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop6samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop7samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop8samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop9samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop10samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop11samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop12samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop13samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop14samples10000restart, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop15samples10000restart, c(0.25, 0.75))))

lowerNormalLoopSamples100Barrier = cbind(3:15, rbind(quantile(mseNormalLoop3samples100barrier, c(0.25, 0.75)), 
                                                     quantile(mseNormalLoop4samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop5samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop6samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop7samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop8samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop9samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop10samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop11samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop12samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop13samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop14samples100barrier, c(0.25, 0.75)),
                                                     quantile(mseNormalLoop15samples100barrier, c(0.25, 0.75))))

lowerNormalLoopSamples1000Barrier = cbind(3:15, rbind(quantile(mseNormalLoop3samples1000barrier, c(0.25, 0.75)), 
                                                      quantile(mseNormalLoop4samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop5samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop6samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop7samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop8samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop9samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop10samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop11samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop12samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop13samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop14samples1000barrier, c(0.25, 0.75)),
                                                      quantile(mseNormalLoop15samples1000barrier, c(0.25, 0.75))))

lowerNormalLoopSamples10000Barrier = cbind(3:15, rbind(quantile(mseNormalLoop3samples10000barrier, c(0.25, 0.75)), 
                                                       quantile(mseNormalLoop4samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop5samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop6samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop7samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop8samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop9samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop10samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop11samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop12samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop13samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop14samples10000barrier, c(0.25, 0.75)),
                                                       quantile(mseNormalLoop15samples10000barrier, c(0.25, 0.75))))

lowerItersNormalLoopSamples100GradDescent = cbind(3:15, rbind(quantile(itersNormalLoop3samples100gradDescent, c(0.25, 0.75)), 
                                                              quantile(itersNormalLoop4samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop5samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop6samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop7samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop8samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop9samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop10samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop11samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop12samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop13samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop14samples100gradDescent, c(0.25, 0.75)),
                                                              quantile(itersNormalLoop15samples100gradDescent, c(0.25, 0.75))))

lowerItersNormalLoopSamples1000GradDescent = cbind(3:15, rbind(quantile(itersNormalLoop3samples1000gradDescent, c(0.25, 0.75)), 
                                                               quantile(itersNormalLoop4samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop5samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop6samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop7samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop8samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop9samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop10samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop11samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop12samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop13samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop14samples1000gradDescent, c(0.25, 0.75)),
                                                               quantile(itersNormalLoop15samples1000gradDescent, c(0.25, 0.75))))

lowerItersNormalLoopSamples10000GradDescent = cbind(3:15, rbind(quantile(itersNormalLoop3samples10000gradDescent, c(0.25, 0.75)), 
                                                                quantile(itersNormalLoop4samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop5samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop6samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop7samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop8samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop9samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop10samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop11samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop12samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop13samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop14samples10000gradDescent, c(0.25, 0.75)),
                                                                quantile(itersNormalLoop15samples10000gradDescent, c(0.25, 0.75))))

lowerItersNormalLoopSamples100Restart = cbind(3:15, rbind(quantile(itersNormalLoop3samples100restart, c(0.25, 0.75)), 
                                                          quantile(itersNormalLoop4samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop5samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop6samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop7samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop8samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop9samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop10samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop11samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop12samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop13samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop14samples100restart, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop15samples100restart, c(0.25, 0.75))))

lowerItersNormalLoopSamples1000Restart = cbind(3:15, rbind(quantile(itersNormalLoop3samples1000restart, c(0.25, 0.75)), 
                                                           quantile(itersNormalLoop4samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop5samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop6samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop7samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop8samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop9samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop10samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop11samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop12samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop13samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop14samples1000restart, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop15samples1000restart, c(0.25, 0.75))))

lowerItersNormalLoopSamples10000Restart = cbind(3:15, rbind(quantile(itersNormalLoop3samples10000restart, c(0.25, 0.75)), 
                                                            quantile(itersNormalLoop4samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop5samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop6samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop7samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop8samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop9samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop10samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop11samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop12samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop13samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop14samples10000restart, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop15samples10000restart, c(0.25, 0.75))))

lowerItersNormalLoopSamples100Barrier = cbind(3:15, rbind(quantile(itersNormalLoop3samples100barrier, c(0.25, 0.75)), 
                                                          quantile(itersNormalLoop4samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop5samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop6samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop7samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop8samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop9samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop10samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop11samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop12samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop13samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop14samples100barrier, c(0.25, 0.75)),
                                                          quantile(itersNormalLoop15samples100barrier, c(0.25, 0.75))))

lowerItersNormalLoopSamples1000Barrier = cbind(3:15, rbind(quantile(itersNormalLoop3samples1000barrier, c(0.25, 0.75)), 
                                                           quantile(itersNormalLoop4samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop5samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop6samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop7samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop8samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop9samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop10samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop11samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop12samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop13samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop14samples1000barrier, c(0.25, 0.75)),
                                                           quantile(itersNormalLoop15samples1000barrier, c(0.25, 0.75))))

lowerItersNormalLoopSamples10000Barrier = cbind(3:15, rbind(quantile(itersNormalLoop3samples10000barrier, c(0.25, 0.75)), 
                                                            quantile(itersNormalLoop4samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop5samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop6samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop7samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop8samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop9samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop10samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop11samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop12samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop13samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop14samples10000barrier, c(0.25, 0.75)),
                                                            quantile(itersNormalLoop15samples10000barrier, c(0.25, 0.75))))

normalLoopBounds = c(min(log(lowerNormalLoopSamples10000GradDescent[,2])), max(c(log(lowerNormalLoopSamples100GradDescent[,3]))))

# Normal Loops
#plot(3:15, mediansNormalLoopSamples100Restart, main = "L-BFGS with Restart", ylim = normalLoopBounds, xlab = "n", ylab = "median mean squared error")
plot(3:15, log(mediansNormalLoopSamples100Restart), ylim = normalLoopBounds, xlab = "n", ylab = "ln(mean squared error)")
apply(lowerNormalLoopSamples100Restart, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

#plot(3:15, mediansNormalLoopSamples100Barrier, main = "L-BFGS with Barrier", ylim = normalLoopBounds, xlab = "n", ylab = "")
plot(3:15, log(mediansNormalLoopSamples100Barrier), ylim = normalLoopBounds, xlab = "n", ylab = "")
apply(lowerNormalLoopSamples100Barrier, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

#plot(3:15, mediansNormalLoopSamples100GradDescent, main = "Gradient Descent", ylim = normalLoopBounds, xlab = "n", ylab = "median mean squared error")
plot(3:15, log(mediansNormalLoopSamples100GradDescent), ylim = normalLoopBounds, xlab = "n", ylab = "")
apply(lowerNormalLoopSamples100GradDescent, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

##
plot(3:15, log(mediansNormalLoopSamples1000Restart), ylim = normalLoopBounds, xlab = "n", ylab = "ln(mean squared error)")
apply(lowerNormalLoopSamples1000Restart, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(3:15, log(mediansNormalLoopSamples1000Barrier), ylim = normalLoopBounds, xlab = "n", ylab = "")
apply(lowerNormalLoopSamples1000Barrier, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(3:15, log(mediansNormalLoopSamples1000GradDescent), ylim = normalLoopBounds, xlab = "n", ylab = "")
apply(lowerNormalLoopSamples1000GradDescent, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

##
plot(3:15, log(mediansNormalLoopSamples10000Restart), ylim = normalLoopBounds, xlab = "n", ylab = "ln(mean squared error)")
apply(lowerNormalLoopSamples10000Restart, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(3:15, log(mediansNormalLoopSamples10000Barrier), ylim = normalLoopBounds, xlab = "n", ylab = "")
apply(lowerNormalLoopSamples10000Barrier, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

plot(3:15, log(mediansNormalLoopSamples10000GradDescent), ylim = normalLoopBounds, xlab = "n", ylab = "")
apply(lowerNormalLoopSamples10000GradDescent, 1, function(x) { lines(rep(x[1],2), log(x[2:3])) })

# points(3:15, mediansNormalLoopSamples1000GradDescent, pch = 4, col = "Green")
# points(3:15, mediansNormalLoopSamples10000GradDescent, pch = 5, col = "Blue")
# 
# points(3:15, mediansNormalLoopSamples1000Restart, pch = 4, col = "Green")
# points(3:15, mediansNormalLoopSamples10000Restart, pch = 5, col = "Blue")
# 
# points(3:15, mediansNormalLoopSamples1000Barrier, pch = 4, col = "Green")
# points(3:15, mediansNormalLoopSamples10000Barrier, pch = 5, col = "Blue")

#title(main = "Normal Loops", outer = TRUE, cex.main = 1.5)

# # Clayton Loops
# plot(3:15, mediansClaytonLoopSamples100GradDescent, main = "Gradient Descent", xlab = "n", ylab = "median mean squared error", col = "Red")
# points(3:15, mediansClaytonLoopSamples1000GradDescent, pch = 4, col = "Green")
# points(3:15, mediansClaytonLoopSamples10000GradDescent, pch = 5, col = "Blue")
# 
# plot(3:15, mediansClaytonLoopSamples100Restart, main = "L-BFGS with Restart", xlab = "n", ylab = "", col = "Red")
# points(3:15, mediansClaytonLoopSamples1000Restart, pch = 4, col = "Green")
# points(3:15, mediansClaytonLoopSamples10000Restart, pch = 5, col = "Blue")
# 
# plot(3:15, mediansClaytonLoopSamples100Barrier, main = "L-BFGS with Barrier", xlab = "n", ylab = "", col = "Red")
# points(3:15, mediansClaytonLoopSamples1000Barrier, pch = 4, col = "Green")
# points(3:15, mediansClaytonLoopSamples10000Barrier, pch = 5, col = "Blue")
# 
# title(main = "Clayton Loops", outer = TRUE, cex.main = 1.5)
# 
# ### Plots for number of iterations
# 
# par(mfrow=c(1, 3), omi = c(0.5, 0, 0.5, 0))
# 
# # Normal Chains
# plot(2:15, itersNormalChainSamples100GradDescent, ylim = c(1, 100), main = "Gradient Descent", xlab = "n", ylab = "median iterations", col = "Red")
# points(2:15, itersNormalChainSamples1000GradDescent, pch = 4, col = "Green")
# points(2:15, itersNormalChainSamples10000GradDescent, pch = 5, col = "Blue")
# 
# plot(2:15, itersNormalChainSamples100Restart, ylim = c(1, 100), main = "L-BFGS with Restart", xlab = "n", ylab = "", col = "Red")
# points(2:15, itersNormalChainSamples1000Restart, pch = 4, col = "Green")
# points(2:15, itersNormalChainSamples10000Restart, pch = 5, col = "Blue")
# 
# plot(2:15, itersNormalChainSamples100Barrier, ylim = c(1, 100), main = "L-BFGS with Barrier", xlab = "n", ylab = "", col = "Red")
# points(2:15, itersNormalChainSamples1000Barrier, pch = 4, col = "Green")
# points(2:15, itersNormalChainSamples10000Barrier, pch = 5, col = "Blue")
# 
# legend(-8.54692, -34.67457, xjust = 0.5, legend = c("100 samples", "1000 samples", "10000 samples"), horiz = TRUE, pch = c(1, 4, 5), xpd = NA, col = c("Red", "Green", "Blue"))
# 
# title(main = "Normal Chains", outer = TRUE, cex.main = 1.5)
# 
#par(mfrow=c(1, 3), omi = c(0.5, 0, 0.5, 0))
# 
# # Clayton Chains
# plot(2:15, itersClaytonChainSamples100GradDescent, ylim = c(1, 100), main = "Gradient Descent", xlab = "n", ylab = "median iterations", col = "Red")
# points(2:15, itersClaytonChainSamples1000GradDescent, pch = 4, col = "Green")
# points(2:15, itersClaytonChainSamples10000GradDescent, pch = 5, col = "Blue")
# 
# plot(2:15, itersClaytonChainSamples100Restart, ylim = c(1, 100), main = "L-BFGS with Restart", xlab = "n", ylab = "", col = "Red")
# points(2:15, itersClaytonChainSamples1000Restart, pch = 4, col = "Green")
# points(2:15, itersClaytonChainSamples10000Restart, pch = 5, col = "Blue")
# 
# plot(2:15, itersClaytonChainSamples100Barrier, ylim = c(1, 100), main = "L-BFGS with Barrier", xlab = "n", ylab = "", col = "Red")
# points(2:15, itersClaytonChainSamples1000Barrier, pch = 4, col = "Green")
# points(2:15, itersClaytonChainSamples10000Barrier, pch = 5, col = "Blue")
# 
# title(main = "Clayton Chains", outer = TRUE, cex.main = 1.5)

#normalLoopBounds = c(0, max(itersNormalChainSamples100GradDescent))
# 
# # Normal Loops
plot(3:15, itersNormalLoopSamples100Restart, ylim = c(1, 100), xlab = "n", ylab = "iterations")
apply(lowerItersNormalLoopSamples100Restart, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples100Barrier, ylim = c(1, 100), xlab = "n", ylab = "")
apply(lowerItersNormalLoopSamples100Barrier, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples100GradDescent, ylim = c(1, 100), xlab = "n", ylab = "")
apply(lowerItersNormalLoopSamples100GradDescent, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples1000Restart, ylim = c(1, 100), xlab = "n", ylab = "iterations")
apply(lowerItersNormalLoopSamples1000Restart, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples1000Barrier, ylim = c(1, 100), xlab = "n", ylab = "")
apply(lowerItersNormalLoopSamples1000Barrier, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples1000GradDescent, ylim = c(1, 100), xlab = "n", ylab = "")
apply(lowerItersNormalLoopSamples1000GradDescent, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples10000Restart, ylim = c(1, 100), xlab = "n", ylab = "iterations")
apply(lowerItersNormalLoopSamples10000Restart, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples10000Barrier, ylim = c(1, 100), xlab = "n", ylab = "")
apply(lowerItersNormalLoopSamples10000Barrier, 1, function(x) { lines(rep(x[1],2), x[2:3]) })

plot(3:15, itersNormalLoopSamples10000GradDescent, ylim = c(1, 100), xlab = "n", ylab = "")
apply(lowerItersNormalLoopSamples10000GradDescent, 1, function(x) { lines(rep(x[1],2), x[2:3]) })


#title(main = "Normal Loops", outer = TRUE, cex.main = 1.5)

#legend(-7.54692, -34.67457, xjust = 0.5, legend = c("100 samples", "1000 samples", "10000 samples"), horiz = TRUE, pch = c(1, 4, 5), xpd = NA, col = c("Red", "Green", "Blue"))

# # Clayton Loops
# plot(3:15, itersClaytonLoopSamples100GradDescent, main = "Gradient Descent", ylim = c(1, 100), xlab = "n", ylab = "median iterations", col = "Red")
# points(3:15, itersClaytonLoopSamples1000GradDescent, pch = 4, col = "Green")
# points(3:15, itersClaytonLoopSamples10000GradDescent, pch = 5, col = "Blue")
# 
# plot(3:15, itersClaytonLoopSamples100Restart, main = "L-BFGS with Restart", ylim = c(1, 100), xlab = "n", ylab = "", col = "Red")
# points(3:15, itersClaytonLoopSamples1000Restart, pch = 4, col = "Green")
# points(3:15, itersClaytonLoopSamples10000Restart, pch = 5, col = "Blue")
# 
# plot(3:15, itersClaytonLoopSamples100Barrier, main = "L-BFGS with Barrier", ylim = c(1, 100), xlab = "n", ylab = "", col = "Red")
# points(3:15, itersClaytonLoopSamples1000Barrier, pch = 4, col = "Green")
# points(3:15, itersClaytonLoopSamples10000Barrier, pch = 5, col = "Blue")
# 
# title(main = "Clayton Loops", outer = TRUE, cex.main = 1.5)

par(mfrow=c(1,1))

#plot(2:15, mediansClaytonChainSamples10000GradDescent)
#points(2:15, mediansClaytonChainSamples1000GradDescent, pch = 4)
#points(2:15, mediansClaytonChainSamples10000GradDescent, pch = 5)