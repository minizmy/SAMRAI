/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Templated operations for real face-centered patch data. 
 *
 ************************************************************************/

#ifndef included_math_PatchFaceDataOpsReal
#define included_math_PatchFaceDataOpsReal

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/FaceData.h"
#include "SAMRAI/math/PatchFaceDataBasicOps.h"
#include "SAMRAI/math/PatchFaceDataMiscellaneousOpsReal.h"
#include "SAMRAI/math/PatchFaceDataNormOpsReal.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/Patch.h"
#include "SAMRAI/tbox/PIO.h"
#include "SAMRAI/tbox/Pointer.h"

#include <iostream>

namespace SAMRAI {
namespace math {

/**
 * Class PatchFaceDataOpsReal provides a collection of operations
 * to manipulate float and double numerical face-centered patch data.  The
 * operations include basic arithmetic, norms and ordering, and assorted
 * miscellaneous operations.  With the assertion of a few basic routines,
 * this class inherits its interface (and thus its functionality) from the
 * base classes PatchFaceDataBasicOps, PatchFaceDataNormOpsReal,
 * and PatchFaceDataMiscellaneousOpsReal from which it is derived.  The
 * name of each of these base classes is indicative of the set of
 * face-centered patch data operations that it provides.
 *
 * Note that this templated class should only be used to instantiate
 * objects with double or float as the template parameter.  A similar set of
 * operations is implemented for complex and integer patch data in the classes
 * PatchFaceDataOpsComplex and PatchFaceDataOpsInteger,
 * repsectively.
 *
 * @see math::PatchFaceDataBasicOps
 * @see math::PatchFaceDataMiscellaneousOpsReal
 * @see math::PatchFaceDataNormOpsReal
 */

template<class TYPE>
class PatchFaceDataOpsReal:
   public tbox::DescribedClass,
   public PatchFaceDataBasicOps<TYPE>,
   public PatchFaceDataMiscellaneousOpsReal<TYPE>,
   public PatchFaceDataNormOpsReal<TYPE>
{
public:
   /**
    * Empty constructor and destructor.
    */
   PatchFaceDataOpsReal();

   virtual ~PatchFaceDataOpsReal<TYPE>() {}

   /**
    * Copy dst data to src data over given box.
    */
   void
   copyData(
      tbox::Pointer<pdat::FaceData<TYPE> >& dst,
      const tbox::Pointer<pdat::FaceData<TYPE> >& src,
      const hier::Box& box) const;

   /**
    * Swap pointers for patch data objects.  Objects are checked for
    * consistency of depth, box, and ghost box.
    */
   void
   swapData(
      tbox::Pointer<hier::Patch> patch,
      const int data1_id,
      const int data2_id) const;

   /**
    * Print data entries over given box to given output stream.
    */
   void
   printData(
      const tbox::Pointer<pdat::FaceData<TYPE> >& data,
      const hier::Box& box,
      std::ostream& s = tbox::plog) const;

   /**
    * Initialize data to given scalar over given box.
    */
   void
   setToScalar(
      tbox::Pointer<pdat::FaceData<TYPE> >& dst,
      const TYPE& alpha,
      const hier::Box& box) const;

private:
   // The following are not implemented:
   PatchFaceDataOpsReal(
      const PatchFaceDataOpsReal<TYPE>&);
   void
   operator = (
      const PatchFaceDataOpsReal<TYPE>&);

};

}
}

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "SAMRAI/math/PatchFaceDataOpsReal.C"
#endif

#endif