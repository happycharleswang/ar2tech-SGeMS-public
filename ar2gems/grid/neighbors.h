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


#ifndef __GSTLAPPLI_GRID_MODEL_NEIGHBORS_H__ 
#define __GSTLAPPLI_GRID_MODEL_NEIGHBORS_H__ 
 
#include <grid/common.h>
#include <math/gstlvector.h> 
#include <grid/geovalue.h>  
#include <vector> 
 
 

/** Neighbors class contains neighborhood nodes returned by Neighborhood class. 
 */ 
class GRID_DECL Neighbors 
{ 
 public: 
   typedef Geovalue value_type;
  typedef std::vector<Geovalue>::iterator iterator; 
  typedef std::vector<Geovalue>::const_iterator const_iterator; 
   
 
 public:
   /* some inline APIs from std::vector<> */
   inline bool empty() const;
   inline void push_back (const Geovalue & node);
   inline void pop_back();
   template <class InputIterator>
   inline void insert(Neighbors::iterator position, InputIterator first, InputIterator last);
   inline Geovalue & operator[] (GsTLInt n);
   inline void clear();
   inline void reserve(GsTLInt n);
   inline Neighbors::iterator erase(Neighbors::iterator first, Neighbors::iterator last);
   inline void resize(GsTLInt n, Geovalue val = Geovalue());
   inline GsTLInt size() const;
   inline Geovalue & front();
   inline Geovalue & back();

   /* APIs from Neighborhood */
   inline bool is_empty() const;
   inline Neighbors::iterator begin();
   inline Neighbors::const_iterator begin() const;
   inline Neighbors::iterator end();
   inline Neighbors::const_iterator end() const;
   Geovalue center() const;

   inline bool is_valid() const;
   inline void set_valid(bool on);


   /* new APIs */
   // Set center
   void center_is(const Geovalue & _center);

   // Get the number of informed neighbors
   GsTLInt number_informed_neighbors();

   // Get the number of harddata neighbors
   GsTLInt number_harddata_neighbors();


   Neighbors();
   ~Neighbors();

   
 protected: 
  Geovalue center_;
  std::vector<Geovalue> neighbors_; 
  GsTLInt number_informed_neighbors_; // number of informed neighbors
  GsTLInt number_harddata_neighbors_; // number of harddata neighbors
  bool neighbors_vector_changed_; // keep track of modifications on neighbors_ vector

  bool is_valid_;
}; 


/* inline methods */
bool 
Neighbors::empty() const
{
  return neighbors_.empty();
}

bool 
Neighbors::is_empty() const
{
  return neighbors_.empty();
}

void 
Neighbors::push_back (const Geovalue & node)
{
  neighbors_.push_back(node);
  neighbors_vector_changed_ = true;
}

void 
Neighbors::pop_back()
{
  neighbors_.pop_back();
}

template <class InputIterator>
void 
Neighbors::insert(Neighbors::iterator position, InputIterator first, InputIterator last)
{
  neighbors_.insert(position, first, last);
}

Geovalue & 
Neighbors::operator[] (GsTLInt n)
{
  return neighbors_[n];
}

void 
Neighbors::clear()
{
  neighbors_.clear();
  neighbors_vector_changed_ = true;
}

void 
Neighbors::reserve(GsTLInt n)
{
  neighbors_.reserve(n);
}

Neighbors::iterator 
Neighbors::erase(Neighbors::iterator first, Neighbors::iterator last)
{
  return neighbors_.erase(first, last);
}

void 
Neighbors::resize(GsTLInt n, Geovalue val)
{
  neighbors_.resize(n, val);
}

GsTLInt
Neighbors::size() const
{
  return neighbors_.size();
}

Geovalue & 
Neighbors::front()
{
  return neighbors_[0];
}

Geovalue & 
Neighbors::back()
{
  return neighbors_[neighbors_.size() - 1];
}

Neighbors::iterator 
Neighbors::begin()
{
  return neighbors_.begin();
}

Neighbors::const_iterator 
Neighbors::begin() const
{
  return neighbors_.cbegin();
}

Neighbors::iterator 
Neighbors::end()
{
  return neighbors_.end();
}

Neighbors::const_iterator 
Neighbors::end() const
{
  return neighbors_.cend();
}

bool Neighbors::is_valid() const{ return is_valid_;}
void Neighbors::set_valid(bool on) { is_valid_ = on;}

#endif 
