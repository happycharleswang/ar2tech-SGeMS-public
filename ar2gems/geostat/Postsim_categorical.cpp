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
** Author: Alexandre Boucher
** Copyright (C) 2002-2004 The Board of Trustees of the Leland Stanford Junior
**   University
** All rights reserved.
**
** This file is part of the "geostat" module of the Geostatistical Earth
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
#include <geostat/Postsim_categorical.h>
#include <utils/string_manipulation.h>
#include <grid/geostat_grid.h>
#include <grid/gval_iterator.h>
#include <math/gstlpoint.h>
#include <geostat/utilities.h>

#include <algorithm>


bool Postsim_categorical::initialize( const Parameters_handler* parameters,
			Error_messages_handler* errors ) 
{
  this->extract_parameters(parameters);
	std::cout << "initializing algorithm Postsim_categorical \n";

	std::string harddata_grid_name = parameters->value( "grid_name.value" );
	errors->report( harddata_grid_name.empty(), 
			"grid_name", "No grid specified" );

	if( !harddata_grid_name.empty() ) {
		grid_ = 0;
		bool ok = geostat_utils::create( grid_, harddata_grid_name,
				 "grid_name", errors );
		if( !ok || !grid_ ) return false;
	}
	else 
		return false;

  std::string region_name = parameters->value( "grid_name.region" );
  region_ = grid_->region(region_name);


  std::string prop_str = parameters->value( "props.value" );
  if(prop_str.empty()) {
    errors->report("props","No property specified");
    return false;
  }
	std::vector< std::string > prop_name = 
            String_Op::decompose_string(prop_str, ";" );
	
	for(std::vector< std::string >::iterator it_str = prop_name.begin(); 
    it_str != prop_name.end(); it_str++ ) {
      Grid_categorical_property* prop = grid_->categorical_property( *it_str );
      errors->report(!prop, "props","No categorical property exist with that name");
      props_.push_back(prop);
  }
  if(!errors->empty()) return false;


  std::string output_name_prefix = parameters->value( "out_name_prefix.value" );
  errors->report(output_name_prefix.empty(), "out_name_prefix", "No output_name specified" );
  
  //Check if the selected property are compatible, i.e. has the same categorical_definition

  cat_prop_vecT::iterator it = props_.begin();
  cat_def_ = (*it)->get_category_definition();
  if(!cat_def_) {
    errors->report("props","The property "+(*it)->name()+" does not have a categorical definition");
    return false;
  }
  for( ; it!= props_.end(); it++) {
    if( cat_def_ != (*it)->get_category_definition() ) {
      errors->report("props","Different categorical definition between properties");
      return false;
    }
  }


  // We need to create as many property as there are categories.
	const CategoricalPropertyDefinitionName* defname =
			dynamic_cast<const CategoricalPropertyDefinitionName*>(cat_def_);

	ncat_;
	if(defname) ncat_ = defname->number_of_category();
	else {
		std::set<int> cat_code;
    for( it= props_.begin() ; it!= props_.end(); it++) {
		  Grid_continuous_property::const_iterator it_gval = (*it)->begin(true);
		  for( ; it_gval != (*it)->end(); ++it_gval) {
        cat_code.insert( *it_gval );
		  }
    }
    ncat_ = cat_code.size();
	}

  Grid_property_group* group = geostat_utils::add_group_to_grid( grid_, output_name_prefix,"CategoricalProbability");
  CategoricalProbabilityPropertyGroup* cgroup = 
    dynamic_cast<CategoricalProbabilityPropertyGroup*>(group);
  cgroup->set_categorical_definition(defname);

  for(int i =0; i<ncat_; i++) {
    std::string prop_name = output_name_prefix+" "+cat_def_->get_category_name_from_index(i);
    Grid_continuous_property* prop = geostat_utils::add_property_to_grid( grid_, prop_name );
    prop->set_parameters(parameters_);
    etype_props_.push_back( prop );
    cgroup->add_property(prop);
  }

	return true;
}


int Postsim_categorical::execute( GsTL_project* ) { 


	bool all_informed;
	std::vector< Grid_continuous_property* >::const_iterator it_prop;;
	int nprop = props_.size();

	for(int node_id = 0 ; node_id < grid_->size(); ++node_id ) {
    if(region_ && !region_->is_inside_region(node_id)) continue;
		all_informed=true;
		for(int k = 0; k < props_.size(); ++k ) {
      if( !props_[k]->is_informed( node_id ) ) {
        all_informed = false;
        break;
      }
		}
    if( !all_informed ) continue;

    // For eac category
    for(int c = 0; c< ncat_; c++) {
      int code = cat_def_->category_id_from_index(c);
      float sum = 0.;
      for(int k = 0; k < props_.size(); ++k ) {
        sum += props_[k]->get_indicator_value( node_id, code );
      }
      etype_props_[c]->set_value(sum/nprop,node_id);
    }
	}

	return 0;
}


Postsim_categorical::Postsim_categorical() : grid_(0), region_(0) {
}



Named_interface* Postsim_categorical::create_new_interface( std::string& ) {
  return new Postsim_categorical;
}

