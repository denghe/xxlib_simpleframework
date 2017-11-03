#include "Precompile.h"

GenericEventParameter::GenericEventParameter( double value_numeric )
: isNumeric( true )
, value_numeric( value_numeric  )
{
}

GenericEventParameter::GenericEventParameter( string value_string )
: isNumeric( false )
, value_string( value_string )
{
}

GenericEventParameter::GenericEventParameter( GenericEventParameter const & other )
: isNumeric( other.isNumeric )
{
    if( other.isNumeric )
        this->value_numeric = other.value_numeric;
    else
        this->value_string = other.value_string;
}

GenericEventParameter::GenericEventParameter( GenericEventParameter && other )
: isNumeric( other.isNumeric )
{
    if( other.isNumeric )
        this->value_numeric = other.value_numeric;
    else
        this->value_string = move(other.value_string);
}

GenericEventParameter & GenericEventParameter::operator=( GenericEventParameter const & other )
{
    this->isNumeric = other.isNumeric;
    if( other.isNumeric )
        this->value_numeric = other.value_numeric;
    else
        this->value_string = other.value_string;
    return *this;
}

GenericEventParameter & GenericEventParameter::operator=( GenericEventParameter && other )
{
    this->isNumeric = other.isNumeric;
    if( other.isNumeric )
        this->value_numeric = other.value_numeric;
    else
        this->value_string = move( other.value_string );
    return *this;
}

