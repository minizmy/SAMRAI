/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Factory class for creating side data objects 
 *
 ************************************************************************/

#ifndef included_pdat_SideDataFactory
#define included_pdat_SideDataFactory

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/MultiblockSideDataTranslator.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/BoxGeometry.h"
#include "SAMRAI/hier/IntVector.h"
#include "SAMRAI/hier/PatchDataFactory.h"
#include "SAMRAI/tbox/Complex.h"
#include "SAMRAI/tbox/Pointer.h"

namespace SAMRAI {
namespace pdat {

/**
 * Class SideDataFactory is a factory class used to allocate new
 * instances of SideData objects.  It is a subclass of the patch
 * data factory class and side data is a subclass of patch data.  Both
 * the factory and data classes are templated on the type of the contained
 * object (e.g., double or int).
 *
 * Note that it is possible to create a side data factory to allocate
 * and manage data for cell sides associated with a single coordinate
 * direction only.  See the constructor for more information.
 *
 * @see pdat::SideData
 * @see pdat::PatchDataFactory
 */

template<class TYPE>
class SideDataFactory:public hier::PatchDataFactory
{
public:
   /**
    * The constructor for the side data factory class.  The ghost cell
    * width, depth (number of components), and fine boundary representation arguments
    * give the defaults for all edge data objects created with this factory.
    * Also, the default data allocation scheme is to generate storage for sides
    * in all coordinate directions (default integer vector of all 1's).  To
    * use this factory to manage side data objects for sides associated
    * with a single direction only, provide the directions vector argument.
    * A zero entry indicates that data for that direction is not wanted.
    * Otherwise, data will be created for that direction.  See the
    * SideVariable<DIM> class header file for more information.
    */
   explicit SideDataFactory(
      int depth,
      const hier::IntVector& ghosts,
      bool fine_boundary_represents_var);

   /**
    * Same as previous constructor but with directions vector of 1's
    */
   explicit SideDataFactory(
      int depth,
      const hier::IntVector& ghosts,
      bool fine_boundary_represents_var,
      const hier::IntVector& directions);

   /**
    * Virtual destructor for the side data factory class.
    */
   virtual ~SideDataFactory<TYPE>();

   /**
    * @brief Abstract virtual function to clone a patch data factory.
    *
    * This will return a new instantiation of the abstract factory
    * with the same properties.  The properties of the cloned factory
    * can then be changed without modifying the original.
    *
    * @param ghosts default ghost cell width for concrete classes created from
    * the factory.
    */
   virtual tbox::Pointer<hier::PatchDataFactory>
   cloneFactory(
      const hier::IntVector& ghosts);

   /**
    * Virtual factory function to allocate a concrete side data object.
    * The default information about the object (e.g., ghost cell width)
    * is taken from the factory.
    */
   virtual tbox::Pointer<hier::PatchData>
   allocate(
      const hier::Patch& patch) const;

   /**
    * Allocate the box geometry object associated with the patch data.
    * This information will be used in the computation of intersections
    * and data dependencies between objects.
    */
   virtual tbox::Pointer<hier::BoxGeometry>
   getBoxGeometry(
      const hier::Box& box) const;

   /**
    * Get the depth (number of components).  This is the depth that
    * will be used in the instantiation of side data objects.
    */
   int
   getDepth() const;

   /**
    * Return constant reference to vector describing which coordinate
    * directions have data associated with this side data object.
    * A vector entry of zero indicates that there is no data array
    * allocated for the corresponding coordinate direction.  A non-zero
    * value indicates that a valid data array is maintained for that
    * coordinate direction.
    */
   const hier::IntVector&
   getDirectionVector() const;

   /**
    * Calculate the amount of memory needed to store the side data object,
    * including object data and dynamically allocated data.
    */
   virtual size_t
   getSizeOfMemory(
      const hier::Box& box) const;

   /**
    * Return a boolean value indicating how data for the side quantity will be treated
    * on coarse-fine interfaces.  This value is passed into the constructor.  See
    * the FaceVariable<DIM> class header file for more information.
    */
   bool fineBoundaryRepresentsVariable() const {
      return d_fine_boundary_represents_var;
   }

   /**
    * Return true since the side data index space extends beyond the interior of
    * patches.  That is, side data lives on patch borders.
    */
   bool dataLivesOnPatchBorder() const {
      return true;
   }

   /**
    * Return whether it is valid to copy this SideDataFactory to the
    * supplied destination patch data factory.  It will return true if
    * dst_pdf is SideDataFactory or OutersideDataFactory, false otherwise.
    */
   bool
   validCopyTo(
      const tbox::Pointer<hier::PatchDataFactory>& dst_pdf) const;

   /**
    * Return pointer to a multiblock data translator
    */
   hier::MultiblockDataTranslator *
   getMultiblockDataTranslator();

private:
   int d_depth;
   bool d_fine_boundary_represents_var;
   hier::IntVector d_directions;

   MultiblockSideDataTranslator<TYPE>* d_mb_trans;

};

}
}
#ifdef SAMRAI_INLINE
#include "SAMRAI/pdat/SideDataFactory.I"
#endif

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "SAMRAI/pdat/SideDataFactory.C"
#endif

#endif