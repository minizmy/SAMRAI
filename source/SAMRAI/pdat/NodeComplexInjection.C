/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Constant averaging operator for node-centered complex data on
 *                a  mesh. 
 *
 ************************************************************************/

#ifndef included_pdat_NodeComplexInjection_C
#define included_pdat_NodeComplexInjection_C

#include "SAMRAI/pdat/NodeComplexInjection.h"
#include "SAMRAI/tbox/Complex.h"

#include <float.h>
#include <math.h>
#include "SAMRAI/tbox/Utilities.h"
#include "SAMRAI/hier/Index.h"
#include "SAMRAI/pdat/NodeData.h"
#include "SAMRAI/pdat/NodeVariable.h"

/*
 *************************************************************************
 *                                                                       *
 * External declarations for FORTRAN  routines.                          *
 *                                                                       *
 *************************************************************************
 */
extern "C" {

#ifdef __INTEL_COMPILER
#pragma warning (disable:1419)
#endif

// in concoarsen1d.f:
void F77_FUNC(conavgnodecplx1d, CONAVGNODECPLX1D) (const int &, const int &,
   const int &, const int &,
   const int &, const int &,
   const int *,
   const dcomplex *, dcomplex *);
// in concoarsen2d.f:
void F77_FUNC(conavgnodecplx2d, CONAVGNODECPLX2D) (const int &, const int &,
   const int &, const int &,
   const int &, const int &, const int &, const int &,
   const int &, const int &, const int &, const int &,
   const int *,
   const dcomplex *, dcomplex *);
// in concoarsen3d.f:
void F77_FUNC(conavgnodecplx3d, CONAVGNODECPLX3D) (const int &, const int &,
   const int &,
   const int &, const int &, const int &,
   const int &, const int &, const int &,
   const int &, const int &, const int &,
   const int &, const int &, const int &,
   const int &, const int &, const int &,
   const int *,
   const dcomplex *, dcomplex *);
}

namespace SAMRAI {
namespace pdat {

NodeComplexInjection::NodeComplexInjection(
   const tbox::Dimension& dim):
   hier::CoarsenOperator(dim, "CONSTANT_COARSEN")
{
}

NodeComplexInjection::~NodeComplexInjection()
{
}

bool NodeComplexInjection::findCoarsenOperator(
   const tbox::Pointer<hier::Variable>& var,
   const std::string& op_name) const
{
   TBOX_DIM_ASSERT_CHECK_ARGS2(*this, *var);

   const tbox::Pointer<NodeVariable<dcomplex> > cast_var(var);
   if (!cast_var.isNull() && (op_name == getOperatorName())) {
      return true;
   } else {
      return false;
   }
}

int NodeComplexInjection::getOperatorPriority() const
{
   return 0;
}

hier::IntVector
NodeComplexInjection::getStencilWidth() const {
   return hier::IntVector::getZero(getDim());
}

void NodeComplexInjection::coarsen(
   hier::Patch& coarse,
   const hier::Patch& fine,
   const int dst_component,
   const int src_component,
   const hier::Box& coarse_box,
   const hier::IntVector& ratio) const
{
   TBOX_DIM_ASSERT_CHECK_ARGS5(*this, coarse, fine, coarse_box, ratio);

   tbox::Pointer<NodeData<dcomplex> >
   fdata = fine.getPatchData(src_component);
   tbox::Pointer<NodeData<dcomplex> >
   cdata = coarse.getPatchData(dst_component);

   TBOX_ASSERT(!fdata.isNull());
   TBOX_ASSERT(!cdata.isNull());
   TBOX_ASSERT(cdata->getDepth() == fdata->getDepth());

   const hier::Index filo = fdata->getGhostBox().lower();
   const hier::Index fihi = fdata->getGhostBox().upper();
   const hier::Index cilo = cdata->getGhostBox().lower();
   const hier::Index cihi = cdata->getGhostBox().upper();

   const hier::Index ifirstc = coarse_box.lower();
   const hier::Index ilastc = coarse_box.upper();

   for (int d = 0; d < cdata->getDepth(); d++) {
      if (getDim() == tbox::Dimension(1)) {
         F77_FUNC(conavgnodecplx1d, CONAVGNODECPLX1D) (ifirstc(0), ilastc(0),
            filo(0), fihi(0),
            cilo(0), cihi(0),
            &ratio[0],
            fdata->getPointer(d),
            cdata->getPointer(d));
      } else if (getDim() == tbox::Dimension(2)) {
         F77_FUNC(conavgnodecplx2d, CONAVGNODECPLX2D) (ifirstc(0), ifirstc(1),
            ilastc(0), ilastc(1),
            filo(0), filo(1), fihi(0), fihi(1),
            cilo(0), cilo(1), cihi(0), cihi(1),
            &ratio[0],
            fdata->getPointer(d),
            cdata->getPointer(d));
      } else if (getDim() == tbox::Dimension(3)) {
         F77_FUNC(conavgnodecplx3d, CONAVGNODECPLX3D) (ifirstc(0), ifirstc(1),
            ifirstc(2),
            ilastc(0), ilastc(1), ilastc(2),
            filo(0), filo(1), filo(2),
            fihi(0), fihi(1), fihi(2),
            cilo(0), cilo(1), cilo(2),
            cihi(0), cihi(1), cihi(2),
            &ratio[0],
            fdata->getPointer(d),
            cdata->getPointer(d));
      } else {
         TBOX_ERROR(
            "NodeComplexInjection::coarsen dimension > 3 not supported"
            << std::endl);
      }
   }
}

}
}
#endif