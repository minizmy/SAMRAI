/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   SinusoidalFrontTagger class declaration 
 *
 ************************************************************************/
#ifndef included_SinusoidalFrontTagger
#define included_SinusoidalFrontTagger

#include <string>
#include "SAMRAI/tbox/Pointer.h"
#include "SAMRAI/tbox/Database.h"

/*
 * SAMRAI classes
 */
#include "SAMRAI/pdat/CellData.h"
#include "SAMRAI/pdat/NodeData.h"
#include "SAMRAI/mesh/StandardTagAndInitStrategy.h"
#include "SAMRAI/appu/VisItDataWriter.h"

using namespace std;
using namespace SAMRAI;

/*!
 * @brief Class to tag a sinusoidal "front" in given domain.
 */
class SinusoidalFrontTagger:
   public mesh::StandardTagAndInitStrategy,
   public appu::VisDerivedDataStrategy
{

public:
   /*!
    * @brief Constructor.
    */
   SinusoidalFrontTagger(
      /*! Ojbect name */
      const std::string& object_name,
      const tbox::Dimension& dim,
      /*! Input database */
      SAMRAI::tbox::Database* database = NULL);

   ~SinusoidalFrontTagger();

   //@{ @name SAMRAI::mesh::StandardTagAndInitStrategy virtuals

public:
   /*!
    * @brief Allocate and initialize data for a new level
    * in the patch hierarchy.
    *
    * This is where you implement the code for initialize data on the
    * grid.  Nevermind when it is called or where in the program that
    * happens.  All the information you need to initialize the grid
    * are in the arguments.
    *
    * @see SAMRAI::mesh::StandardTagAndInitStrategy::initializeLevelData()
    */
   virtual void
   initializeLevelData(
      /*! Hierarchy to initialize */
      const tbox::Pointer<SAMRAI::hier::PatchHierarchy> hierarchy,
      /*! Level to initialize */
      const int level_number,
      const double init_data_time,
      const bool can_be_refined,
      /*! Whether level is being introduced for the first time */
      const bool initial_time,
      /*! Level to copy data from */
      const tbox::Pointer<SAMRAI::hier::PatchLevel> old_level =
         tbox::Pointer<SAMRAI::hier::PatchLevel>((0)),
      /*! Whether data on new patch needs to be allocated */
      const bool allocate_data = true);

   virtual void
   resetHierarchyConfiguration(
      /*! New hierarchy */
      tbox::Pointer<SAMRAI::hier::PatchHierarchy> new_hierarchy,
      /*! Coarsest level */ int coarsest_level,
      /*! Finest level */ int finest_level);

   virtual void
   applyGradientDetector(
      const tbox::Pointer<hier::PatchHierarchy> hierarchy,
      const int level_number,
      const double error_data_time,
      const int tag_index,
      const bool initial_time,
      const bool uses_richardson_extrapolation);

   //@}

   void
   initializePatchData(
      hier::Patch& patch,
      const double init_data_time,
      const bool initial_time,
      const bool allocate_data);

   bool
   packDerivedDataIntoDoubleBuffer(
      double* buffer,
      const hier::Patch& patch,
      const hier::Box& region,
      const std::string& variable_name,
      int depth_index) const;

   void
   setTime(
      double time);

public:
   /*!
    * @brief Deallocate internally managed patch data on level.
    */
   void
   deallocatePatchData(
      hier::PatchLevel& level);

   /*!
    * @brief Deallocate internally managed patch data on hierarchy.
    */
   void
   deallocatePatchData(
      hier::PatchHierarchy& hierarchy);

#ifdef HAVE_HDF5
   /*!
    * @brief Tell a VisIt plotter which data to write for this class.
    */
   int
   registerVariablesWithPlotter(
      appu::VisItDataWriter& writer);
#endif

   /*
    * Compute patch data allocated by this class, on a hierarchy.
    */
   void
   computeHierarchyData(
      hier::PatchHierarchy& hierarchy,
      double time);

   /*!
    * @brief Compute distance and tag data for a level.
    */
   void
   computeLevelData(
      const hier::PatchHierarchy& hierarchy,
      const int ln,
      const double time,
      const int dist_id,
      const int tag_id,
      const tbox::Pointer<hier::PatchLevel>& old_level =
         tbox::Pointer<hier::PatchLevel>()) const;

   /*!
    * @brief Compute distance and tag data for a patch.
    */
   void
   computePatchData(
      const hier::Patch& patch,
      const double time,
      pdat::NodeData<double>* dist_data,
      pdat::CellData<int>* tag_data) const;

private:
   std::string d_name;

   const tbox::Dimension d_dim;

   tbox::Pointer<hier::PatchHierarchy> d_hierarchy;

   /*!
    * @brief Period of sinusoid.
    */
   double d_period;

   /*!
    * @brief Initial displacement.
    */
   std::vector<double> d_init_disp;

   /*!
    * @brief Front velocity.
    */
   std::vector<double> d_velocity;

   /*!
    * @brief Amplitude of sinusoid.
    */
   double d_amplitude;

   /*!
    * @brief ghost cell width of internal data.
    *
    * Optional.  Meant to influence gridding parameters.  Defaults to zero.
    */
   hier::IntVector d_ghost_cell_width;

   /*!
    * @brief Number of cells to tag around cells intersecting the front.
    */
   hier::IntVector d_buffer_cells;

   tbox::Array<double> d_buffer_space;

   tbox::Pointer<hier::VariableContext> d_context;

   /*!
    * @brief Distance from the front in the x direction.
    */
   int d_dist_id;
   /*!
    * @brief Value of tag based on distance from front.
    */
   int d_tag_id;

   /*!
    * @brief Whether to allocate data on the mesh.
    */
   bool d_allocate_data;

   /*!
    * @brief Front time.
    */
   double d_time;

   tbox::Pointer<tbox::Timer> t_setup;
   tbox::Pointer<tbox::Timer> t_node_pos;
   tbox::Pointer<tbox::Timer> t_distance;
   tbox::Pointer<tbox::Timer> t_tag_cells;
   tbox::Pointer<tbox::Timer> t_copy;

};

#endif  // included_ssup_SinusoidalFrontTagger