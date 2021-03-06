/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2017 Lawrence Livermore National Security, LLC
 * Description:   Singleton database class for Umpire allocators 
 *
 ************************************************************************/

#ifndef included_tbox_AllocatorDatabase
#define included_tbox_AllocatorDatabase

#include "SAMRAI/SAMRAI_config.h"

#ifdef HAVE_UMPIRE

#include "SAMRAI/tbox/StartupShutdownManager.h"

#include "umpire/Allocator.hpp"
#include "umpire/TypedAllocator.hpp"

namespace SAMRAI {
namespace tbox {

/*!
 * @brief Singleton class holding allocators for common SAMRAI operation
 *
 * This class provides access to Umpire allocators that are used to allocate
 * data for specific operations that occur during runs of applications
 * using SAMRAI.  The main intent is to support coordination of data
 * allocations on the host and device when running with cuda-based GPU
 * features enabled.  When not running with GPUs, these allocators will
 * default to do regular allocations of CPU memory.
 *
 * Allocators defined here are:
 *
 * Device pool--A pool of memory on the device that can be used for
 * temporary data that is created inside of kernels running on the GPU.
 *
 * Stream allocator--Allocator for pinned memory for MPI buffers used
 * in communications launched by tbox::Schedule, most notably during the
 * execution of refine and coarsen schedules.
 *
 * Tag allocator--Allocator for memory for the tag data object created and
 * owned by GriddingAlgorithm and provided to applications.
 *
 * These allocators can be overriden by creating Umpire allocators with the
 * appropriate name prior to calling tbox::SAMRAIManager::initialize().
 * The names are samrai::temporary_data_allocator, samrai::stream_allocator,
 * and samrai::tag_allocator.  Please see the Umpire documentation for details
 * on how to create new allocators.
 */

class AllocatorDatabase
{
public:
   /*!
    * @brief Static accessor function to get pointer to the instance of
    * the singleton object.
    */
   static AllocatorDatabase* getDatabase();

   /*!
    * @brief Initialize the allocators.
    */
   void initialize();

   /*!
    * @brief Get the device pool allocator.
    */
   umpire::Allocator getDevicePool();

   /*!
    * @brief Get the stream allocator.
    */
   umpire::TypedAllocator<char> getStreamAllocator();

   /*!
    * @brief Get the allocator for tag data.
    */
   umpire::Allocator getTagAllocator();

protected:
   AllocatorDatabase() = default;

   virtual ~AllocatorDatabase();

private:
   static void startupCallback();
   static void shutdownCallback();

   static AllocatorDatabase* s_allocator_database_instance;

   static StartupShutdownManager::Handler
   s_startup_handler;
};

}
}

#endif
#endif
