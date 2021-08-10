#include "ImaginaryNumber.h"
#include "helper.h"

ImaginaryNumber::ImaginaryNumber()
{
    imaginary_component = 0.0d;
    number_type = IMAGINARY;
    magnitude = sqrt(imaginary_component * imaginary_component);
    phase = calculate_phase(0, imaginary_component);
}

ImaginaryNumber::ImaginaryNumber(double rval)
{
    imaginary_component = rval;
    number_type = IMAGINARY;
    magnitude = sqrt(imaginary_component * imaginary_component);
    phase = calculate_phase(0, imaginary_component);
}

ImaginaryNumber::ImaginaryNumber( const ImaginaryNumber& other )
{
    imaginary_component = other.imaginary_component;
    number_type = other.number_type;
    magnitude = other.magnitude;
    phase = other.phase;
}

void ImaginaryNumber::set_imaginary_component (double rval)
{
    imaginary_component = rval;
    magnitude = sqrt(imaginary_component * imaginary_component);
    phase = calculate_phase(0, imaginary_component);
}

double ImaginaryNumber::get_imaginary_component() const
{
    return imaginary_component;
}

//Getter function for magnitude
double ImaginaryNumber::get_magnitude() const{
    return magnitude;
}

//Getter function for phase
double ImaginaryNumber::get_phase() const{
    return phase;
}

//Operator Overload
ImaginaryNumber ImaginaryNumber::operator + (const ImaginaryNumber& arg)
{
    return ImaginaryNumber(imaginary_component + arg.get_imaginary_component());
}

ImaginaryNumber ImaginaryNumber::operator - (const ImaginaryNumber& arg)
{
    return ImaginaryNumber(imaginary_component - arg.get_imaginary_component());
}

RealNumber ImaginaryNumber::operator * (const ImaginaryNumber& arg)
{
    return RealNumber(-imaginary_component * arg.get_imaginary_component());
}

RealNumber ImaginaryNumber::operator / (const ImaginaryNumber& arg)
{
    return RealNumber(imaginary_component / arg.get_imaginary_component());
}

ComplexNumber ImaginaryNumber::operator + (const RealNumber& arg)
{
    return ComplexNumber(arg.get_real_component(), imaginary_component);
}

ComplexNumber ImaginaryNumber::operator - (const RealNumber& arg)
{
    return ComplexNumber(-arg.get_real_component(), imaginary_component);
}

ImaginaryNumber ImaginaryNumber::operator * (const RealNumber& arg)
{
    return ImaginaryNumber(arg.get_real_component() * imaginary_component);
}

ImaginaryNumber ImaginaryNumber::operator / (const RealNumber& arg)
{
    return ImaginaryNumber(imaginary_component/arg.get_real_component());
}

ComplexNumber ImaginaryNumber::operator + (const ComplexNumber& arg)
{
    return ComplexNumber(arg.get_real_component(), arg.get_imaginary_component() + imaginary_component);
}

ComplexNumber ImaginaryNumber::operator - (const ComplexNumber& arg)
{
    return ComplexNumber(-arg.get_real_component(), imaginary_component - arg.get_imaginary_component());
}

ComplexNumber ImaginaryNumber::operator * (const ComplexNumber& arg)
{
    return ComplexNumber(-arg.get_imaginary_component() * imaginary_component, arg.get_real_component() * imaginary_component);
}

ComplexNumber ImaginaryNumber::operator / (const ComplexNumber& arg)
{
    double denom = arg.get_real_component() * arg.get_real_component() + arg.get_imaginary_component() * arg.get_imaginary_component();
    return ComplexNumber((imaginary_component * arg.get_imaginary_component())/denom, (imaginary_component * arg.get_real_component())/denom);
}

string ImaginaryNumber::to_String(){
    /* Do not modify */
    stringstream my_output;
    my_output << std::setprecision(3) << imaginary_component << 'i';
    return my_output.str();
}