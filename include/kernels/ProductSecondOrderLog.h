/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef PRODUCTSECONDORDERLOG_H
#define PRODUCTSECONDORDERLOG_H

#include "Kernel.h"

// Forward Declaration
class ProductSecondOrderLog;

template <>
InputParameters validParams<ProductSecondOrderLog>();

class ProductSecondOrderLog : public Kernel
{
public:
  ProductSecondOrderLog(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // MooseVariable & _coupled_var_A;
  // MooseVariable & _coupled_var_B;
  const VariableValue & _v;
  const VariableValue & _w;
  unsigned int _v_id;
  unsigned int _w_id;
  const MaterialProperty<Real> & _n_gas;

  // The reaction coefficient
  const MaterialProperty<Real> & _reaction_coeff;
  Real _stoichiometric_coeff;
  bool _v_eq_u;
  bool _w_eq_u;
  bool _v_coupled;
  bool _w_coupled;
};
#endif // PRODUCTSECONDORDERLOG_H
