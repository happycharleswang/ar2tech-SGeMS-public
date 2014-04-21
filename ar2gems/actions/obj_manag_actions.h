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
** This file is part of the "actions" module of the Geostatistical Earth
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

#ifndef __GSTLAPPLI_OBJ_MANAG_ACTIONS_H__ 
#define __GSTLAPPLI_OBJ_MANAG_ACTIONS_H__ 
 
#include <actions/common.h>
#include <appli/action.h> 
#include <grid/geostat_grid.h> 
#include <grid/property_copier.h>

#include <qstring.h>
 
#include <string> 
#include <set>
 

class Input_filter; 
class GsTL_project; 
class Error_messages_handler;
class Cartesian_grid;
 
 
class ACTIONS_DECL New_rgrid : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  New_rgrid() {} 
  virtual ~New_rgrid() {} 
   
  /** The parameters for this function are: 
   * - the name of the new grid 
   * - its dimensions nx, ny, nz 
   */ 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
 
 private: 
  int nx_, ny_, nz_; 
  std::string name_; 
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
}; 
 
 
class ACTIONS_DECL New_cartesian_grid_action : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  New_cartesian_grid_action(); 
  virtual ~New_cartesian_grid_action() {} 
   
  /** The parameters for this function are: 
   * - the name of the new grid 
   * - its dimensions nx, ny, nz 
   */ 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
 
 private: 
  int nx_, ny_, nz_; 
  float xsize_, ysize_, zsize_; 
  double Ox_, Oy_, Oz_;  
  float z_rotation_;
  std::string name_; 
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
}; 
 
 
class ACTIONS_DECL RunScript : public Action 
{
public:
	static Named_interface* create_new_interface( std::string& );

public:
	RunScript() { }
	virtual ~RunScript() {}

	virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
	virtual bool exec();
	
protected:
	std::string _scriptfile;
	GsTL_project* _proj; 
	Error_messages_handler* _errors;

};

//TL modified
/*
class ACTIONS_DECL Load_sim : public Action {
public: 
  static Named_interface* create_new_interface( std::string& );
  
public:
  Load_sim() ;
  virtual ~Load_sim() ;

  
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec();
  
protected: 

  typedef Grid_continuous_property::property_type Type ;

  QString dirname_; 
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
  SmartPtr<Input_filter> filter_; 
  static const std::string parser ;

  void transform(const Type *, int, int, int, Grid_continuous_property *) ;

};

*/



 

/** Transfer a property from a grid to another grid.
* 4 parameters are required (in that order):
* - the server grid
* - the server property
* - the client grid
* - the client porperty
*/
class ACTIONS_DECL Copy_property: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Copy_property() {} 
  virtual ~Copy_property() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
 
 protected: 
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
  SmartPtr<Property_copier> copier_;

  Geostat_grid* server_;
  Grid_continuous_property* server_prop_;

  Geostat_grid* client_;
  Grid_continuous_property* client_prop_;

};  


/** Transfer a property from RAM to disk.
* parameters: grid name and property names
*/
class ACTIONS_DECL Swap_property_to_disk : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Swap_property_to_disk() {} 
  virtual ~Swap_property_to_disk() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

 protected: 
  std::vector<Grid_continuous_property*> properties_to_swap_;
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
}; 



/** Transfer a property from disk to RAM.
* parameters: grid name and property names
*/
class ACTIONS_DECL Swap_property_to_ram : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Swap_property_to_ram() {} 
  virtual ~Swap_property_to_ram() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

 protected: 
  std::vector<Grid_continuous_property*> properties_to_swap_;
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
}; 




class ACTIONS_DECL Delete_objects : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Delete_objects() {} 
  virtual ~Delete_objects() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

 protected: 
  std::vector<Grid_continuous_property*> properties_to_delete_;
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
}; 



class ACTIONS_DECL Delete_properties : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Delete_properties() {} 
  virtual ~Delete_properties() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

 protected: 
  std::vector<Grid_continuous_property*> properties_to_delete_;
  GsTL_project* proj_; 
  Error_messages_handler* errors_;
}; 



class ACTIONS_DECL Clear_property_value_if: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Clear_property_value_if() {} 
  virtual ~Clear_property_value_if() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
}; 


class ACTIONS_DECL Set_active_region: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Set_active_region() {} 
  virtual ~Set_active_region() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
};


class ACTIONS_DECL Delete_regions: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Delete_regions() {} 
  virtual ~Delete_regions() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
};


class ACTIONS_DECL Set_hard_data: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Set_hard_data() {} 
  virtual ~Set_hard_data() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
};


class ACTIONS_DECL Set_region_from_property: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Set_region_from_property() {} 
  virtual ~Set_region_from_property() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
}; 


class ACTIONS_DECL Set_region_from_categorical_property: public Action {
 public:
  static Named_interface* create_new_interface( std::string& );

 public:
  Set_region_from_categorical_property() {}
  virtual ~Set_region_from_categorical_property() {}

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool exec();
};



class ACTIONS_DECL Merge_regions : public Action { 
// public: 
//  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Merge_regions() {} 
  virtual ~Merge_regions() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec() = 0; 

protected : 
  std::vector<Grid_region*> regions_;
  Grid_region* new_region_;
  std::string grid_name_;
  GsTL_project* proj_;

}; 

class ACTIONS_DECL Merge_regions_union : public Merge_regions { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Merge_regions_union() {} 
  virtual ~Merge_regions_union() {}
 /*
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
                     */
  virtual bool exec(); 

};


class ACTIONS_DECL Merge_regions_intersection : public Merge_regions { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
   Merge_regions_intersection() {} 
  virtual ~Merge_regions_intersection() {}
 /*
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
                     */
  virtual bool exec(); 

};

class ACTIONS_DECL Set_region_complement : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Set_region_complement() {} 
  virtual ~Set_region_complement() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

}; 



class ACTIONS_DECL Clear_property_value_from_property : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Clear_property_value_from_property() {} 
  virtual ~Clear_property_value_from_property() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

}; 



class ACTIONS_DECL Clear_property_inside_region : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Clear_property_inside_region() {} 
  virtual ~Clear_property_inside_region() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

}; 


class ACTIONS_DECL Clear_property_outside_region : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Clear_property_outside_region() {} 
  virtual ~Clear_property_outside_region() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

}; 



class ACTIONS_DECL Create_trend : public Action { 
 public: 
   static Named_interface* create_new_interface( std::string& );
 
 public: 
  Create_trend();
  virtual ~Create_trend() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

  virtual std::vector<std::string> get_trend_functions();

 protected: 
   GsTL_project* proj_;
   Geostat_grid* grid_;
   std::string direction_id_;
   std::string grid_name_;
   Grid_continuous_property* trend_;
   std::set<std::string> directions_;

   bool is_direction_valid(std::string direction, 
                           Error_messages_handler* errors);
}; 

class ACTIONS_DECL Scale_property : public Action { 
 public: 
   static Named_interface* create_new_interface( std::string& );
 
 public: 
  Scale_property();
  virtual ~Scale_property() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

 protected: 
   Geostat_grid* grid_;
   Grid_continuous_property* input_prop_;
   Grid_continuous_property* output_prop_;
   float target_min_;
   float target_max_;
   
};

class ACTIONS_DECL Create_indicator_properties : public Action {
 public:
  static Named_interface* create_new_interface( std::string& );

 public:
  Create_indicator_properties();
  virtual ~Create_indicator_properties() {}

  /** The parameters for this function are:
   * - the name of the grid
   * - Name of the property to be truncated
   * [ type of truncation: userdefined, decile, quantile, decile ]
   * if userdefined [tresholds]
   * Note that categorical property do not need threshold specifications
   */
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool exec();

 private:

  void get_thresholds_from_data(std::string option);

  std::vector<float> thresholds_;
  std::string group_name_;
  Geostat_grid* grid_;
  Grid_continuous_property* data_prop_;
  std::vector<std::string > id_names_;
  GsTL_project* proj_;
  Error_messages_handler* errors_;
};


class ACTIONS_DECL Downscale_grid : public Action {
 public:
  static Named_interface* create_new_interface( std::string& );

 public:
  Downscale_grid(){}
  virtual ~Downscale_grid() {}

  /** The parameters for this function are:
   * - Grid to be downscale
   * - Name of the new downscaled grif
   * -factor_x, factor_y, factor_z (the donscaling factor (integer))

   */
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool exec(){return true;}


};


class ACTIONS_DECL Upscale_properties : public Action {
 public:
  static Named_interface* create_new_interface( std::string& );

 public:
  Upscale_properties(){}
  virtual ~Upscale_properties() {}

  /** The parameters for this function are:
   * - Upscale grid (target)
   * - Downscale grid (Source)
   * - 0-1 compute the variance (0: no; 1:yes)
   * - Properties to be upscaled
   */
  // todo    * - Upscaling code (md : median; mn mean; vr : variance) e.g. vr:mn:md (separated by a single : )

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool exec();

  std::vector<Grid_continuous_property*> get_upscaled_properties();

private :
    void process_continuous_property(const Grid_continuous_property* prop, std::vector<int>& index);
    void process_categorical_property(const Grid_categorical_property* prop, std::vector<int>& index);
private :
    bool compute_variance_;
    const Cartesian_grid* source_grid_;
    Cartesian_grid* target_grid_;
    std::vector<const Grid_continuous_property*> props_;
    std::vector<Grid_continuous_property*> upscaled_props_;

    int n_cells_;

};



class ACTIONS_DECL Upscale_group : public Action {
 public:
  static Named_interface* create_new_interface( std::string& );

 public:
  Upscale_group(){}
  virtual ~Upscale_group() {}

  /** The parameters for this function are:
   * - Upscale grid (target)
   * - Downscale grid (Source)
   * - 0-1 compute the variance (0: no; 1:yes)
   * - Groups to be upscaled
   */

  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors );
  virtual bool exec();

private :
  Cartesian_grid* target_grid_;
  const Cartesian_grid* source_grid_;

  Upscale_properties upscaler_;
  std::string group_name_;

};


class ACTIONS_DECL Cap_property: public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Cap_property() {} 
  virtual ~Cap_property() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 
};


class ACTIONS_DECL Extract_cell_volume : public Action { 
 public: 
  static Named_interface* create_new_interface( std::string& ); 
 
 public: 
  Extract_cell_volume() {} 
  virtual ~Extract_cell_volume() {}
 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ); 
  virtual bool exec(); 

 protected: 

};



#endif 
