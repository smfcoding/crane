#include "ReactantSecondOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactantSecondOrder);

template <>
InputParameters
validParams<ReactantSecondOrder>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "Whether or not v and u are the same variable.");
  return params;
}

ReactantSecondOrder::ReactantSecondOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _reaction_coeff(getMaterialProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("_v_eq_u"))
{
}

Real
ReactantSecondOrder::computeQpResidual()
{
  Real mult_1;

  if (isCoupled("v"))
    mult_1 = _v[_qp];
  else
    mult_1 = _n_gas[_qp];

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult_1 * _u[_qp];
  // if (isCoupled("v"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _u[_qp];
  // }
  // else
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _u[_qp];
}

Real
ReactantSecondOrder::computeQpJacobian()
{
  Real mult_1, power, eq_u_mult, gas_mult;
  power = 1.0;
  eq_u_mult = _u[_qp];
  gas_mult = 1.0;

  if (isCoupled("v"))
    mult_1 = _v[_qp];
  else
    mult_1 = _n_gas[_qp];

  if (_v_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_1;
  }
  else
    gas_mult *= mult_1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power * std::pow(eq_u_mult, power-1) * gas_mult * _phi[_j][_qp];
  // return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power * eq_u_mult * gas_mult * _phi[_j][_qp];
}

Real
ReactantSecondOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isCoupled("v"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _u[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
