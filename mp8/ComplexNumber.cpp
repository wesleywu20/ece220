#include "ComplexNumber.h"
#include "helper.h"

ComplexNumber::ComplexNumber()
{
    real_component = 0.0d;
    imaginary_component = 0.0d;
    magnitude = sqrt(real_component * real_component + imaginary_component * imaginary_component);
    phase = calculate_phase(real_component, imaginary_component);
    number_type = COMPLEX;
}

ComplexNumber::ComplexNumber(double rval_real_component, double rval_imaginary_component)
{
    real_component = rval_real_component;
    imaginary_component = rval_imaginary_component;
    magnitude = sqrt(real_component * real_component + imaginary_component * imaginary_component);
    phase = calculate_phase(real_component, imaginary_component);
    number_type = COMPLEX;
}

ComplexNumber::ComplexNumber( const ComplexNumber& other )
{
    real_component = other.real_component;
    imaginary_component = other.imaginary_component;
    magnitude = other.magnitude;
    phase = other.phase;
    number_type = other.number_type;
}

void ComplexNumber::set_real_component (double rval)
{
    real_component = rval;
    magnitude = sqrt(real_component * real_component + imaginary_component * imaginary_component);
    phase = calculate_phase(real_component, imaginary_component);
}

double ComplexNumber::get_real_component() const
{
    return real_component;
}

void ComplexNumber::set_imaginary_component (double rval)
{
    imaginary_component = rval;
    magnitude = sqrt(real_component * real_component + imaginary_component * imaginary_component);
    phase = calculate_phase(real_component, imaginary_component);
}

double ComplexNumber::get_imaginary_component() const
{
    return imaginary_component;
}

double ComplexNumber::get_magnitude() const
{
    return magnitude;
}

double ComplexNumber::get_phase() const
{
    return phase;
}

ComplexNumber ComplexNumber::operator + (const ComplexNumber& arg)
{
    return ComplexNumber(real_component + arg.get_real_component(), imaginary_component + arg.get_imaginary_component());
}

ComplexNumber ComplexNumber::operator - (const ComplexNumber& arg)
{
    return ComplexNumber(real_component - arg.get_real_component(), imaginary_component - arg.get_imaginary_component());
}

ComplexNumber ComplexNumber::operator * (const ComplexNumber& arg)
{
    return ComplexNumber(real_component * arg.get_real_component() - imaginary_component * arg.get_imaginary_component(), 
    real_component * arg.get_imaginary_component() + arg.get_real_component() * imaginary_component);
}

ComplexNumber ComplexNumber::operator / (const ComplexNumber& arg)
{
    double denom = arg.get_real_component() * arg.get_real_component() + arg.get_imaginary_component() * arg.get_imaginary_component();
    return ComplexNumber((real_component * arg.get_real_component() + imaginary_component * arg.get_imaginary_component())/denom, 
                        (imaginary_component * arg.get_real_component() - real_component * arg.get_imaginary_component())/denom);
}

ComplexNumber ComplexNumber::operator + (const RealNumber& arg)
{
	return ComplexNumber(real_component + arg.get_real_component(), imaginary_component);
}

ComplexNumber ComplexNumber::operator - (const RealNumber& arg)
{
	return ComplexNumber(real_component - arg.get_real_component(), imaginary_component);
}

ComplexNumber ComplexNumber::operator * (const RealNumber& arg)
{
    /* Your code here */
	return ComplexNumber(real_component * arg.get_real_component(), imaginary_component * arg.get_real_component());
}

ComplexNumber ComplexNumber::operator / (const RealNumber& arg)
{
    /* Your code here */
	return ComplexNumber(real_component / arg.get_real_component(), imaginary_component / arg.get_real_component());
}

ComplexNumber ComplexNumber::operator + (const ImaginaryNumber& arg){
	return ComplexNumber(real_component, imaginary_component + arg.get_imaginary_component());
}

ComplexNumber ComplexNumber::operator - (const ImaginaryNumber& arg)
{
	return ComplexNumber(real_component, imaginary_component - arg.get_imaginary_component());
}

ComplexNumber ComplexNumber::operator * (const ImaginaryNumber& arg)
{
	return ComplexNumber(-imaginary_component * arg.get_imaginary_component(), real_component * arg.get_imaginary_component());
}

ComplexNumber ComplexNumber::operator / (const ImaginaryNumber& arg)
{
    double denom = arg.get_imaginary_component() * arg.get_imaginary_component();
	return ComplexNumber((imaginary_component * arg.get_imaginary_component())/denom,
                        (-real_component * arg.get_imaginary_component())/denom);
}

string ComplexNumber::to_String(){
    /* Do not modify */
    stringstream my_output;
    if (imaginary_component > 0)
        my_output << std::setprecision(3) << real_component << " + " << imaginary_component << 'i';
    else if (imaginary_component < 0)
        my_output << std::setprecision(3) << real_component << " - " << abs(imaginary_component) << 'i';
    else
        my_output << std::setprecision(3) << real_component;
    
    return my_output.str();
}