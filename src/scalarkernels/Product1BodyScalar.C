#include "Product1BodyScalar.h"

registerMooseObject("CraneApp", Product1BodyScalar);

template <>
InputParameters
validParams<Product1BodyScalar>()
{
  InputParameters params = validParams<ODEKernel>();
  params.addCoupledVar("v", 0, "Coupled variable 1.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("n_gas", "The gas density.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  // params.addRequiredParam<Real>("reaction_coefficient", "The reaction coefficient.");
  params.addParam<bool>("v_eq_u", false, "Whether or not u = v.");
  params.addParam<bool>("rate_constant_equation", false, "True if rate constant is provided by equation.");
  // params.addRequiredParam<UserObjectName>("rate_provider",
  //     "The name of the UserObject that can provide the rate coefficient.");
  return params;
}

Product1BodyScalar::Product1BodyScalar(const InputParameters & parameters)
  : ODEKernel(parameters),
    _v_var(isCoupledScalar("v") ? coupledScalar("v") : -1),
    _v(coupledScalarValue("v")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
    // _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Product1BodyScalar::computeQpResidual()
{
  Real mult1;
  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;


  // if (_rate_constant_equation)
  return -_stoichiometric_coeff * _rate_coefficient[_i] * mult1;
  // else
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
}

Real
Product1BodyScalar::computeQpJacobian()
{
  Real mult1;
  Real power, eq_u_mult, gas_mult;
  Real rate_constant;
  power = 0.0;
  eq_u_mult = 1.0;
  gas_mult = 1.0;

  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (isCoupledScalar("v") && _v_eq_u)
  {
    power += 1.0;
    eq_u_mult = mult1;
  }
  else
    gas_mult *= mult1;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
  //   rate_constant = _data.reaction_coefficient();

  return -_stoichiometric_coeff * rate_constant * gas_mult * power * std::pow(eq_u_mult, power-1);
}

Real
Product1BodyScalar::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power, mult1, deriv_factor, other_factor;
  Real rate_constant;
  power = 0;
  other_factor = 1;
  deriv_factor = 1;
  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (jvar == _v_var && !_v_eq_u)
  {
    power += 1;
    deriv_factor = mult1;
  }
  else
    other_factor *= mult1;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
  //   rate_constant = _data.reaction_coefficient();

  return -_stoichiometric_coeff * rate_constant * other_factor * power
    * std::pow(deriv_factor, power-1);
}
