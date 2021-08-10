#include "RealNumber.h"
#include "helper.h"

RealNumber::RealNumber()
{
    real_component = 0.0d;
    number_type = REAL;
    magnitude = sqrt(real_component * real_component);
    phase = calculate_phase(real_component, 0);
}

RealNumber::RealNumber(double rval)
{
    real_component = rval;
    number_type = REAL;
    magnitude = sqrt(real_component * real_component);
    phase = calculate_phase(real_component, 0);
}

RealNumber::RealNumber( const RealNumber& other )
{
    real_component = other.real_component;
    number_type = other.number_type;
    magnitude = other.magnitude;
    phase = other.phase;
}

void RealNumber::set_real_component (double rval)
{
    real_component = rval;
    magnitude = sqrt(real_component * real_component);
    phase = calculate_phase(real_component, 0);
}

double RealNumber::get_real_component() const
{
    return real_component;
}

double RealNumber::get_magnitude() const{
    return magnitude;
}

double RealNumber::get_phase() const{
    return phase;
}

RealNumber RealNumber::operator + (const RealNumber& arg)
{
    return RealNumber(real_component + arg.get_real_component());
}

RealNumber RealNumber::operator - (const RealNumber& arg)
{
    return RealNumber(real_component - arg.get_real_component());
}

RealNumber RealNumber::operator * (const RealNumber& arg)
{
    return RealNumber(real_component * arg.get_real_component());
}

RealNumber RealNumber::operator / (const RealNumber& arg)
{
    return RealNumber(real_component / arg.get_real_component());
}

ComplexNumber RealNumber::operator + (const ImaginaryNumber& arg)
{
	return ComplexNumber(real_component, arg.get_imaginary_component());
}

ComplexNumber RealNumber::operator - (const ImaginaryNumber& arg)
{
	return ComplexNumber(real_component, -arg.get_imaginary_component());
}

ImaginaryNumber RealNumber::operator * (const ImaginaryNumber& arg)
{
	return ImaginaryNumber(real_component * arg.get_imaginary_component());
}

ImaginaryNumber RealNumber::operator / (const ImaginaryNumber& arg){
	return ImaginaryNumber((real_component * arg.get_imaginary_component()) / (-arg.get_imaginary_component() * arg.get_imaginary_component()));
}

ComplexNumber RealNumber::operator + (const ComplexNumber& arg){
	return ComplexNumber(real_component + arg.get_real_component(), arg.get_imaginary_component());
}

ComplexNumber RealNumber::operator - (const ComplexNumber& arg){
	return ComplexNumber(real_component - arg.get_real_component(), -arg.get_imaginary_component());
}

ComplexNumber RealNumber::operator * (const ComplexNumber& arg){
	return ComplexNumber(real_component * arg.get_real_component(), real_component * arg.get_imaginary_component());
}

ComplexNumber RealNumber::operator / (const ComplexNumber& arg){
    double denom = arg.get_real_component() * arg.get_real_component() + arg.get_imaginary_component() * arg.get_imaginary_component();
    return ComplexNumber((real_component * arg.get_real_component())/denom, (-real_component * arg.get_imaginary_component())/denom);
}

string RealNumber::to_String(){
    /* Do not modify */
    stringstream my_output;
    my_output << std::setprecision(3) << real_component;
    return my_output.str();
}