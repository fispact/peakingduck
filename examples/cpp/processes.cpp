#include "peakingduck.hpp"

#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <memory>
#include <iomanip>

using namespace peakingduck::core;
using namespace peakingduck::util;
using namespace peakingduck::io;

class SnipRemoval : public IProcess<double> {
public:
  SnipRemoval(int snipSize) : size(snipSize) {}
  NumericalData<double> go(const NumericalData<double>& data) const override {
    rrange<int> window(0, size+1);
    return data - data.snip(window.begin(), window.end());
  }
private:
  int size;
};

class SavitzkyGolaySmoother : public IProcess<double> {
public:
  SavitzkyGolaySmoother(int windowwidth, int polynomialorder = 2) :
    windowsize(windowwidth), order(polynomialorder) {}
  NumericalData<double> go(const NumericalData<double>& data) const override {
    namespace py = pybind11;
    py::scoped_interpreter guard{};

    py::object savgol_filter = py::module::import("scipy.signal").attr("savgol_filter");
    py::array_t<double> pyData = py::cast(data.to_vector());
    py::object result = savgol_filter(pyData, windowsize, order);
    return NumericalData<double>(result.cast<std::vector<double> >());
  }
private:
  int windowsize, order;
};

class ThresholdCut : public IProcess<double> {
public:
  ThresholdCut(double thresholdLevel) : threshold(thresholdLevel) {}
  NumericalData<double> go(const NumericalData<double>& data) const override {
    return data.ramp(threshold);
  }
private:
  double threshold;
};

int main() {
  SimpleProcessManager<double> pm;
  pm.append(std::dynamic_pointer_cast<IProcess<double> >(std::make_shared<MovingAverageSmoother<double> >(1)));
  pm.append(std::dynamic_pointer_cast<IProcess<double> >(std::make_shared<SnipRemoval>(40)));
  pm.append(std::dynamic_pointer_cast<IProcess<double> >(std::make_shared<SavitzkyGolaySmoother>(3)));
  pm.append(std::dynamic_pointer_cast<IProcess<double> >(std::make_shared<ThresholdCut>(1e3)));
  pm.append(std::dynamic_pointer_cast<IProcess<double> >(std::make_shared<MovingAverageSmoother<double> >(3)));

  std::ifstream file("../../reference/spectrum0.csv");
  Spectrum<double, double> hist_raw;
  Deserialize<double, double, ','>(file, hist_raw);
  NumericalData<double> energies = hist_raw.X(), data = hist_raw.Y();
  
  NumericalData<double> processeddata = pm.run(data);
  for (int i : rrange<int>(0, processeddata.size())) {
    if (processeddata[i] > 0) {
      std::cout << "Peak at energy: ";
      std::cout << std::setprecision(4) << 0.5*(energies[i] + energies[i+1]);
      std::cout << " eV\n";
    }
  }
}
