/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   hier 
 *
 ************************************************************************/

#ifndef included_pdat_OuterfaceVariable
#define included_pdat_OuterfaceVariable

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/hier/Variable.h"
#include "SAMRAI/tbox/Complex.h"

#include <string>

namespace SAMRAI {
namespace pdat {

/*!
 * @brief Class OuterfaceVariable<DIM> is a templated variable class
 * used to define face-centered data quantities only on patch boundaries.
 * It is a subclass of hier::Variable and is templated on the type
 * of the underlying data (e.g., double, int, bool, etc.).
 *
 * Note that the data layout in the outerface data arrays matches the corresponding
 * array sections provided by the face data implementation. See header file for
 * the OuterfaceData<DIM> class for a more detailed description of the data layout.
 *
 * IMPORTANT: The class OutersideVariable<DIM> and associated "outerside
 * data" classes define the same storage as this outerface variable class,
 * except that the individual array indices are not permuted in the outerside
 * data type.
 *
 * @see pdat::FaceData
 * @see pdat::OuterfaceData
 * @see pdat::OuterfaceDataFactory
 * @see hier::Variable
 */

template<class TYPE>
class OuterfaceVariable:public hier::Variable
{
public:
   /*!
    * @brief Create an outerface variable object having properties
    * specified by the name and depth (i.e., number of data values
    * at each index location).  The default depth is one.
    *
    * Note that The ghost cell width for all outerface data is currently
    * fixed at zero; this may be changed in the future if needed.
    */
   explicit OuterfaceVariable(
      const tbox::Dimension& dim,
      const std::string& name,
      int depth = 1);

   /*!
    * @brief Virtual destructor for outerface variable objects.
    */
   virtual ~OuterfaceVariable<TYPE>();

   /*!
    * @brief Return a boolean true value indicating that fine patch
    * values take precedence on coarse-fine interfaces.
    */
   bool fineBoundaryRepresentsVariable() const {
      return true;
   }

   /*!
    * @brief Return true indicating that outerface data
    * exists on the patch boundary.
    */
   bool dataLivesOnPatchBorder() const {
      return true;
   }

   /*!
    * @brief Return the the depth (number of components).
    */
   int
   getDepth() const;

private:
   // neither of the following functions are implemented
   OuterfaceVariable(
      const OuterfaceVariable<TYPE>&);
   void
   operator = (
      const OuterfaceVariable<TYPE>&);
};

}
}

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "SAMRAI/pdat/OuterfaceVariable.C"
#endif

#endif