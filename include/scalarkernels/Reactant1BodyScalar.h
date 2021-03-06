#ifndef REACTANT1BODYSCALAR_H
#define REACTANT1BODYSCALAR_H

#include "ODEKernel.h"
// #include "RateCoefficientProvider.h"

class Reactant1BodyScalar;

template <>
InputParameters validParams<Reactant1BodyScalar>();

class Reactant1BodyScalar : public ODEKernel
{
public:
  Reactant1BodyScalar(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  const VariableValue & _rate_coefficient;
  Real _n_gas;
  Real _stoichiometric_coeff;
  bool _rate_constant_equation;

  // const RateCoefficientProvider & _data;
};

#endif /* REACTANT1BODYSCALAR_H */
