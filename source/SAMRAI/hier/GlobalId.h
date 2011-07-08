/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Globally unique identifier that can be locally determined.
 *
 ************************************************************************/
#ifndef included_hier_GlobalId
#define included_hier_GlobalId

#include "SAMRAI/SAMRAI_config.h"
#include "SAMRAI/hier/LocalId.h"

#include "SAMRAI/tbox/SAMRAI_MPI.h"

#include <iostream>

namespace SAMRAI {
namespace hier {

/*!
 * @brief A locally determined, globally unique identifier.
 *
 * GlobalIds are used as object identifiers in distributed sets of
 * objects, such as Patches and MappedBoxes.  GlobalIds allow objects
 * to be created in parallel while maintaining globally-unique
 * identifiers for those objects.
 *
 * GlobalId includes an owner rank and a LocalId.  A GlobalId is
 * typically generated only by the owner, so global uniqueness is
 * guaranteed by uniqueness of the LocalId portion.
 *
 * Comparison operators are provided to define sorted ordering of
 * objects.  The owners and LocalIds are used for all comparisons.
 * The owner ranks are compared first, followed by the LocalIds.
 */
class GlobalId
{

public:

   /*!
    * @brief Default constructor is non-initializing.
    *
    * Sets rank to an invalid value provided by SAMRAI_MPI.
    *
    * The object can be initialized using the assignment operator or
    * the non-const versions of the getOwnerRank() and getLocalId() methods.
    */
   /*
    * TODO: Replace the magic number with a method returning the
    * official invalid value.  See artifact artf13924.
    */
   GlobalId();

   /*!
    * @brief Initializing constructor.
    */
   explicit GlobalId(
      const LocalId &local_id,
      const int owner_rank);

   /*!
    * @brief Copy constructor.
    *
    * @param[in] other
    */
   GlobalId(
      const GlobalId& other);

   /*!
    * @brief Destructor.
    */
   virtual ~GlobalId(
      void);

   /*!
    * @brief Access the owner rank.
    */
   int&
   getOwnerRank();

   /*!
    * @brief Access the owner rank.
    */
   const int&
   getOwnerRank() const;

   /*!
    * @brief Access the LocalId.
    */
   LocalId&
   getLocalId();

   /*!
    * @brief Access the LocalId.
    */
   const LocalId&
   getLocalId() const;


   //@{

   //! @name Comparison operators

   /*!
    * @brief Equality operator.
    *
    * All comparison operators use the owner and rank.  Compare the
    * owner values first; if they compare equal, compare the LocalId
    * next.
    */
   bool
   operator == (
      const GlobalId& r) const;

   /*!
    * @brief Inequality operator.
    *
    * See note on comparison for operator==(const GlobalId&);
    */
   bool
   operator != (
      const GlobalId& r) const;

   /*!
    * @brief Less-than operator.
    *
    * See note on comparison for operator==(const GlobalId&);
    */
   bool
   operator < (
      const GlobalId& r) const;

   /*!
    * @brief Greater-than operator.
    *
    * See note on comparison for operator==(const GlobalId&);
    */
   bool
   operator > (
      const GlobalId& r) const;

   /*!
    * @brief Less-than-or-equal-to operator.
    *
    * See note on comparison for operator==(const GlobalId&);
    */
   bool
   operator <= (
      const GlobalId& r) const;

   /*!
    * @brief Greater-thanor-equal-to operator.
    *
    * See note on comparison for operator==(const GlobalId&);
    */
   bool
   operator >= (
      const GlobalId& r) const;

   //@}

   /*!
    * @brief Format and insert rank and LocalId into a stream.
    */
   friend std::ostream&
   operator << (
      std::ostream& co,
      const GlobalId& r);

private:
   /*!
    * @brief Rank of owner of this id.
    */
   int d_owner_rank;

   /*!
    * @brief Local id on the owner process.
    */
   LocalId d_local_id;

};

}
}

#ifdef SAMRAI_INLINE
#include "SAMRAI/hier/GlobalId.I"
#endif

#endif  // included_hier_GlobalId