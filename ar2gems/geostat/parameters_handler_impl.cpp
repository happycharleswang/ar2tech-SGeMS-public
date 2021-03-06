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

#include <geostat/parameters_handler_impl.h>
#include <utils/string_manipulation.h>
#include <utils/gstl_messages.h>


Named_interface*
Parameters_handler_xml::create_new_interface( std::string& param ) {
  return new Parameters_handler_xml( param );
}

Parameters_handler_xml::Parameters_handler_xml( const std::string& parameters ) {
  is_ready_ = handle( parameters );
}


bool Parameters_handler_xml::handle( const std::string& parameters ) {
  QString errors;
  std::string tmp;
  bool parsed = xml_params_.setContent( QString(parameters.c_str()), true, &errors );
  if( !parsed ) {
    QByteArray tmp = errors.toLatin1();

    appli_warning( "Error while parsing the following XML string: " << std::endl
		  << parameters );  
    appli_warning( "The reported errors are: " << std::endl
		  << tmp.constData() ) ;
    is_ready_ = false;
    return false;
  }

  is_ready_ = true;
  root_element_ = xml_params_.documentElement();
  appli_assert( !root_element_.isNull() );   

  return true;
}


std::string
Parameters_handler_xml::value( const std::string& parameter ) const {
  return get_value( root_element_, parameter );

//  QDomElement el = xml_params_.documentElement();
//  appli_assert( !el.isNull() ); 
//  return get_value( xml_params_.documentElement(), parameter );
}


QDomElement
Parameters_handler_xml::xmlDomElement( const std::string& parameter ) const {
	if(parameter.empty()) return root_element_;
  QDomNodeList nodes = root_element_.elementsByTagName( parameter.c_str() );
    if( nodes.count() == 0 ) {
      appli_warning( "No element called \"" <<  parameter << "\"" << std::endl 
		    << "Returning an empty element" );
      return QDomElement();
    }
  return nodes.item(0).toElement();
//  QDomElement elem = root_element_.firstChildElement(parameter.c_str());
	//return elem;
}


std::string
Parameters_handler_xml::get_value( QDomElement start, 
				   const std::string& param ) const {
  String_Op::string_pair split = String_Op::split_string( param, "/" );

  if( split.second.empty() ) {
    // we're at the end of the path. Separate the tag name from a potential
    // attribute name (they are separated by a dot), go to the corresponding
    // DomElement and return the specified attribute value.
    split = String_Op::split_string( split.first, "." );
    if( split.second.empty() ) return "";

    QDomNodeList nodes = start.elementsByTagName( split.first.c_str() );
    if( nodes.count() == 0 ) {
      appli_warning( "No element called \"" <<  split.first << "\"" << std::endl 
		    << "Returning an empty string" );
      return "";
    }
    QDomNode current_node = nodes.item(0);
    appli_assert( current_node.isElement() );
    start = current_node.toElement();
 
    if( !start.hasAttribute( split.second.c_str() ) ) {
      QByteArray tmp = start.tagName().toLatin1();
      appli_warning( "Element \"" << tmp.constData() << "\" has no Attribute called \""
		    <<  split.second << "\"" << std::endl 
		    << "Returning an empty string" );
      return "";
    }
    QByteArray tmp = start.attribute( split.second.c_str() ).toLatin1();
    return std::string( tmp.constData() );
    
  }
  else {
    // we're not at the end of the path: get the next DomElement in the path
    // and recursively call go_to_element

    QDomNodeList nodes = start.elementsByTagName( split.first.c_str() );
    if( nodes.count() == 0 ) {
      appli_warning( "No element called \"" <<  split.first << "\"" << std::endl 
		    << "Returning an empty string" );
      return "";
    }
    QDomNode current_node = nodes.item(0);
    appli_assert( current_node.isElement() );
    start = current_node.toElement();

    return get_value( start, split.second );
  }
}




