
# function to plot histogram
def getplotvalues(x,y):
    """
        Matplotlib hist is slow. Use a hist like plot with conventional line plot
    """
    assert (len(y) + 1) == len(x)
    X = []
    Y = []
    for i in range(len(x)-1):
        X.append(x[i])
        Y.append(y[i])
        X.append(x[i+1])
        Y.append(y[i])
    return X, Y
