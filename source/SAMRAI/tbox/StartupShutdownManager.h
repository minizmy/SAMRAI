/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Manager for startup and shutdown routines to be called at program start and exit 
 *
 ************************************************************************/
#ifndef included_tbox_StartupShutdownManager
#define included_tbox_StartupShutdownManager

#include "SAMRAI/SAMRAI_config.h"

#include "cassert"

namespace SAMRAI {
namespace tbox {

/*!
 * @brief Class StartupShutdownManager is a utility for managing
 * callbacks invoked at program and problem startup and completion.
 *
 * There are four steps in the lifecycle of managed objects.
 *
 * An object is initialized once at the start of a run and a
 * corresponding finalization is done once at the end of the run.
 * These methods can be used to create and destroy static state that
 * is independent of problem; e.g., when running multiple problems
 * during a single program (i.e., main) execution.
 *
 * The lifecycle also has a startup/shutdown step.   This is invoked
 * at least once per run but may be invoked multiple times if
 * multiple SAMRAI problems are being run within a single execution of the
 * main function.  These methods should setup/teardown any state that
 * is problem dependent.
 *
 * In order to address dependencies between classes a Handler provides
 * a priority (ordering) using the getPriority method.  Ordering within
 * the same priority is undefined.
 *
 * The StartupShutdowManger::AbstractHandler defines the interface for
 * the class that is registered.  StartupShutdowManger::Handler is a
 * helper class provided to simplify the use of the StartupShutdownManager.
 * One may simply create a static instance of the Handler class, providing
 * the constructor with callbacks to invoke and priority to use.
 *
 * The StartupShutdownManger is normally managed by the SAMRAIManager
 * class.
 */

class StartupShutdownManager
{

public:
   /*!
    * @brief Abstract base class for handler interface.
    *
    * Defines the four methods to invoke in the lifecycle and four
    * methods to indicate if the interface should be called for that stage.
    *
    * Note the odd looking "has" methods are needed for some error checking.
    */
   class AbstractHandler
   {

public:
      /*!
       * @brief Virtual destructor since class has virtual methods.
       */
      virtual ~AbstractHandler() { }

      /*!
       * @brief Method that is invoked on initialize.
       *
       * This is done once per run.
       *
       * Only called by StartupShutdownManager.
       */
      virtual void
      initialize(
         void) = 0;

      /*!
       * @brief Method that is invoked on startup.
       *
       * This may be done more than once per run.
       *
       * Only called by StartupShutdownManager.
       */
      virtual void
      startup(
         void) = 0;

      /*!
       * @brief Method that is invoked on shutdown.
       *
       * This may be done more than once per run.
       *
       * Only called by StartupShutdownManager.
       */
      virtual void
      shutdown(
         void) = 0;

      /*!
       * @brief Method that is invoked on finalize.
       *
       * This is done once per run.
       *
       * Only called by StartupShutdownManager.
       */
      virtual void
      finalize(
         void) = 0;

      /*!
       * @brief Get the Priority of this handler.
       */
      virtual unsigned char
      getPriority(
         void) = 0;

      /*!
       * @brief Query if handler has initialize callback function.
       */
      virtual bool
      hasInitialize() = 0;

      /*!
       * @brief Query if handler has startup callback function.
       */
      virtual bool
      hasStartup() = 0;

      /*!
       * @brief Query if handler has shutdown callback function.
       */
      virtual bool
      hasShutdown() = 0;

      /*!
       * @brief Query if handler has finalize callback function.
       */
      virtual bool
      hasFinalize() = 0;
   };

   /*!
    * @brief Standard implementation of a Startup/Shutdown handler.
    *
    * This class is provided to simplify construction of a
    * handler class for standard SAMRAI startup/shutdown uses.  This
    * handler registers itself with the StartupShutdownManager on
    * construction.  On construction the methods to invoke for each
    * step of the lifecycle are supplied.  The method may be 0
    * indicating that the managed class does not require anything to
    * be executed at that step.
    *
    * Example usage shows how the static initialization of the
    * s_startup_shutdown_handler is used to provide a simple
    * method of registering a class with the manager that requires
    * startup and shutdown but not initialization or finalization.
    *
    * \code
    * classs StartupShutdownExample {
    * public:
    *    static void startupCallback(void) {
    *       // stuff to do for class initialization.
    *    }
    *
    *    static void shutdownCallback(void) {
    *       // stuff to do for class destruction.
    *    }
    * private:
    * static StartupShutdownManger::Handler<StartupShutdownExample>
    *                            s_startup_shutdown_handler;
    * }
    *
    * static unsigned short handlerPriority = 150;
    * StartupShutdownManger::Handler<StartupShutdownExample>
    *    s_startup_shutdown_handler(0,
    *                               StartupShutdownExample::startupCallback,
    *                               StartupShutdownExample::shutdownCallback,
    *                               0,
    *                               handlerPriority);
    * \endcode
    *
    *
    * Note that this mechanism does NOT work for templated classes as
    * static variables in template classes are only initialized if
    * used.  For template classes a slightly different mechanism using
    * a static in a method is used.  Similar to the Meyer singleton
    * implementation.  \see{tbox::List} class as an example.  This
    * method has the disadvantage of having to check on each object
    * construction if the static has been created (it is done under
    * the hood by the compiler but still exists).  At this time we are
    * not aware of a method to invoke a block of code only once for a
    * template class.
    *
    * TODO:  Should implemenation be moved out of the header?
    *
    */
   class Handler:public AbstractHandler
   {
public:
      /*!
       * @brief Construct a handler with the callbacks provided and
       * specified priority.
       *
       * The callback function pointers should be NULL for callbacks not
       * required for a particular class.
       *
       * @param[in] initialize   Initialization callback function.
       * @param[in] startup      Startup callback function.
       * @param[in] shutdown     Shutdown callback function.
       * @param[in] finalize     Finalization callback function.
       * @param[in] priority
       *
       */
      Handler(
         void(*initialize)(void),
         void(*startup)(void),
         void(*shutdown)(void),
         void(*finalize)(void),
         unsigned char priority):
         d_initialize(initialize),
         d_startup(startup),
         d_shutdown(shutdown),
         d_finalize(finalize),
         d_priority(priority)
      {
         StartupShutdownManager::registerHandler(this);
      }

      /*!
       * @brief Destructor.
       */
      ~Handler() {
      }

      /*!
       * @brief Call initialize callback function.
       */
      void initialize(
         void) {
         if (d_initialize) {
            (*d_initialize)();
         }
      }

      /*!
       * @brief Call startup callback function.
       */
      void startup(
         void) {
         if (d_startup) {
            (*d_startup)();
         }
      }

      /*!
       * @brief Call shutdown callback function.
       */
      void shutdown(
         void) {
         if (d_shutdown) {
            (*d_shutdown)();
         }
      }

      /*!
       * @brief Call finalize callback function.
       */
      void finalize(
         void) {
         if (d_finalize) {
            (*d_finalize)();
         }
      }

      /*!
       * @brief Get the priority.
       */
      unsigned char getPriority(
         void) {
         return d_priority;
      }

      /*!
       * @brief Query if initialize function has been provided.
       */
      bool hasInitialize() {
         return d_initialize != 0;
      }

      /*!
       * @brief Query if startup function has been provided.
       */
      bool hasStartup() {
         return d_startup != 0;
      }

      /*!
       * @brief Query if shutdown function has been provided.
       */
      bool hasShutdown() {
         return d_shutdown != 0;
      }

      /*!
       * @brief Query if finalize function has been provided.
       */
      bool hasFinalize() {
         return d_finalize != 0;
      }

private:

      /*!
       * @brief Default constructor is private.
       *
       * TODO:  Should this be unimplemented?
       */
      Handler() {
      }

      /*!
       * @brief Initialize function.
       */
      void (* d_initialize)(
         void);

      /*!
       * @brief Startup function.
       */
      void (* d_startup)(
         void);

      /*!
       * @brief Shutdown function.
       */
      void (* d_shutdown)(
         void);

      /*!
       * @brief Finalize function.
       */
      void (* d_finalize)(
         void);

      /*!
       * @brief Priority of the handler.
       */
      unsigned char d_priority;

   };

   /*!
    * @brief Register a handler with the StartupShutdownManager.
    *
    * The AbstractHandler interface defines callback methods that will be
    * invoked on SAMRAI initialize, startup and shutdown, and
    * finalize.
    *
    * The AbstractHandler also defines a priority (via getPriority)
    * used to specify the order for startup and shutdown.  Lower
    * numbers are started first, higher last (0 first, 254 last).
    * Order is inverted on shutdown (254 first, 0 last).
    *
    * The priority is required since handlers may have dependencies.
    *
    * Users are reserved priorities 127 to 254.  Unless there is a
    * known dependency on a SAMRAI shutdown handler, users should use
    * these priorities.
    *
    * Note: Currently it is allowed to register a handler in a phase
    * if the handler does not have a callback for that phase.  In
    * other words if during the startup callback of a class A it
    * causes another class B to register a handler it will work only
    * if the handler for B does not have a startup method (hasStartup
    * returns false).  This restriction is in place to prevent one
    * from registering a handler during startup that needs to be
    * started.  This should be avoided but for legacy reasons is being
    * done in SAMRAI.
    *
    * @param handler
    */
   static void
   registerHandler(
      AbstractHandler* handler);

   /*!
    * @brief Invoke the registered initialization handlers.
    *
    * This should only be called once per program execution.
    */
   static void
   initialize();

   /*!
    * @brief Invoke the registered startup handlers.
    *
    * This routine must be called at SAMRAI problem startup.  It may be called
    * more than once per run if running multiple SAMRAI problems within the
    * same execution of the main function.
    */
   static void
   startup();

   /**
    * @brief Invoke the registered shutdown handlers.
    *
    * This routine must be called at SAMRAI problem shutdown.  It may be called
    * more than once per run if running multiple SAMRAI problems within the
    * same execution of the main function.
    */
   static void
   shutdown();

   /*!
    * @brief Invoke the registered finalize handlers.
    *
    * This should only be called once per program execution.
    */
   static void
   finalize();

   /*!
    * @brief Priorities for standard SAMRAI classes
    */
   static const unsigned char priorityArenaManager = 10;
   static const unsigned char priorityReferenceCounter = 20;
   static const unsigned char priorityLogger = 25;
   static const unsigned char priorityListElements = 30;
   static const unsigned char priorityList = 30;
   static const unsigned char priorityInputManager = 40;
   static const unsigned char priorityRestartManager = 50;
   static const unsigned char priorityVariableDatabase = 60;
   static const unsigned char priorityStatistician = 70;
   static const unsigned char priorityBoundaryLookupTable = 80;
   static const unsigned char priorityHierarchyDataOpsManager = 90;
   static const unsigned char priorityTimerManger = 95;
   static const unsigned char priorityTimers = 98;
   static const unsigned char priorityVariables = 100;

private:
   /*!
    * @brief Sets up the StartupShutdownManager singleton.
    */
   static void
   setupSingleton(
      void);

   /*!
    * @brief Used to maintain a list of registered handlers.
    *
    * TODO:  Should implementation be pulled out of header?
    */
   class ListElement
   {

public:

      /*!
       * @brief Constructor.
       */
      ListElement():
         handler(0),
         next(0)
      {
      }

      /*!
       * @brief A registered handler being stored in a list.
       */
      StartupShutdownManager::AbstractHandler* handler;

      /*!
       * @brief Next element of the list.
       */ 
      ListElement* next;

   };

   /*
    * @brief Maximum value of handler priorities.
    */
   static const short s_number_of_priorities = 255;

   /*!
    * @brief Array of singly linked lists holding registered handlers.
    */
   static ListElement* s_manager_list[s_number_of_priorities];

   /*!
    * @brief Array of pointers to ListElement, used for constructing
    * and modifying s_manager_list.
    */
   static ListElement* s_manager_list_last[s_number_of_priorities];

   /*!
    * @brief Number of items at each priority value.
    */
   static int s_num_manager_items[s_number_of_priorities];

   /*!
    * @brief Flag telling if the singleton been initialized.
    */
   static bool s_singleton_initialized;

   /*!
    * @brief Flags telling if the manager is currently in one of the loops
    * invoking callbacks.
    */
   static bool s_in_initialize;
   static bool s_in_startup;
   static bool s_in_shutdown;
   static bool s_in_finalize;

   /*!
    * @brief Flags telling which methods have been invoked.  Generally used
    * only for error checking.
    */
   static bool s_initialized;
   static bool s_startuped;
   static bool s_shutdowned;
   static bool s_finalized;

};

}
}

#endif