/* -----------------------------------------------------------------------------
** Copyright (c) 2012 Advanced Resources and Risk Technology, LLC
** All rights reserved.
**
** This file is part of Advanced Resources and Risk Technology, LLC (AR2TECH) 
** version of the open source software sgems.  It is a derivative work by 
** AR2TECH (THE LICENSOR) based on the x-free license granted in the original 
** version of the software (see notice below) and now sublicensed such that it 
** cannot be distributed or modified without the explicit and written permission 
** of AR2TECH.
**
** Only AR2TECH can modify, alter or revoke the licensing terms for this 
** file/software.
**
** This file cannot be modified or distributed without the explicit and written 
** consent of AR2TECH.
**
** Contact Dr. Alex Boucher (aboucher@ar2tech.com) for any questions regarding
** the licensing of this file/software
**
** The open-source version of sgems can be downloaded at 
** sourceforge.net/projects/sgems.
** ----------------------------------------------------------------------------*/



/**********************************************************************
** Author: Nicolas Remy
** Copyright (C) 2002-2004 The Board of Trustees of the Leland Stanford Junior
**   University
** All rights reserved.
**
** This file is part of the "math" module of the Geostatistical Earth
** Modeling Software (GEMS)
**
** This file may be distributed and/or modified under the terms of the 
** license defined by the Stanford Center for Reservoir Forecasting and 
** appearing in the file LICENSE.XFREE included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.gnu.org/copyleft/gpl.html for GPL licensing information.
**
** Contact the Stanford Center for Reservoir Forecasting, Stanford University
** if any conditions of this licensing are not clear to you.
**
**********************************************************************/


#include <math/correlation_measure.h>

#include <cmath>


Correlation_measure_factory::CallBackMap* 
Correlation_measure_factory::methods_map_ = 0;

std::vector<std::string>*
Correlation_measure_factory::available_methods_ = 0;

/** Add new correlation measures here
*/
void 
Correlation_measure_factory::initialize() {
  methods_map_ = new CallBackMap;
  available_methods_ = new std::vector<std::string>;

  add_method( "variogram", Variogram_measure::new_instance );
  add_method( "indicator-variogram", Indicator_variogram_measure::new_instance );
  add_method( "covariance", Covariance_measure::new_instance );
  add_method( "correlogram", Correlogram_measure::new_instance );
  add_method( "madogram", Madogram_measure::new_instance );
  add_method( "rodogram", Rodogram_measure::new_instance );
}


Correlation_measure* 
Correlation_measure_factory::
instantiate( const std::string& method_name ) {
  if( !methods_map_ ) initialize();
 
  CallBackMap::const_iterator found = methods_map_->find( method_name );
  if( found == methods_map_->end() )
    return 0;

  return (found->second)();
}


bool
Correlation_measure_factory::
add_method( const std::string& name, 
            const CorrelationCallBackType& callback ) {
  if( !methods_map_ ) initialize();

  std::pair<CallBackMap::iterator, bool> pos;
  pos = methods_map_->insert( std::make_pair( name, callback ) );
  available_methods_->push_back( name );
  return pos.second;
}
  

std::vector<std::string> 
Correlation_measure_factory::
available_methods() {
  if( !methods_map_ ) initialize();

  return  *available_methods_;
}





//=====================================================

double Correlation_measure::NaN = -9e99;

Correlation_measure::Correlation_measure() {
  pair_count_ = 0;
  accumulated_value_ = 0; 
}

void Correlation_measure::add_pair( const ValPair& head_prop, 
                                    const ValPair& tail_prop ) {
  accumulated_value_ += compute_single( head_prop, tail_prop );
  pair_count_ ++;
}
  

double Correlation_measure::
correlation( const std::vector<ValPair>& head_prop_values,
             const std::vector<ValPair>& tail_prop_values ) {
  if( head_prop_values.size() != tail_prop_values.size() ) return NaN;

  for( unsigned int i =0 ; i < head_prop_values.size() ; i++ ) {
    add_pair( head_prop_values[i], tail_prop_values[i] );
  }
  return correlation();
}


double Correlation_measure::correlation() {
  if( pair_count_ <= 0 ) return NaN;

  double n= double(pair_count_);
  return accumulated_value_ / n;
}


//----------------------------

Correlation_measure* Variogram_measure::clone() const {
  return new Variogram_measure( *this );
}

double Variogram_measure::
compute_single( const ValPair& head_prop, 
                const ValPair& tail_prop ) {

  return  
    0.5*(head_prop.first - head_prop.second)*(tail_prop.first - tail_prop.second);
}



//----------------------------

Correlation_measure* Covariance_measure::clone() const {
  return new Covariance_measure( *this );
}

void Covariance_measure::
add_pair( const ValPair& head_prop, 
          const ValPair& tail_prop ) {
  pair_count_ ++;
  accumulated_value_ += head_prop.first * tail_prop.second;

  means_.first += head_prop.first;
  means_.second += tail_prop.second;
}
  
double Covariance_measure::
correlation( const std::vector<ValPair>& head_prop_values,
             const std::vector<ValPair>& tail_prop_values ) {

  if( head_prop_values.size() != tail_prop_values.size() ) return NaN;

  double total = 0.0;
  double mean1 = 0;
  double mean2 = 0;
  for( unsigned int i = 0 ; i < head_prop_values.size() ; i++ ) {
    total += head_prop_values[i].first * tail_prop_values[i].second;
    mean1 += head_prop_values[i].first;
    mean2 += tail_prop_values[i].second;
  }

  double n = double( head_prop_values.size());

//  float v1 = (total - mean1*mean2) / double( head_prop_values.size() );
//  float v2 = mean1*mean2 / double( head_prop_values.size() );
  float v1 = total / n;
  float v2 = mean1/ n *mean2 / n;
  return v1-v2;
}

double Covariance_measure::correlation() const {

  double n = double(pair_count_);
  double means_prod = means_.first/n * means_.second/n;
  return accumulated_value_/n - means_prod;
/*
  double means_prod = means_.first * means_.second;
  return (accumulated_value_ - means_prod) / double(pair_count_);
  */
}


double Covariance_measure::
compute_single( const ValPair& head_prop, 
                const ValPair& tail_prop ) {
  return -1; //not implemented
}



//----------------------------

Correlation_measure* Correlogram_measure::clone() const {
  return new Correlogram_measure( *this );
}

void Correlogram_measure::
add_pair( const ValPair& head_prop, 
          const ValPair& tail_prop ) {

  double v1 = head_prop.first;
  double v2 = tail_prop.second;
            
  pair_count_ ++;
  accumulated_value_ += v1*v2;

  vars_.first  += v1*v1;
  vars_.second += v2*v2;

  means_.first  += v1;
  means_.second += v2;
}

double Correlogram_measure::correlation() { 
  double n = pair_count_;
  means_.first /= n;
  means_.second /= n;


  vars_.first = vars_.first/n  - (means_.first*means_.first);
  vars_.second= vars_.second/n - (means_.second*means_.second);

  double means_prod = means_.first*means_.second;
  double vars_prod = vars_.first*vars_.second;
  return (accumulated_value_/n - means_prod) / std::sqrt(vars_prod);
}


double Correlogram_measure::
compute_single( const ValPair& head_prop, 
                const ValPair& tail_prop ) {
  return -1; //not implemented
}


//----------------------------

Correlation_measure* Indicator_variogram_measure::clone() const {
  return new Indicator_variogram_measure( *this );
}

void Indicator_variogram_measure::
set_parameters( const std::vector<double>& params ) {
  if( params.empty() ) return;
  head_threshold_ = params[0];
  if( params.size() >= 2 )
    tail_threshold_ = params[1];
  else
    tail_threshold_ = params[0];
}

Indicator_variogram_measure::ValPair 
Indicator_variogram_measure::
indicator( const ValPair& v, double thresh ) {
  double i1=0.0, i2=0.0;
  if( v.first < thresh ) i1 = 1.0;
  if( v.second < thresh ) i2 = 1.0;

  return std::make_pair( i1, i2 );
}

void Indicator_variogram_measure::
add_pair( const ValPair& head_prop, 
          const ValPair& tail_prop ) {
  Variogram_measure::add_pair( indicator(head_prop, head_threshold_),
                               indicator(tail_prop, tail_threshold_) );
}


//----------------------------

Correlation_measure* Variogram_measure_order_w::clone() const {
  return new Variogram_measure_order_w( *this );
}

void Variogram_measure_order_w::
set_parameters( const std::vector<double>& params ) {
  if( params.empty() ) return;
  w_ = params[0];
}

double Variogram_measure_order_w::
compute_single( const ValPair& head_prop,
                const ValPair& tail_prop ) {

  return
    0.5*std::pow(std::fabs(head_prop.first - head_prop.second),w_/2)
	   *std::pow(std::fabs(tail_prop.first - tail_prop.second),w_/2);
}

//----------------------------

Correlation_measure* Madogram_measure::clone() const {
  return new Madogram_measure( *this );
}
double Madogram_measure::
compute_single( const ValPair& head_prop,
                const ValPair& tail_prop ) {

// Must take the square root such that the product is the
// absolute value
  return
    0.5*std::sqrt(std::fabs(head_prop.first - head_prop.second))
	   *std::sqrt(std::fabs(tail_prop.first - tail_prop.second));
}

//----------------------------

Correlation_measure* Rodogram_measure::clone() const {
  return new Rodogram_measure( *this );
}
double Rodogram_measure::
compute_single( const ValPair& head_prop,
                const ValPair& tail_prop ) {

  return
  0.5*std::pow(std::fabs(head_prop.first - head_prop.second),0.25)
	 *std::pow(std::fabs(tail_prop.first - tail_prop.second),0.25);
}
