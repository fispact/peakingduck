# we should wrap matplotlib so it is easy to change later
# if need be
try:
    import matplotlib.pyplot
    PLT = matplotlib.pyplot
except:
    PLT = None
    raise ImportError("Matplotlib cannot be found. It is required for plotting.")


class PlotAdapter(object):
    """
        Wraps the Matplotlib plotter
    """
    _engine_name = 'matplotlib'
    _engine = matplotlib.pyplot

    def __repr__(self):
        return "<%s: %s>" % (self.__class__.__name__, self.enginename)

    @property
    def enginename(self):
        """
        The name of the plotting engine
        """
        return self._engine_name

    @property
    def engine(self):
        """
        The plotter engine
        """
        return self._engine

    def show(self):
        self.engine.show()

    def grid(self, show=True):
        self.engine.grid(show)

    def addlegend(self, location):
        self.engine.legend(loc=location)

    def newcanvas(self, *args, **kwargs):
        self._figure = self.engine.figure(*args, **kwargs)
        return self._figure


class LinePlotAdapter(PlotAdapter):
    def lineplot(self, x, y, datalabel="", xlabel="", ylabel="",
                 logx=False, logy=False, overlay=True):

        if not overlay:
            self.newcanvas()

        self.engine.xlabel(xlabel)
        self.engine.ylabel(ylabel)

        if logx:
            self.engine.xscale('log')
        if logy:
            self.engine.yscale('log')

        self.engine.plot(x, y, label=datalabel)