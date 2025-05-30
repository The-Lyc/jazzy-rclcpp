^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package rclcpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

28.1.8 (2025-04-02)
-------------------

28.1.7 (2025-03-26)
-------------------
* fix(ClockConditionalVariable): Fixed potential crash on shutdown (`#2762 <https://github.com/ros2/rclcpp/issues/2762>`_)
* doc: Added warning to not instantiate Clock directly with RCL_ROS_TIME (`#2769 <https://github.com/ros2/rclcpp/issues/2769>`_)
  * Backports: `#2768 <https://github.com/ros2/rclcpp/issues/2768>`_
* Use rmw_event_type_is_supported in test_qos_event (`#2766 <https://github.com/ros2/rclcpp/issues/2766>`_)
  * Backports: `#2761 <https://github.com/ros2/rclcpp/issues/2761>`_
* fix: Fixed expiring of goals if events executor is used (`#2674 <https://github.com/ros2/rclcpp/issues/2674>`_)
* Executor strong reference fix (`#2754 <https://github.com/ros2/rclcpp/issues/2754>`_)
  * Backports: `#2745 <https://github.com/ros2/rclcpp/issues/2745>`_
* Double gc executor fix (`#2753 <https://github.com/ros2/rclcpp/issues/2753>`_)
* Fix typo in doc section for get_service_typesupport_handle (`#2752 <https://github.com/ros2/rclcpp/issues/2752>`_)
  * Backports: `#2751 <https://github.com/ros2/rclcpp/issues/2751>`_
* Test case and fix for for https://github.com/ros2/rclcpp/issues/2652 (`#2740 <https://github.com/ros2/rclcpp/issues/2740>`_)
  * Backports: `#2713 <https://github.com/ros2/rclcpp/issues/2713>`_
* fix(timer): Delete node, after executor thread terminated (`#2738 <https://github.com/ros2/rclcpp/issues/2738>`_)
  * Backports: `#2737 <https://github.com/ros2/rclcpp/issues/2737>`_
* fix(Executor): Fixed entities not beeing executed after just beeing added (`#2729 <https://github.com/ros2/rclcpp/issues/2729>`_)
  * Backports: `#2737 <https://github.com/ros2/rclcpp/issues/2724>`_
* Fix transient local IPC publish  (`#2722 <https://github.com/ros2/rclcpp/issues/2722>`_)
  * Backports: `#2708 <https://github.com/ros2/rclcpp/issues/2708>`_
* Contributors: Janosch Machowinski, Jeffery Hsu, Tomoya Fujita, Francisco Martín Rico

28.1.6 (2024-12-18)
-------------------
* apply actual QoS from rmw to the IPC publisher. (`#2707 <https://github.com/ros2/rclcpp/issues/2707>`_) (`#2712 <https://github.com/ros2/rclcpp/issues/2712>`_)
  * apply actual QoS from rmw to the IPC publisher.
  * address uncrustify warning.
  ---------
  (cherry picked from commit 016cfeac99e4b67f58abdf247e57f05b85c09ec4)
  Co-authored-by: Tomoya Fujita <Tomoya.Fujita@sony.com>
* Adding in topic name to logging on IPC issues (`#2706 <https://github.com/ros2/rclcpp/issues/2706>`_) (`#2710 <https://github.com/ros2/rclcpp/issues/2710>`_)
  * Adding in topic name to logging on IPC issues
  * Update test matching output logging
  * adding in single quotes
  ---------
  (cherry picked from commit a13e16e2cbaeacb14ff31272d01cbb21bd8ac037)
  Co-authored-by: Steve Macenski <stevenmacenski@gmail.com>
* enable testRaceConditionAddNode for rmw_connextdds. (`#2698 <https://github.com/ros2/rclcpp/issues/2698>`_)
* Re-enable executor test on rmw_connextdds. (`#2693 <https://github.com/ros2/rclcpp/issues/2693>`_) (`#2695 <https://github.com/ros2/rclcpp/issues/2695>`_)
  It supports the events executor now, so re-enable the test.
  (cherry picked from commit d7245365ed867db9b309ed3efbfb0391bda09bd5)
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
* Fix warnings on Windows. (backport `#2692 <https://github.com/ros2/rclcpp/issues/2692>`_) (`#2694 <https://github.com/ros2/rclcpp/issues/2694>`_)
  * Fix warnings on Windows. (`#2692 <https://github.com/ros2/rclcpp/issues/2692>`_)
  For reasons I admit I do not understand, the deprecation
  warnings for StaticSingleThreadedExecutor on Windows
  happen when we construct a shared_ptr for it in the tests.
  If we construct a regular object, then it is fine.  Luckily
  this test does not require a shared_ptr, so just make it
  a regular object here, which rixes the warning.
  While we are in here, make all of the tests camel case to
  be consistent.
  (cherry picked from commit 3310f9eaed967e0c18d17bb2f82d2def838bb7a5)
  # Conflicts:
  #	rclcpp/test/rclcpp/executors/test_executors.cpp
  * resolve backport conflict.
  ---------
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
  Co-authored-by: Tomoya Fujita <Tomoya.Fujita@sony.com>
* Omnibus fixes for running tests with Connext. (backport `#2684 <https://github.com/ros2/rclcpp/issues/2684>`_) (`#2690 <https://github.com/ros2/rclcpp/issues/2690>`_)
  * Omnibus fixes for running tests with Connext. (`#2684 <https://github.com/ros2/rclcpp/issues/2684>`_)
  * Omnibus fixes for running tests with Connext.
  When running the tests with RTI Connext as the default
  RMW, some of the tests are failing.  There are three
  different failures fixed here:
  1.  Setting the liveliness duration to a value smaller than
  a microsecond causes Connext to throw an error.  Set it to
  a millisecond.
  2.  Using the SystemDefaultsQoS sets the QoS to KEEP_LAST 1.
  Connext is somewhat slow in this regard, so it can be the case
  that we are overwriting a previous service introspection event
  with the next one.  Switch to the ServicesDefaultQoS in the test,
  which ensures we will not lose events.
  3.  Connext is slow to match publishers and subscriptions.  Thus,
  when creating a subscription "on-the-fly", we should wait for the
  publisher to match it before expecting the subscription to actually
  receive data from it.
  With these fixes in place, the test_client_common, test_generic_service,
  test_service_introspection, and test_executors tests all pass for
  me with rmw_connextdds.
  * Fixes for executors.
  * One more fix for services.
  * More fixes for service_introspection.
  * More fixes for introspection.
  ---------
  (cherry picked from commit 9984197c292d6c5ae0e7661aaea245ffb0fea057)
  # Conflicts:
  #	rclcpp/test/rclcpp/executors/test_executors.cpp
  #	rclcpp/test/rclcpp/test_generic_service.cpp
  * address backport merge conflicts.
  ---------
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
  Co-authored-by: Tomoya Fujita <Tomoya.Fujita@sony.com>
* fix(Executor): Fix segfault if callback group is deleted during rmw_wait (`#2682 <https://github.com/ros2/rclcpp/issues/2682>`_)
* Fix NodeOptions assignment operator (`#2656 <https://github.com/ros2/rclcpp/issues/2656>`_) (`#2660 <https://github.com/ros2/rclcpp/issues/2660>`_)
  * Fix NodeOptions assignment operator
  Also copy the enable_logger_service\_ member during the assignment operation
  * Add more checks for NodeOptions copy test
  * Set non default values by avoiding the copy-assignement
  Co-authored-by: Christophe Bedard <bedard.christophe@gmail.com>
  (cherry picked from commit 9b654942f99f17850e0e95480958abdbb508bc00)
  Co-authored-by: Romain DESILLE <r.desille@gmail.com>
* set QoS History KEEP_ALL explicitly for statistics publisher. (`#2650 <https://github.com/ros2/rclcpp/issues/2650>`_) (`#2657 <https://github.com/ros2/rclcpp/issues/2657>`_)
  * set QoS History KEEP_ALL explicitly for statistics publisher.
  * test_subscription_options adjustment.
  Co-authored-by: Tomoya Fujita <Tomoya.Fujita@sony.com>
* Contributors: Cristóbal Arroyo, Tomoya Fujita, jmachowinski, mergify[bot]

28.1.5 (2024-09-19)
-------------------
* backport fix events-executor warm-up bug and add unit-tests (`#2591 <https://github.com/ros2/rclcpp/issues/2591>`_) (`#2628 <https://github.com/ros2/rclcpp/issues/2628>`_)
* Contributors: Alberto Soragna

28.1.4 (2024-09-06)
-------------------
* Split test_executors.cpp even further. (`#2572 <https://github.com/ros2/rclcpp/issues/2572>`_) (`#2619 <https://github.com/ros2/rclcpp/issues/2619>`_)
  That's because it is too large for Windows Debug to compile,
  so split into smaller bits.
  Even with this split, the file is too big; that's likely
  because we are using TYPED_TEST here, which generates multiple
  symbols per test case.  To deal with this, without further
  breaking up the file, also add in the /bigobj flag when
  compiling on Windows Debug.
  (cherry picked from commit c743c173e68d92af872cf163e10721a8dbe51dd0)
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
* Correct node name in service test code (`#2615 <https://github.com/ros2/rclcpp/issues/2615>`_) (`#2616 <https://github.com/ros2/rclcpp/issues/2616>`_)
  (cherry picked from commit e846f56224a39b93f1c609e7ee03fff0662b7453)
  Co-authored-by: Barry Xu <barry.xu@sony.com>
* Release ownership of entities after spinning cancelled (backport `#2556 <https://github.com/ros2/rclcpp/issues/2556>`_) (`#2580 <https://github.com/ros2/rclcpp/issues/2580>`_)
  * Release ownership of entities after spinning cancelled (`#2556 <https://github.com/ros2/rclcpp/issues/2556>`_)
  * Release ownership of entities after spinning cancelled
  * Move release action to every exit point in different spin functions
  * Move wait_result\_.reset() before setting spinning to false
  * Update test code
  * Move test code to test_executors.cpp
  ---------
  (cherry picked from commit 069a0018935b33a14632a1cdf4074984a1cf80fe)
  # Conflicts:
  #	rclcpp/test/rclcpp/executors/test_executors.cpp
  * Fix backport issue (`#2581 <https://github.com/ros2/rclcpp/issues/2581>`_)
  ---------
  Co-authored-by: Barry Xu <barry.xu@sony.com>
* Contributors: mergify[bot]

28.1.3 (2024-06-27)
-------------------
* Add test creating two content filter topics with the same topic name (`#2546 <https://github.com/ros2/rclcpp/issues/2546>`_) (`#2549 <https://github.com/ros2/rclcpp/issues/2549>`_) (`#2552 <https://github.com/ros2/rclcpp/issues/2552>`_)
  Co-authored-by: Mario Domínguez López <116071334+Mario-DL@users.noreply.github.com>
  (cherry picked from commit 7c096888caf92aa7557e1d3efc5448b56d8ce81c)
  Co-authored-by: Alejandro Hernández Cordero <ahcorde@gmail.com>
* Contributors: mergify[bot]

28.1.2 (2024-05-13)
-------------------
* add impl pointer for ExecutorOptions (`#2523 <https://github.com/ros2/rclcpp/issues/2523>`_) (`#2525 <https://github.com/ros2/rclcpp/issues/2525>`_)
  * add impl pointer for ExecutorOptions
  (cherry picked from commit 343b29b617b163ad72b9fe3f6441dd4ed3d3af09)
  Co-authored-by: William Woodall <william@osrfoundation.org>
* Fixup Executor::spin_all() regression fix (`#2517 <https://github.com/ros2/rclcpp/issues/2517>`_) (`#2521 <https://github.com/ros2/rclcpp/issues/2521>`_)
  * test(Executors): Added tests for busy waiting
  Checks if executors are busy waiting while they should block
  in spin_some or spin_all.
  * fix: Reworked spinAll test
  This test was strange. It looked like, it assumed that spin_all did
  not return instantly. Also it was racy, as the thread could terminate
  instantly.
  * fix(Executor): Fixed spin_all not returning instantly is no work was available
  * Update rclcpp/test/rclcpp/executors/test_executors.cpp
  * test(executors): Added test for busy waiting while calling spin
  * fix(executor): Reset wait_result on every call to spin_some_impl
  Before, the method would not recollect available work in case of
  spin_some, spin_all. This would lead to the method behaving differently
  than to what the documentation states.
  * restore previous test logic for now
  * refactor spin_some_impl's logic and improve busy wait tests
  * added some more comments about the implementation
  ---------
  Co-authored-by: Janosch Machowinski <J.Machowinski@cellumation.com>
  Co-authored-by: jmachowinski <jmachowinski@users.noreply.github.com>
  Co-authored-by: Tomoya Fujita <Tomoya.Fujita@sony.com>
  Co-authored-by: William Woodall <william@osrfoundation.org>
* Contributors: mergify[bot]

28.1.1 (2024-04-24)
-------------------
* Revise the description of service configure_introspection() (`#2511 <https://github.com/ros2/rclcpp/issues/2511>`_) (`#2513 <https://github.com/ros2/rclcpp/issues/2513>`_)
* Contributors: mergify[bot]

28.1.0 (2024-04-16)
-------------------
* Remove references to index.ros.org. (`#2504 <https://github.com/ros2/rclcpp/issues/2504>`_)
* Reduce overhead for inheriting from rclcpp::Executor when base functionality is not reused (`#2506 <https://github.com/ros2/rclcpp/issues/2506>`_)
* Contributors: Chris Lalancette, William Woodall, jmachowinski

28.0.1 (2024-04-16)
-------------------
* [wjwwood] Updated "Data race fixes" (`#2500 <https://github.com/ros2/rclcpp/issues/2500>`_)
  * Fix callback group logic in executor
  * fix: Fixed unnecessary copy of wait_set
  * fix(executor): Fixed race conditions with rebuild of wait_sets
  Before this change, the rebuild of wait set would be triggered
  after the wait set was waken up. With bad timing, this could
  lead to the rebuild not happening with multi threaded executor.
  * fix(Executor): Fixed lost of entities rebuild request
  * chore: Added assert for not set callback_group in execute_any_executable
  * Add test for cbg getting reset
  Co-authored-by: Janosch Machowinski <j.machowinski@nospam.org>
  * chore: renamed test cases to snake_case
  * style
  * fixup test to avoid polling and short timeouts
  * fix: Use correct notify_waitable\_ instance
  * fix(StaticSingleThreadedExecutor): Added missing special case handling for current_notify_waitable\_
  * fix(TestCallbackGroup): Fixed test after change to timers
  ---------
  Co-authored-by: Janosch Machowinski <j.machowinski@cellumation.com>
  Co-authored-by: Michael Carroll <mjcarroll@intrinsic.ai>
  Co-authored-by: Janosch Machowinski <j.machowinski@nospam.org>
* fixup var names to snake case (`#2501 <https://github.com/ros2/rclcpp/issues/2501>`_)
* Added optional TimerInfo to timer callback (`#2343 <https://github.com/ros2/rclcpp/issues/2343>`_)
  Co-authored-by: Alexis Tsogias <a.tsogias@cellumation.com>
  Co-authored-by: Janosch Machowinski <J.Machowinski@cellumation.com>
* Fix uninitialized memory in test (`#2498 <https://github.com/ros2/rclcpp/issues/2498>`_)
  When I added in the tests for large messages, I made a mistake and reserved space in the strings, but didn't actually expand it.  Thus, we were writing into uninitialized memory.  Fix this by just using the correct constructor for string, which will allocate and initialize the memory properly.
* Ensure waitables handle guard condition retriggering (`#2483 <https://github.com/ros2/rclcpp/issues/2483>`_)
  Co-authored-by: Michael Carroll <mjcarroll@intrinsic.ai>
* fix: init concatenated_vector with begin() & end() (`#2492 <https://github.com/ros2/rclcpp/issues/2492>`_)
  * this commit will fix the warning [-Wstringop-overflow=] `#2461 <https://github.com/ros2/rclcpp/issues/2461>`_
* Use the same context for the specified node in rclcpp::spin functions (`#2433 <https://github.com/ros2/rclcpp/issues/2433>`_)
  * Use the same conext for the specified node in rclcpp::spin_xx functions
  * Add test for spinning with non-default-context
  * Format code
  ---------
* Disable compare-function-pointers in test_utilities (`#2489 <https://github.com/ros2/rclcpp/issues/2489>`_)
* address ambiguous auto variable. (`#2481 <https://github.com/ros2/rclcpp/issues/2481>`_)
* Increase the cppcheck timeout to 1200 seconds (`#2484 <https://github.com/ros2/rclcpp/issues/2484>`_)
* Removed test_timers_manager clang warning (`#2479 <https://github.com/ros2/rclcpp/issues/2479>`_)
* Flaky timer test fix (`#2469 <https://github.com/ros2/rclcpp/issues/2469>`_)
  * fix(time_source): Fixed possible race condition
  * fix(test_executors_time_cancel_behaviour): Fixed multiple race conditions
  ---------
  Co-authored-by: Janosch Machowinski <j.machowinski@nospam.org>
* Add tracepoint for generic publisher/subscriber (`#2448 <https://github.com/ros2/rclcpp/issues/2448>`_)
* update rclcpp::Waitable API to use references and const (`#2467 <https://github.com/ros2/rclcpp/issues/2467>`_)
* Utilize rclcpp::WaitSet as part of the executors (`#2142 <https://github.com/ros2/rclcpp/issues/2142>`_)
  * Deprecate callback_group call taking context
  * Add base executor objects that can be used by implementors
  * Template common operations
  * Address reviewer feedback:
  * Add callback to EntitiesCollector constructor
  * Make function to check automatically added callback groups take a list
  * Lint
  * Address reviewer feedback and fix templates
  * Lint and docs
  * Make executor own the notify waitable
  * Add pending queue to collector, remove from waitable
  Also change node's get_guard_condition to return shared_ptr
  * Change interrupt guard condition to shared_ptr
  Check if guard condition is valid before adding it to the waitable
  * Lint and docs
  * Utilize rclcpp::WaitSet as part of the executors
  * Don't exchange atomic twice
  * Fix add_node and add more tests
  * Make get_notify_guard_condition follow API tick-tock
  * Improve callback group tick-tocking
  * Don't lock twice
  * Address reviewer feedback
  * Add thread safety annotations and make locks consistent
  * @wip
  * Reset callback groups for multithreaded executor
  * Avoid many small function calls when building executables
  * Re-trigger guard condition if buffer has data
  * Address reviewer feedback
  * Trace points
  * Remove tracepoints
  * Reducing diff
  * Reduce diff
  * Uncrustify
  * Restore tests
  * Back to weak_ptr and reduce test time
  * reduce diff and lint
  * Restore static single threaded tests that weren't working before
  * Restore more tests
  * Fix multithreaded test
  * Fix assert
  * Fix constructor test
  * Change ready_executables signature back
  * Don't enforce removing callback groups before nodes
  * Remove the "add_valid_node" API
  * Only notify if the trigger condition is valid
  * Only trigger if valid and needed
  * Fix spin_some/spin_all implementation
  * Restore single threaded executor
  * Picking ABI-incompatible executor changes
  * Add PIMPL
  * Additional waitset prune
  * Fix bad merge
  * Expand test timeout
  * Introduce method to clear expired entities from a collection
  * Make sure to call remove_expired_entities().
  * Prune queued work when callback group is removed
  * Prune subscriptions from dynamic storage
  * Styles fixes.
  * Re-trigger guard conditions
  * Condense to just use watiable.take_data
  * Lint
  * Address reviewer comments (nits)
  * Lock mutex when copying
  * Refactors to static single threaded based on reviewers
  * More small refactoring
  * Lint
  * Lint
  * Add ready executable accessors to WaitResult
  * Make use of accessors from wait_set
  * Fix tests
  * Fix more tests
  * Tidy up single threaded executor implementation
  * Don't null out timer, rely on call
  * change how timers are checked from wait result in executors
  * peak -> peek
  * fix bug in next_waitable logic
  * fix bug in StaticSTE that broke the add callback groups to executor tests
  * style
  ---------
  Co-authored-by: Chris Lalancette <clalancette@gmail.com>
  Co-authored-by: William Woodall <william@osrfoundation.org>
* fix flakiness in TestTimersManager unit-test (`#2468 <https://github.com/ros2/rclcpp/issues/2468>`_)
  the previous version of the test was relying on the assumption that a timer with 1ms period gets called at least 6 times if the main thread waits 15ms. this is true most of the times, but it's not guaranteed, especially when running the test on windows CI servers. the new version of the test makes no assumptions on how much time it takes for the timers manager to invoke the timers, but rather focuses on ensuring that they are called the right amount of times, which is what's important for the purpose of the test
* Contributors: Alberto Soragna, Alejandro Hernández Cordero, Chris Lalancette, Homalozoa X, Kotaro Yoshimoto, Michael Carroll, Tomoya Fujita, William Woodall, h-suzuki-isp, jmachowinski

28.0.0 (2024-03-28)
-------------------
* fix spin_some_max_duration unit-test for events-executor (`#2465 <https://github.com/ros2/rclcpp/issues/2465>`_)
* refactor and improve the parameterized spin_some tests for executors (`#2460 <https://github.com/ros2/rclcpp/issues/2460>`_)
  * refactor and improve the spin_some parameterized tests for executors
  * disable spin_some_max_duration for the StaticSingleThreadedExecutor and EventsExecutor
  * fixup and clarify the docstring for Executor::spin_some()
  * style
  * review comments
  ---------
* enable simulation clock for timer canceling test. (`#2458 <https://github.com/ros2/rclcpp/issues/2458>`_)
  * enable simulation clock for timer canceling test.
  * move MainExecutorTypes to test_executors_timer_cancel_behavior.cpp.
  ---------
* Revert "relax the test simulation rate for timer canceling tests. (`#2453 <https://github.com/ros2/rclcpp/issues/2453>`_)" (`#2456 <https://github.com/ros2/rclcpp/issues/2456>`_)
  This reverts commit 1c350d0d7fb9c7158e0a39057112486ddbd38e9a.
* relax the test simulation rate for timer canceling tests. (`#2453 <https://github.com/ros2/rclcpp/issues/2453>`_)
* Fix TypeAdapted publishing with large messages. (`#2443 <https://github.com/ros2/rclcpp/issues/2443>`_)
  Mostly by ensuring we aren't attempting to store
  large messages on the stack.  Also add in tests.
  I verified that before these changes, the tests failed,
  while after them they succeed.
* Implement generic client (`#2358 <https://github.com/ros2/rclcpp/issues/2358>`_)
  * Implement generic client
  * Fix the incorrect parameter declaration
  * Deleted copy constructor and assignment for FutureAndRequestId
  * Update codes after rebase
  * Address review comments
  * Address review comments from iuhilnehc-ynos
  * Correct an error in a description
  * Fix window build errors
  * Address review comments from William
  * Add doc strings to create_generic_client
  ---------
* Rule of five: implement move operators (`#2425 <https://github.com/ros2/rclcpp/issues/2425>`_)
* Various cleanups to deal with uncrustify 0.78. (`#2439 <https://github.com/ros2/rclcpp/issues/2439>`_)
  These should also work with uncrustify 0.72.
* Remove the set_deprecated signatures in any_subscription_callback. (`#2431 <https://github.com/ros2/rclcpp/issues/2431>`_)
  These have been deprecated since April 2021, so it is safe
  to remove them now.
* fix doxygen syntax for NodeInterfaces (`#2428 <https://github.com/ros2/rclcpp/issues/2428>`_)
* Set hints to find the python version we actually want. (`#2426 <https://github.com/ros2/rclcpp/issues/2426>`_)
  The comment in the commit explains the reasoning behind it.
* Update quality declaration documents (`#2427 <https://github.com/ros2/rclcpp/issues/2427>`_)
* feat: add/minus for msg::Time and rclcpp::Duration (`#2419 <https://github.com/ros2/rclcpp/issues/2419>`_)
  * feat: add/minus for msg::Time and rclcpp::Duration
* Contributors: Alberto Soragna, Barry Xu, Chris Lalancette, Christophe Bedard, HuaTsai, Jonas Otto, Tim Clephas, Tomoya Fujita, William Woodall

27.0.0 (2024-02-07)
-------------------
* Split test_executors up into smaller chunks. (`#2421 <https://github.com/ros2/rclcpp/issues/2421>`_)
* [events executor] - Fix Behavior with Timer Cancel (`#2375 <https://github.com/ros2/rclcpp/issues/2375>`_)
* Removed deprecated header (`#2413 <https://github.com/ros2/rclcpp/issues/2413>`_)
* Make sure to mark RingBuffer methods as 'override'. (`#2410 <https://github.com/ros2/rclcpp/issues/2410>`_)
* Contributors: Alejandro Hernández Cordero, Chris Lalancette, Matt Condino

26.0.0 (2024-01-24)
-------------------
* Increase the cppcheck timeout to 600 seconds. (`#2409 <https://github.com/ros2/rclcpp/issues/2409>`_)
* Add transient local durability support to publisher and subscriptions when using intra-process communication (`#2303 <https://github.com/ros2/rclcpp/issues/2303>`_)
* Stop storing the context in the guard condition. (`#2400 <https://github.com/ros2/rclcpp/issues/2400>`_)
* Contributors: Chris Lalancette, Jeffery Hsu

25.0.0 (2023-12-26)
-------------------
* Updated GenericSubscription to AnySubscriptionCallback (`#1928 <https://github.com/ros2/rclcpp/issues/1928>`_)
* make type support helper supported for service (`#2209 <https://github.com/ros2/rclcpp/issues/2209>`_)
* Adding QoS to subscription options (`#2323 <https://github.com/ros2/rclcpp/issues/2323>`_)
* Switch to target_link_libraries. (`#2374 <https://github.com/ros2/rclcpp/issues/2374>`_)
* aligh with rcl that a rosout publisher of a node might not exist (`#2357 <https://github.com/ros2/rclcpp/issues/2357>`_)
* Fix data race in EventHandlerBase (`#2349 <https://github.com/ros2/rclcpp/issues/2349>`_)
* Support users holding onto shared pointers in the message memory pool (`#2336 <https://github.com/ros2/rclcpp/issues/2336>`_)
* Contributors: Chen Lihui, Chris Lalancette, DensoADAS, Lucas Wendland, mauropasse

24.0.0 (2023-11-06)
-------------------
* fix (signal_handler.hpp): spelling (`#2356 <https://github.com/ros2/rclcpp/issues/2356>`_)
* Updates to not use std::move in some places. (`#2353 <https://github.com/ros2/rclcpp/issues/2353>`_)
* rclcpp::Time::max() clock type support. (`#2352 <https://github.com/ros2/rclcpp/issues/2352>`_)
* Serialized Messages with Topic Statistics (`#2274 <https://github.com/ros2/rclcpp/issues/2274>`_)
* Add a custom deleter when constructing rcl_service_t (`#2351 <https://github.com/ros2/rclcpp/issues/2351>`_)
* Disable the loaned messages inside the executor. (`#2335 <https://github.com/ros2/rclcpp/issues/2335>`_)
* Use message_info in SubscriptionTopicStatistics instead of typed message (`#2337 <https://github.com/ros2/rclcpp/issues/2337>`_)
* Add missing 'enable_rosout' comments (`#2345 <https://github.com/ros2/rclcpp/issues/2345>`_)
* Adjust rclcpp usage of type description service (`#2344 <https://github.com/ros2/rclcpp/issues/2344>`_)
* address rate related flaky tests. (`#2329 <https://github.com/ros2/rclcpp/issues/2329>`_)
* Fixes pointed out by the clang analyzer. (`#2339 <https://github.com/ros2/rclcpp/issues/2339>`_)
* Remove useless ROSRate class (`#2326 <https://github.com/ros2/rclcpp/issues/2326>`_)
* Contributors: Alexey Merzlyakov, Chris Lalancette, Jiaqi Li, Lucas Wendland, Michael Carroll, Michael Orlov, Tomoya Fujita, Zard-C

23.2.0 (2023-10-09)
-------------------
* add clients & services count (`#2072 <https://github.com/ros2/rclcpp/issues/2072>`_)
* remove invalid sized allocation test for SerializedMessage. (`#2330 <https://github.com/ros2/rclcpp/issues/2330>`_)
* Adding API to copy all parameters from one node to another (`#2304 <https://github.com/ros2/rclcpp/issues/2304>`_)
* Contributors: Minju, Lee, Steve Macenski, Tomoya Fujita

23.1.0 (2023-10-04)
-------------------
* Add locking to protect the TimeSource::NodeState::node_base\_ (`#2320 <https://github.com/ros2/rclcpp/issues/2320>`_)
* Update SignalHandler get_global_signal_handler to avoid complex types in static memory (`#2316 <https://github.com/ros2/rclcpp/issues/2316>`_)
* Removing Old Connext Tests (`#2313 <https://github.com/ros2/rclcpp/issues/2313>`_)
* Documentation for list_parameters  (`#2315 <https://github.com/ros2/rclcpp/issues/2315>`_)
* Decouple rosout publisher init from node init. (`#2174 <https://github.com/ros2/rclcpp/issues/2174>`_)
* fix the depth to relative in list_parameters (`#2300 <https://github.com/ros2/rclcpp/issues/2300>`_)
* Contributors: Chris Lalancette, Lucas Wendland, Minju, Lee, Tomoya Fujita, Tully Foote

23.0.0 (2023-09-08)
-------------------
* Fix the return type of Rate::period. (`#2301 <https://github.com/ros2/rclcpp/issues/2301>`_)
* Update API docs links in package READMEs (`#2302 <https://github.com/ros2/rclcpp/issues/2302>`_)
* Cleanup flaky timers_manager tests. (`#2299 <https://github.com/ros2/rclcpp/issues/2299>`_)
* Contributors: Chris Lalancette, Christophe Bedard

22.2.0 (2023-09-07)
-------------------
* Topic correct typeadapter deduction (`#2294 <https://github.com/ros2/rclcpp/issues/2294>`_)
* Fix C++20 allocator construct deprecation (`#2292 <https://github.com/ros2/rclcpp/issues/2292>`_)
* Make Rate to select the clock to work with (`#2123 <https://github.com/ros2/rclcpp/issues/2123>`_)
* Correct the position of a comment. (`#2290 <https://github.com/ros2/rclcpp/issues/2290>`_)
* Remove unnecessary lambda captures in the tests. (`#2289 <https://github.com/ros2/rclcpp/issues/2289>`_)
* Add rcl_logging_interface as an explicit dependency. (`#2284 <https://github.com/ros2/rclcpp/issues/2284>`_)
* Revamp list_parameters to be more efficient and easier to read. (`#2282 <https://github.com/ros2/rclcpp/issues/2282>`_)
* Contributors: AiVerisimilitude, Alexey Merzlyakov, Chen Lihui, Chris Lalancette, Jiaqi Li

22.1.0 (2023-08-21)
-------------------
* Do not crash Executor when send_response fails due to client failure. (`#2276 <https://github.com/ros2/rclcpp/issues/2276>`_)
* Adding Custom Unknown Type Error (`#2272 <https://github.com/ros2/rclcpp/issues/2272>`_)
* Add a pimpl inside rclcpp::Node for future distro backports (`#2228 <https://github.com/ros2/rclcpp/issues/2228>`_)
* Remove an unused variable from the events executor tests. (`#2270 <https://github.com/ros2/rclcpp/issues/2270>`_)
* Add spin_all shortcut (`#2246 <https://github.com/ros2/rclcpp/issues/2246>`_)
* Adding Missing Group Exceptions (`#2256 <https://github.com/ros2/rclcpp/issues/2256>`_)
* Change associated clocks storage to unordered_set (`#2257 <https://github.com/ros2/rclcpp/issues/2257>`_)
* associated clocks should be protected by mutex. (`#2255 <https://github.com/ros2/rclcpp/issues/2255>`_)
* Instrument loaned message publication code path (`#2240 <https://github.com/ros2/rclcpp/issues/2240>`_)
* Contributors: Chris Lalancette, Christophe Bedard, Emerson Knapp, Luca Della Vedova, Lucas Wendland, Tomoya Fujita, Tony Najjar

22.0.0 (2023-07-11)
-------------------
* Implement get_node_type_descriptions_interface for lifecyclenode and add smoke test for it (`#2237 <https://github.com/ros2/rclcpp/issues/2237>`_)
* Add new node interface TypeDescriptionsInterface to provide GetTypeDescription service (`#2224 <https://github.com/ros2/rclcpp/issues/2224>`_)
* Move always_false_v to detail namespace (`#2232 <https://github.com/ros2/rclcpp/issues/2232>`_)
* Revamp the test_subscription.cpp tests. (`#2227 <https://github.com/ros2/rclcpp/issues/2227>`_)
* warning: comparison of integer expressions of different signedness (`#2219 <https://github.com/ros2/rclcpp/issues/2219>`_)
* Modifies timers API to select autostart state (`#2005 <https://github.com/ros2/rclcpp/issues/2005>`_)
* Enable callback group tests for connextdds (`#2182 <https://github.com/ros2/rclcpp/issues/2182>`_)
* Contributors: Chris Lalancette, Christopher Wecht, Eloy Briceno, Emerson Knapp, Nathan Wiebe Neufeldt, Tomoya Fujita

21.3.0 (2023-06-12)
-------------------
* Fix up misspellings of "receive". (`#2208 <https://github.com/ros2/rclcpp/issues/2208>`_)
* Remove flaky stressAddRemoveNode test (`#2206 <https://github.com/ros2/rclcpp/issues/2206>`_)
* Use TRACETOOLS\_ prefix for tracepoint-related macros (`#2162 <https://github.com/ros2/rclcpp/issues/2162>`_)
* Contributors: Chris Lalancette, Christophe Bedard, Michael Carroll

21.2.0 (2023-06-07)
-------------------
* remove nolint since ament_cpplint updated for the c++17 header (`#2198 <https://github.com/ros2/rclcpp/issues/2198>`_)
* Feature/available capacity of ipm (`#2173 <https://github.com/ros2/rclcpp/issues/2173>`_)
* add mutex to protect events_executor current entity collection (`#2187 <https://github.com/ros2/rclcpp/issues/2187>`_)
* Declare rclcpp callbacks before the rcl entities (`#2024 <https://github.com/ros2/rclcpp/issues/2024>`_)
* Contributors: Alberto Soragna, Chen Lihui, DensoADAS, mauropasse

21.1.1 (2023-05-11)
-------------------
* Fix race condition in events-executor (`#2177 <https://github.com/ros2/rclcpp/issues/2177>`_)
* Add missing stdexcept include (`#2186 <https://github.com/ros2/rclcpp/issues/2186>`_)
* Fix a format-security warning when building with clang (`#2171 <https://github.com/ros2/rclcpp/issues/2171>`_)
* Fix delivered message kind (`#2175 <https://github.com/ros2/rclcpp/issues/2175>`_)
* Contributors: Alberto Soragna, Chris Lalancette, methylDragon, Øystein Sture

21.1.0 (2023-04-27)
-------------------

21.0.0 (2023-04-18)
-------------------
* Add support for logging service. (`#2122 <https://github.com/ros2/rclcpp/issues/2122>`_)
* Picking ABI-incompatible executor changes (`#2170 <https://github.com/ros2/rclcpp/issues/2170>`_)
* add events-executor and timers-manager in rclcpp (`#2155 <https://github.com/ros2/rclcpp/issues/2155>`_)
* Create common structures for executors to use (`#2143 <https://github.com/ros2/rclcpp/issues/2143>`_)
* Implement deliver message kind (`#2168 <https://github.com/ros2/rclcpp/issues/2168>`_)
* Contributors: Alberto Soragna, Lei Liu, Michael Carroll, methylDragon

20.0.0 (2023-04-13)
-------------------
* applied tracepoints for ring_buffer (`#2091 <https://github.com/ros2/rclcpp/issues/2091>`_)
* Dynamic Subscription (REP-2011 Subset): Stubs for rclcpp (`#2165 <https://github.com/ros2/rclcpp/issues/2165>`_)
* Add type_hash to cpp TopicEndpointInfo (`#2137 <https://github.com/ros2/rclcpp/issues/2137>`_)
* Trigger the intraprocess guard condition with data (`#2164 <https://github.com/ros2/rclcpp/issues/2164>`_)
* Minor grammar fix (`#2149 <https://github.com/ros2/rclcpp/issues/2149>`_)
* Fix unnecessary allocations in executor.cpp (`#2135 <https://github.com/ros2/rclcpp/issues/2135>`_)
* add Logger::get_effective_level(). (`#2141 <https://github.com/ros2/rclcpp/issues/2141>`_)
* Remove deprecated header (`#2139 <https://github.com/ros2/rclcpp/issues/2139>`_)
* Implement matched event (`#2105 <https://github.com/ros2/rclcpp/issues/2105>`_)
* use allocator via init_options argument. (`#2129 <https://github.com/ros2/rclcpp/issues/2129>`_)
* Fixes to silence some clang warnings. (`#2127 <https://github.com/ros2/rclcpp/issues/2127>`_)
* Documentation improvements on the executor (`#2125 <https://github.com/ros2/rclcpp/issues/2125>`_)
* Avoid losing waitable handles while using MultiThreadedExecutor (`#2109 <https://github.com/ros2/rclcpp/issues/2109>`_)
* Hook up the incompatible type event inside of rclcpp (`#2069 <https://github.com/ros2/rclcpp/issues/2069>`_)
* Update all rclcpp packages to C++17. (`#2121 <https://github.com/ros2/rclcpp/issues/2121>`_)
* Fix clang warning: bugprone-use-after-move (`#2116 <https://github.com/ros2/rclcpp/issues/2116>`_)
* Contributors: Barry Xu, Chris Lalancette, Christopher Wecht, Emerson Knapp, Michael Carroll, Tomoya Fujita, Yadu, mauropasse, methylDragon, ymski

19.3.0 (2023-03-01)
-------------------
* Fix memory leak in tracetools::get_symbol() (`#2104 <https://github.com/ros2/rclcpp/issues/2104>`_)
* Service introspection (`#1985 <https://github.com/ros2/rclcpp/issues/1985>`_)
* Allow publishing borrowed messages with intra-process enabled (`#2108 <https://github.com/ros2/rclcpp/issues/2108>`_)
* to fix flaky test about TestTimeSource.callbacks (`#2111 <https://github.com/ros2/rclcpp/issues/2111>`_)
* Contributors: Brian, Chen Lihui, Christophe Bedard, Miguel Company

19.2.0 (2023-02-24)
-------------------
* to create a sublogger while getting child of Logger (`#1717 <https://github.com/ros2/rclcpp/issues/1717>`_)
* Fix documentation of Context class (`#2107 <https://github.com/ros2/rclcpp/issues/2107>`_)
* fixes for rmw callbacks in qos_event class (`#2102 <https://github.com/ros2/rclcpp/issues/2102>`_)
* Contributors: Alberto Soragna, Chen Lihui, Silvio Traversaro

19.1.0 (2023-02-14)
-------------------
* Add support for timers on reset callback (`#1979 <https://github.com/ros2/rclcpp/issues/1979>`_)
* Topic node guard condition in executor (`#2074 <https://github.com/ros2/rclcpp/issues/2074>`_)
* Fix bug on the disorder of calling shutdown callback (`#2097 <https://github.com/ros2/rclcpp/issues/2097>`_)
* Contributors: Barry Xu, Chen Lihui, mauropasse

19.0.0 (2023-01-30)
-------------------
* Add default constructor to NodeInterfaces (`#2094 <https://github.com/ros2/rclcpp/issues/2094>`_)
* Fix clock state cached time to be a copy, not a reference. (`#2092 <https://github.com/ros2/rclcpp/issues/2092>`_)
* Fix -Wmaybe-uninitialized warning (`#2081 <https://github.com/ros2/rclcpp/issues/2081>`_)
* Fix the keep_last warning when using system defaults. (`#2082 <https://github.com/ros2/rclcpp/issues/2082>`_)
* Add in a fix for older compilers. (`#2075 <https://github.com/ros2/rclcpp/issues/2075>`_)
* Contributors: Alexander Hans, Chris Lalancette, Shane Loretz

18.0.0 (2022-12-29)
-------------------
* Implement Unified Node Interface (NodeInterfaces class) (`#2041 <https://github.com/ros2/rclcpp/issues/2041>`_)
* Do not throw exception if trying to dequeue an empty intra-process buffer (`#2061 <https://github.com/ros2/rclcpp/issues/2061>`_)
* Move event callback binding to PublisherBase and SubscriptionBase (`#2066 <https://github.com/ros2/rclcpp/issues/2066>`_)
* Implement validity checks for rclcpp::Clock (`#2040 <https://github.com/ros2/rclcpp/issues/2040>`_)
* Explicitly set callback type (`#2059 <https://github.com/ros2/rclcpp/issues/2059>`_)
* Fix logging macros to build with msvc and cpp20 (`#2063 <https://github.com/ros2/rclcpp/issues/2063>`_)
* Add clock type to node_options (`#1982 <https://github.com/ros2/rclcpp/issues/1982>`_)
* Fix nullptr dereference in prune_requests_older_than (`#2008 <https://github.com/ros2/rclcpp/issues/2008>`_)
* Remove templating on to_rcl_subscription_options (`#2056 <https://github.com/ros2/rclcpp/issues/2056>`_)
* Fix SharedFuture from async_send_request never becoming valid (`#2044 <https://github.com/ros2/rclcpp/issues/2044>`_)
* Add in a warning for a KeepLast depth of 0. (`#2048 <https://github.com/ros2/rclcpp/issues/2048>`_)
* Mark rclcpp::Clock::now() as const (`#2050 <https://github.com/ros2/rclcpp/issues/2050>`_)
* Fix a case that did not throw ParameterUninitializedException (`#2036 <https://github.com/ros2/rclcpp/issues/2036>`_)
* Update maintainers (`#2043 <https://github.com/ros2/rclcpp/issues/2043>`_)
* Contributors: Alberto Soragna, Audrow Nash, Chen Lihui, Chris Lalancette, Jeffery Hsu, Lei Liu, Mateusz Szczygielski, Shane Loretz, andrei, mauropasse, methylDragon

17.1.0 (2022-11-02)
-------------------
* MultiThreadExecutor number of threads is at least 2+ in default. (`#2032 <https://github.com/ros2/rclcpp/issues/2032>`_)
* Fix bug that a callback not reached (`#1640 <https://github.com/ros2/rclcpp/issues/1640>`_)
* Set the minimum number of threads of the Multithreaded executor to 2 (`#2030 <https://github.com/ros2/rclcpp/issues/2030>`_)
* check thread whether joinable before join (`#2019 <https://github.com/ros2/rclcpp/issues/2019>`_)
* Set cpplint test timeout to 3 minutes (`#2022 <https://github.com/ros2/rclcpp/issues/2022>`_)
* Make sure to include-what-you-use in the node_interfaces. (`#2018 <https://github.com/ros2/rclcpp/issues/2018>`_)
* Do not clear entities callbacks on destruction (`#2002 <https://github.com/ros2/rclcpp/issues/2002>`_)
* fix mismatched issue if using zero_allocate (`#1995 <https://github.com/ros2/rclcpp/issues/1995>`_)
* Contributors: Alexis Paques, Chen Lihui, Chris Lalancette, Cristóbal Arroyo, Tomoya Fujita, mauropasse, uupks

17.0.0 (2022-09-13)
-------------------
* Make ParameterService and Sync/AsyncParameterClient accept rclcpp::QoS (`#1978 <https://github.com/ros2/rclcpp/issues/1978>`_)
* support regex match for parameter client (`#1992 <https://github.com/ros2/rclcpp/issues/1992>`_)
* operator+= and operator-= for Duration (`#1988 <https://github.com/ros2/rclcpp/issues/1988>`_)
* Revert "Revert "Add a create_timer method to Node and `LifecycleNode` classes (`#1975 <https://github.com/ros2/rclcpp/issues/1975>`_)" (`#2009 <https://github.com/ros2/rclcpp/issues/2009>`_) (`#2010 <https://github.com/ros2/rclcpp/issues/2010>`_)
* force compiler warning if callback handles not captured (`#2000 <https://github.com/ros2/rclcpp/issues/2000>`_)
* Revert "Add a `create_timer` method to `Node` and `LifecycleNode` classes (`#1975 <https://github.com/ros2/rclcpp/issues/1975>`_)" (`#2009 <https://github.com/ros2/rclcpp/issues/2009>`_)
* Add a `create_timer` method to `Node` and `LifecycleNode` classes (`#1975 <https://github.com/ros2/rclcpp/issues/1975>`_)
* [docs] add note about callback lifetime for {on, post}_set_parameter_callback (`#1981 <https://github.com/ros2/rclcpp/issues/1981>`_)
* fix memory leak (`#1994 <https://github.com/ros2/rclcpp/issues/1994>`_)
* Support pre-set and post-set parameter callbacks in addition to on-set-parameter-callback. (`#1947 <https://github.com/ros2/rclcpp/issues/1947>`_)
* Make create_service accept rclcpp::QoS (`#1969 <https://github.com/ros2/rclcpp/issues/1969>`_)
* Make create_client accept rclcpp::QoS (`#1964 <https://github.com/ros2/rclcpp/issues/1964>`_)
* Fix the documentation for rclcpp::ok to be accurate. (`#1965 <https://github.com/ros2/rclcpp/issues/1965>`_)
* use regex for wildcard matching (`#1839 <https://github.com/ros2/rclcpp/issues/1839>`_)
* Revert "Introduce executors new spin_for method, replace spin_until_future_complete with spin_until_complete. (`#1821 <https://github.com/ros2/rclcpp/issues/1821>`_) (`#1874 <https://github.com/ros2/rclcpp/issues/1874>`_)" (`#1956 <https://github.com/ros2/rclcpp/issues/1956>`_)
* Introduce executors new spin_for method, replace spin_until_future_complete with spin_until_complete. (`#1821 <https://github.com/ros2/rclcpp/issues/1821>`_) (`#1874 <https://github.com/ros2/rclcpp/issues/1874>`_)
* test adjustment for LoanedMessage. (`#1951 <https://github.com/ros2/rclcpp/issues/1951>`_)
* fix virtual dispatch issues identified by clang-tidy (`#1816 <https://github.com/ros2/rclcpp/issues/1816>`_)
* Remove unused on_parameters_set_callback\_ (`#1945 <https://github.com/ros2/rclcpp/issues/1945>`_)
* Fix subscription.is_serialized() for callbacks with message info (`#1950 <https://github.com/ros2/rclcpp/issues/1950>`_)
* wait for subscriptions on another thread. (`#1940 <https://github.com/ros2/rclcpp/issues/1940>`_)
* Fix documentation of `RCLCPP\_[INFO,WARN,...]` (`#1943 <https://github.com/ros2/rclcpp/issues/1943>`_)
* Always trigger guard condition waitset (`#1923 <https://github.com/ros2/rclcpp/issues/1923>`_)
* Add statistics for handle_loaned_message (`#1927 <https://github.com/ros2/rclcpp/issues/1927>`_)
* Drop wrong template specialization (`#1926 <https://github.com/ros2/rclcpp/issues/1926>`_)
* Contributors: Alberto Soragna, Andrew Symington, Barry Xu, Brian, Chen Lihui, Chris Lalancette, Daniel Reuter, Deepanshu Bansal, Hubert Liberacki, Ivan Santiago Paunovic, Jochen Sprickerhof, Nikolai Morin, Shane Loretz, Tomoya Fujita, Tyler Weaver, William Woodall, schrodinbug

16.2.0 (2022-05-03)
-------------------
* Update get_parameter_from_event to follow the function description (`#1922 <https://github.com/ros2/rclcpp/issues/1922>`_)
* Add 'best available' QoS enum values and methods (`#1920 <https://github.com/ros2/rclcpp/issues/1920>`_)
* Contributors: Barry Xu, Jacob Perron

16.1.0 (2022-04-29)
-------------------
* use reinterpret_cast for function pointer conversion. (`#1919 <https://github.com/ros2/rclcpp/issues/1919>`_)
* Contributors: Tomoya Fujita

16.0.1 (2022-04-13)
-------------------
* remove DEFINE_CONTENT_FILTER cmake option (`#1914 <https://github.com/ros2/rclcpp/issues/1914>`_)
* Contributors: Chen Lihui

16.0.0 (2022-04-08)
-------------------
* remove things that were deprecated during galactic (`#1913 <https://github.com/ros2/rclcpp/issues/1913>`_)
* Contributors: William Woodall

15.4.0 (2022-04-05)
-------------------
* add take_data_by_entity_id API to waitable (`#1892 <https://github.com/ros2/rclcpp/issues/1892>`_)
* add content-filtered-topic interfaces (`#1561 <https://github.com/ros2/rclcpp/issues/1561>`_)
* Contributors: Alberto Soragna, Chen Lihui

15.3.0 (2022-03-30)
-------------------
* [NodeParameters] Set name in param info pre-check (`#1908 <https://github.com/ros2/rclcpp/issues/1908>`_)
* Add test-dep ament_cmake_google_benchmark (`#1904 <https://github.com/ros2/rclcpp/issues/1904>`_)
* Add publish by loaned message in GenericPublisher (`#1856 <https://github.com/ros2/rclcpp/issues/1856>`_)
* Contributors: Abrar Rahman Protyasha, Barry Xu, Gaël Écorchard

15.2.0 (2022-03-24)
-------------------
* Add missing ament dependency on rcl_interfaces (`#1903 <https://github.com/ros2/rclcpp/issues/1903>`_)
* Update data callback tests to account for all published samples (`#1900 <https://github.com/ros2/rclcpp/issues/1900>`_)
* Increase timeout for acknowledgments to account for slower Connext settings (`#1901 <https://github.com/ros2/rclcpp/issues/1901>`_)
* clang-tidy: explicit constructors (`#1782 <https://github.com/ros2/rclcpp/issues/1782>`_)
* Add client/service QoS getters (`#1784 <https://github.com/ros2/rclcpp/issues/1784>`_)
* Fix a bunch more rosdoc2 issues in rclcpp. (`#1897 <https://github.com/ros2/rclcpp/issues/1897>`_)
* time_until_trigger returns max time if timer is cancelled (`#1893 <https://github.com/ros2/rclcpp/issues/1893>`_)
* Micro-optimizations in rclcpp (`#1896 <https://github.com/ros2/rclcpp/issues/1896>`_)
* Contributors: Andrea Sorbini, Chris Lalancette, Mauro Passerino, Scott K Logan, William Woodall

15.1.0 (2022-03-01)
-------------------
* spin_all with a zero timeout. (`#1878 <https://github.com/ros2/rclcpp/issues/1878>`_)
* Add RMW listener APIs (`#1579 <https://github.com/ros2/rclcpp/issues/1579>`_)
* Remove fastrtps customization on tests (`#1887 <https://github.com/ros2/rclcpp/issues/1887>`_)
* Install headers to include/${PROJECT_NAME} (`#1888 <https://github.com/ros2/rclcpp/issues/1888>`_)
* Use ament_generate_version_header (`#1886 <https://github.com/ros2/rclcpp/issues/1886>`_)
* use universal reference to support rvalue. (`#1883 <https://github.com/ros2/rclcpp/issues/1883>`_)
* fix one subscription can wait_for_message twice (`#1870 <https://github.com/ros2/rclcpp/issues/1870>`_)
* Add return value version of get_parameter_or (`#1813 <https://github.com/ros2/rclcpp/issues/1813>`_)
* Cleanup time source object lifetimes (`#1867 <https://github.com/ros2/rclcpp/issues/1867>`_)
* add is_spinning() method to executor base class
* Contributors: Alberto Soragna, Chen Lihui, Chris Lalancette, Kenji Miyake, Miguel Company, Shane Loretz, Tomoya Fujita, iRobot ROS

15.0.0 (2022-01-14)
-------------------
* Cleanup the TypeAdapt tests (`#1858 <https://github.com/ros2/rclcpp/issues/1858>`_)
* Cleanup includes (`#1857 <https://github.com/ros2/rclcpp/issues/1857>`_)
* Fix include order and relative paths for cpplint (`#1859 <https://github.com/ros2/rclcpp/issues/1859>`_)
* Rename stringstream in macros to a more unique name (`#1862 <https://github.com/ros2/rclcpp/issues/1862>`_)
* Add non transform capabilities for intra-process (`#1849 <https://github.com/ros2/rclcpp/issues/1849>`_)
* Fix rclcpp documentation build (`#1779 <https://github.com/ros2/rclcpp/issues/1779>`_)
* Contributors: Chris Lalancette, Doug Smith, Gonzo, Jacob Perron, Michel Hidalgo

14.1.0 (2022-01-05)
-------------------
* Use UninitializedStaticallyTypedParameterException (`#1689 <https://github.com/ros2/rclcpp/issues/1689>`_)
* Add wait_for_all_acked support (`#1662 <https://github.com/ros2/rclcpp/issues/1662>`_)
* Add tests for function templates of declare_parameter (`#1747 <https://github.com/ros2/rclcpp/issues/1747>`_)
* Contributors: Barry Xu, Bi0T1N, M. Mostafa Farzan

14.0.0 (2021-12-17)
-------------------
* Fixes for uncrustify 0.72 (`#1844 <https://github.com/ros2/rclcpp/issues/1844>`_)
* use private member to keep the all reference underneath. (`#1845 <https://github.com/ros2/rclcpp/issues/1845>`_)
* Make node base sharable (`#1832 <https://github.com/ros2/rclcpp/issues/1832>`_)
* Add Clock::sleep_for() (`#1828 <https://github.com/ros2/rclcpp/issues/1828>`_)
* Synchronize rcl and std::chrono steady clocks in Clock::sleep_until (`#1830 <https://github.com/ros2/rclcpp/issues/1830>`_)
* Use rclcpp::guard_condition (`#1612 <https://github.com/ros2/rclcpp/issues/1612>`_)
* Call CMake function to generate version header (`#1805 <https://github.com/ros2/rclcpp/issues/1805>`_)
* Use parantheses around logging macro parameter (`#1820 <https://github.com/ros2/rclcpp/issues/1820>`_)
* Remove author by request (`#1818 <https://github.com/ros2/rclcpp/issues/1818>`_)
* Update maintainers (`#1817 <https://github.com/ros2/rclcpp/issues/1817>`_)
* min_forward & min_backward thresholds must not be disabled (`#1815 <https://github.com/ros2/rclcpp/issues/1815>`_)
* Re-add Clock::sleep_until (`#1814 <https://github.com/ros2/rclcpp/issues/1814>`_)
* Fix lifetime of context so it remains alive while its dependent node handles are still in use (`#1754 <https://github.com/ros2/rclcpp/issues/1754>`_)
* Add the interface for pre-shutdown callback (`#1714 <https://github.com/ros2/rclcpp/issues/1714>`_)
* Take message ownership from moved LoanedMessage (`#1808 <https://github.com/ros2/rclcpp/issues/1808>`_)
* Suppress clang dead-store warnings in the benchmarks. (`#1802 <https://github.com/ros2/rclcpp/issues/1802>`_)
* Wait for publisher and subscription to match (`#1777 <https://github.com/ros2/rclcpp/issues/1777>`_)
* Fix unused QoS profile for clock subscription and make ClockQoS the default (`#1801 <https://github.com/ros2/rclcpp/issues/1801>`_)
* Contributors: Abrar Rahman Protyasha, Barry Xu, Chen Lihui, Chris Lalancette, Grey, Jacob Perron, Nikolai Morin, Shane Loretz, Tomoya Fujita, mauropasse

13.1.0 (2021-10-18)
-------------------
* Fix dangerous std::bind capture in TimeSource implementation. (`#1768 <https://github.com/ros2/rclcpp/issues/1768>`_)
* Fix dangerous std::bind capture in ParameterEventHandler implementation. (`#1770 <https://github.com/ros2/rclcpp/issues/1770>`_)
* Handle sigterm, in the same way sigint is being handled. (`#1771 <https://github.com/ros2/rclcpp/issues/1771>`_)
* rclcpp::Node copy constructor: make copy of node_waitables\_ member. (`#1799 <https://github.com/ros2/rclcpp/issues/1799>`_)
* Extend NodeGraph to match what rcl provides. (`#1484 <https://github.com/ros2/rclcpp/issues/1484>`_)
* Context::sleep_for(): replace recursion with do-while to avoid potential stack-overflow. (`#1765 <https://github.com/ros2/rclcpp/issues/1765>`_)
* extend_sub_namespace(): Verify string::empty() before calling string::front(). (`#1764 <https://github.com/ros2/rclcpp/issues/1764>`_)
* Deprecate the `void shared_ptr<MessageT>` subscription callback signatures. (`#1713 <https://github.com/ros2/rclcpp/issues/1713>`_)
* Contributors: Abrar Rahman Protyasha, Chris Lalancette, Emerson Knapp, Geoffrey Biggs, Ivan Santiago Paunovic, Jorge Perez, Tomoya Fujita, William Woodall, Yong-Hao Zou, livanov93

13.0.0 (2021-08-23)
-------------------
* Remove can_be_nullptr assignment check for QNX case. (`#1752 <https://github.com/ros2/rclcpp/issues/1752>`_)
* Update client API to be able to remove pending requests. (`#1734 <https://github.com/ros2/rclcpp/issues/1734>`_)
* Fix: Allow to add a node while spinning in the StaticSingleThreadedExecutor. (`#1690 <https://github.com/ros2/rclcpp/issues/1690>`_)
* Add tracing instrumentation for executor and message taking. (`#1738 <https://github.com/ros2/rclcpp/issues/1738>`_)
* Fix: Reset timer trigger time before execute in StaticSingleThreadedExecutor. (`#1739 <https://github.com/ros2/rclcpp/issues/1739>`_)
* Use FindPython3 and make python3 dependency explicit. (`#1745 <https://github.com/ros2/rclcpp/issues/1745>`_)
* Use rosidl_get_typesupport_target(). (`#1729 <https://github.com/ros2/rclcpp/issues/1729>`_)
* Fix returning invalid namespace if sub_namespace is empty. (`#1658 <https://github.com/ros2/rclcpp/issues/1658>`_)
* Add free function to wait for a subscription message. (`#1705 <https://github.com/ros2/rclcpp/issues/1705>`_)
* Use rcpputils/scope_exit.hpp and remove rclcpp/scope_exit.hpp. (`#1727 <https://github.com/ros2/rclcpp/issues/1727>`_)
* Contributors: Ahmed Sobhy, Christophe Bedard, Ivan Santiago Paunovic, Karsten Knese, M. Hofstätter, Mauro Passerino, Shane Loretz, mauropasse

12.0.0 (2021-07-26)
-------------------
* Remove unsafe get_callback_groups API.
  Callers should change to using for_each_callback_group(), or
  store the callback groups they need internally.
* Add in callback_groups_for_each.
  The main reason to add this method in is to make accesses to the
  callback_groups\_ vector thread-safe.  By having a
  callback_groups_for_each that accepts a std::function, we can
  just have the callers give us the callback they are interested
  in, and we can take care of the locking.
  The rest of this fairly large PR is cleaning up all of the places
  that use get_callback_groups() to instead use
  callback_groups_for_each().
* Use a different mechanism to avoid timers being scheduled multiple times by the MultiThreadedExecutor (`#1692 <https://github.com/ros2/rclcpp/issues/1692>`_)
* Fix windows CI (`#1726 <https://github.com/ros2/rclcpp/issues/1726>`_)
  Fix bug in AnyServiceCallback introduced in `#1709 <https://github.com/ros2/rclcpp/issues/1709>`_.
* Contributors: Chris Lalancette, Ivan Santiago Paunovic

11.2.0 (2021-07-21)
-------------------
* Support to defer to send a response in services. (`#1709 <https://github.com/ros2/rclcpp/issues/1709>`_)
  Signed-off-by: Ivan Santiago Paunovic <ivanpauno@ekumenlabs.com>
* Fix documentation bug. (`#1719 <https://github.com/ros2/rclcpp/issues/1719>`_)
  Signed-off-by: William Woodall <william@osrfoundation.org>
* Contributors: Ivan Santiago Paunovic, William Woodall

11.1.0 (2021-07-13)
-------------------
* Removed left over ``is_initialized()`` implementation (`#1711 <https://github.com/ros2/rclcpp/issues/1711>`_)
  Leftover from https://github.com/ros2/rclcpp/pull/1622
* Fixed declare parameter methods for int and float vectors (`#1696 <https://github.com/ros2/rclcpp/issues/1696>`_)
* Cleaned up implementation of the intra-process manager (`#1695 <https://github.com/ros2/rclcpp/issues/1695>`_)
* Added the node name to an executor ``runtime_error`` (`#1686 <https://github.com/ros2/rclcpp/issues/1686>`_)
* Fixed a typo "Attack" -> "Attach" (`#1687 <https://github.com/ros2/rclcpp/issues/1687>`_)
* Removed use of std::allocator<>::rebind (`#1678 <https://github.com/ros2/rclcpp/issues/1678>`_)
  rebind is deprecated in c++17 and removed in c++20
* Contributors: Alberto Soragna, Chen Lihui, Chris Lalancette, Petter Nilsson, Steve Macenski, William Woodall

11.0.0 (2021-05-18)
-------------------
* Allow declare uninitialized parameters (`#1673 <https://github.com/ros2/rclcpp/issues/1673>`_)
* Fix syntax issue with gcc (`#1674 <https://github.com/ros2/rclcpp/issues/1674>`_)
* [service] Don't use a weak_ptr to avoid leaking (`#1668 <https://github.com/ros2/rclcpp/issues/1668>`_)
* Contributors: Ivan Santiago Paunovic, Jacob Perron, William Woodall

10.0.0 (2021-05-11)
-------------------
* Fix doc typo (`#1663 <https://github.com/ros2/rclcpp/issues/1663>`_)
* [rclcpp] Type Adaptation feature (`#1557 <https://github.com/ros2/rclcpp/issues/1557>`_)
* Do not attempt to use void allocators for memory allocation. (`#1657 <https://github.com/ros2/rclcpp/issues/1657>`_)
* Keep custom allocator in publisher and subscription options alive. (`#1647 <https://github.com/ros2/rclcpp/issues/1647>`_)
* Fix get_publishers_subscriptions_info_by_topic test in test_node.cpp (`#1648 <https://github.com/ros2/rclcpp/issues/1648>`_)
* Use OnShutdown callback handle instead of OnShutdown callback (`#1639 <https://github.com/ros2/rclcpp/issues/1639>`_)
* use dynamic_pointer_cast to detect allocator mismatch in intra process manager (`#1643 <https://github.com/ros2/rclcpp/issues/1643>`_)
* Increase cppcheck timeout to 500s (`#1634 <https://github.com/ros2/rclcpp/issues/1634>`_)
* Clarify node parameters docs (`#1631 <https://github.com/ros2/rclcpp/issues/1631>`_)
* Contributors: Audrow Nash, Barry Xu, Jacob Perron, Michel Hidalgo, Shane Loretz, William Woodall

9.0.2 (2021-04-14)
------------------
* Avoid returning loan when none was obtained. (`#1629 <https://github.com/ros2/rclcpp/issues/1629>`_)
* Use a different implementation of mutex two priorities (`#1628 <https://github.com/ros2/rclcpp/issues/1628>`_)
* Do not test the value of the history policy when testing the get_publishers/subscriptions_info_by_topic() methods (`#1626 <https://github.com/ros2/rclcpp/issues/1626>`_)
* Check first parameter type and range before calling the user validation callbacks (`#1627 <https://github.com/ros2/rclcpp/issues/1627>`_)
* Contributors: Ivan Santiago Paunovic, Miguel Company

9.0.1 (2021-04-12)
------------------
* Restore test exception for Connext (`#1625 <https://github.com/ros2/rclcpp/issues/1625>`_)
* Fix race condition in TimeSource clock thread setup (`#1623 <https://github.com/ros2/rclcpp/issues/1623>`_)
* Contributors: Andrea Sorbini, Michel Hidalgo

9.0.0 (2021-04-06)
------------------
* remove deprecated code which was deprecated in foxy and should be removed in galactic (`#1622 <https://github.com/ros2/rclcpp/issues/1622>`_)
* Change index.ros.org -> docs.ros.org. (`#1620 <https://github.com/ros2/rclcpp/issues/1620>`_)
* Unique network flows (`#1496 <https://github.com/ros2/rclcpp/issues/1496>`_)
* Add spin_some support to the StaticSingleThreadedExecutor (`#1338 <https://github.com/ros2/rclcpp/issues/1338>`_)
* Add publishing instrumentation (`#1600 <https://github.com/ros2/rclcpp/issues/1600>`_)
* Create load_parameters and delete_parameters methods (`#1596 <https://github.com/ros2/rclcpp/issues/1596>`_)
* refactor AnySubscriptionCallback and add/deprecate callback signatures (`#1598 <https://github.com/ros2/rclcpp/issues/1598>`_)
* Add generic publisher and generic subscription for serialized messages (`#1452 <https://github.com/ros2/rclcpp/issues/1452>`_)
* use context from `node_base\_` for clock executor. (`#1617 <https://github.com/ros2/rclcpp/issues/1617>`_)
* updating quality declaration links (re: `ros2/docs.ros2.org#52 <https://github.com/ros2/docs.ros2.org/issues/52>`_) (`#1615 <https://github.com/ros2/rclcpp/issues/1615>`_)
* Contributors: Ananya Muddukrishna, BriceRenaudeau, Chris Lalancette, Christophe Bedard, Nikolai Morin, Tomoya Fujita, William Woodall, mauropasse, shonigmann

8.2.0 (2021-03-31)
------------------
* Initialize integers in test_parameter_event_handler.cpp to avoid undefined behavior (`#1609 <https://github.com/ros2/rclcpp/issues/1609>`_)
* Namespace tracetools C++ functions (`#1608 <https://github.com/ros2/rclcpp/issues/1608>`_)
* Revert "Namespace tracetools C++ functions (`#1603 <https://github.com/ros2/rclcpp/issues/1603>`_)" (`#1607 <https://github.com/ros2/rclcpp/issues/1607>`_)
* Namespace tracetools C++ functions (`#1603 <https://github.com/ros2/rclcpp/issues/1603>`_)
* Clock subscription callback group spins in its own thread (`#1556 <https://github.com/ros2/rclcpp/issues/1556>`_)
* Contributors: Chris Lalancette, Christophe Bedard, Ivan Santiago Paunovic, anaelle-sw

8.1.0 (2021-03-25)
------------------
* Remove rmw_connext_cpp references. (`#1595 <https://github.com/ros2/rclcpp/issues/1595>`_)
* Add API for checking QoS profile compatibility (`#1554 <https://github.com/ros2/rclcpp/issues/1554>`_)
* Document misuse of parameters callback (`#1590 <https://github.com/ros2/rclcpp/issues/1590>`_)
* use const auto & to iterate over parameters (`#1593 <https://github.com/ros2/rclcpp/issues/1593>`_)
* Contributors: Chris Lalancette, Jacob Perron, Karsten Knese

8.0.0 (2021-03-23)
------------------
* Guard against integer overflow in duration conversion (`#1584 <https://github.com/ros2/rclcpp/issues/1584>`_)
* Contributors: Jacob Perron

7.0.1 (2021-03-22)
------------------
* get_parameters service should return empty if undeclared parameters are allowed (`#1514 <https://github.com/ros2/rclcpp/issues/1514>`_)
* Made 'Context::shutdown_reason' function a const function (`#1578 <https://github.com/ros2/rclcpp/issues/1578>`_)
* Contributors: Tomoya Fujita, suab321321

7.0.0 (2021-03-18)
------------------
* Document design decisions that were made for statically typed parameters (`#1568 <https://github.com/ros2/rclcpp/issues/1568>`_)
* Fix doc typo in CallbackGroup constructor (`#1582 <https://github.com/ros2/rclcpp/issues/1582>`_)
* Enable qos parameter overrides for the /parameter_events topic  (`#1532 <https://github.com/ros2/rclcpp/issues/1532>`_)
* Add support for rmw_connextdds (`#1574 <https://github.com/ros2/rclcpp/issues/1574>`_)
* Remove 'struct' from the rcl_time_jump_t. (`#1577 <https://github.com/ros2/rclcpp/issues/1577>`_)
* Add tests for declaring statically typed parameters when undeclared parameters are allowed (`#1575 <https://github.com/ros2/rclcpp/issues/1575>`_)
* Quiet clang memory leak warning on "DoNotOptimize". (`#1571 <https://github.com/ros2/rclcpp/issues/1571>`_)
* Add ParameterEventsSubscriber class (`#829 <https://github.com/ros2/rclcpp/issues/829>`_)
* When a parameter change is rejected, the parameters map shouldn't be updated. (`#1567 <https://github.com/ros2/rclcpp/pull/1567>`_)
* Fix when to throw the NoParameterOverrideProvided exception. (`#1567 <https://github.com/ros2/rclcpp/pull/1567>`_)
* Fix SEGV caused by order of destruction of Node sub-interfaces (`#1469 <https://github.com/ros2/rclcpp/issues/1469>`_)
* Fix benchmark test failure introduced in `#1522 <https://github.com/ros2/rclcpp/issues/1522>`_ (`#1564 <https://github.com/ros2/rclcpp/issues/1564>`_)
* Fix documented example in create_publisher (`#1558 <https://github.com/ros2/rclcpp/issues/1558>`_)
* Enforce static parameter types (`#1522 <https://github.com/ros2/rclcpp/issues/1522>`_)
* Allow timers to keep up the intended rate in MultiThreadedExecutor (`#1516 <https://github.com/ros2/rclcpp/issues/1516>`_)
* Fix UBSAN warnings in any_subscription_callback. (`#1551 <https://github.com/ros2/rclcpp/issues/1551>`_)
* Fix runtime error: reference binding to null pointer of type (`#1547 <https://github.com/ros2/rclcpp/issues/1547>`_)
* Contributors: Andrea Sorbini, Chris Lalancette, Colin MacKenzie, Ivan Santiago Paunovic, Jacob Perron, Steven! Ragnarök, bpwilcox, tomoya

6.3.1 (2021-02-08)
------------------
* Reference test resources directly from source tree (`#1543 <https://github.com/ros2/rclcpp/issues/1543>`_)
* clear statistics after window reset (`#1531 <https://github.com/ros2/rclcpp/issues/1531>`_) (`#1535 <https://github.com/ros2/rclcpp/issues/1535>`_)
* Fix a minor string error in the topic_statistics test. (`#1541 <https://github.com/ros2/rclcpp/issues/1541>`_)
* Avoid `Resource deadlock avoided` if use intra_process_comms (`#1530 <https://github.com/ros2/rclcpp/issues/1530>`_)
* Avoid an object copy in parameter_value.cpp. (`#1538 <https://github.com/ros2/rclcpp/issues/1538>`_)
* Assert that the publisher_list size is 1. (`#1537 <https://github.com/ros2/rclcpp/issues/1537>`_)
* Don't access objects after they have been std::move (`#1536 <https://github.com/ros2/rclcpp/issues/1536>`_)
* Update for checking correct variable (`#1534 <https://github.com/ros2/rclcpp/issues/1534>`_)
* Destroy msg extracted from LoanedMessage. (`#1305 <https://github.com/ros2/rclcpp/issues/1305>`_)
* Contributors: Chen Lihui, Chris Lalancette, Ivan Santiago Paunovic, Miaofei Mei, Scott K Logan, William Woodall, hsgwa

6.3.0 (2021-01-25)
------------------
* Add instrumentation for linking a timer to a node (`#1500 <https://github.com/ros2/rclcpp/issues/1500>`_)
* Fix error when using IPC with StaticSingleThreadExecutor (`#1520 <https://github.com/ros2/rclcpp/issues/1520>`_)
* Change to using unique_ptrs for DummyExecutor. (`#1517 <https://github.com/ros2/rclcpp/issues/1517>`_)
* Allow reconfiguring 'clock' topic qos (`#1512 <https://github.com/ros2/rclcpp/issues/1512>`_)
* Allow to add/remove nodes thread safely in rclcpp::Executor  (`#1505 <https://github.com/ros2/rclcpp/issues/1505>`_)
* Call rclcpp::shutdown in test_node for clean shutdown on Windows (`#1515 <https://github.com/ros2/rclcpp/issues/1515>`_)
* Reapply "Add get_logging_directory method to rclcpp::Logger (`#1509 <https://github.com/ros2/rclcpp/issues/1509>`_)" (`#1513 <https://github.com/ros2/rclcpp/issues/1513>`_)
* use describe_parameters of parameter client for test (`#1499 <https://github.com/ros2/rclcpp/issues/1499>`_)
* Revert "Add get_logging_directory method to rclcpp::Logger (`#1509 <https://github.com/ros2/rclcpp/issues/1509>`_)" (`#1511 <https://github.com/ros2/rclcpp/issues/1511>`_)
* Add get_logging_directory method to rclcpp::Logger (`#1509 <https://github.com/ros2/rclcpp/issues/1509>`_)
* Contributors: Chris Lalancette, Christophe Bedard, Ivan Santiago Paunovic, eboasson, mauropasse, tomoya

6.2.0 (2021-01-08)
------------------
* Better documentation for the QoS class (`#1508 <https://github.com/ros2/rclcpp/issues/1508>`_)
* Modify excluding callback duration from topic statistics (`#1492 <https://github.com/ros2/rclcpp/issues/1492>`_)
* Make the test of graph users more robust. (`#1504 <https://github.com/ros2/rclcpp/issues/1504>`_)
* Make sure to wait for graph change events in test_node_graph. (`#1503 <https://github.com/ros2/rclcpp/issues/1503>`_)
* add timeout to SyncParametersClient methods (`#1493 <https://github.com/ros2/rclcpp/issues/1493>`_)
* Fix wrong test expectations (`#1497 <https://github.com/ros2/rclcpp/issues/1497>`_)
* Update create_publisher/subscription documentation, clarifying when a parameters interface is required (`#1494 <https://github.com/ros2/rclcpp/issues/1494>`_)
* Fix string literal warnings (`#1442 <https://github.com/ros2/rclcpp/issues/1442>`_)
* support describe_parameters methods to parameter client. (`#1453 <https://github.com/ros2/rclcpp/issues/1453>`_)
* Contributors: Audrow Nash, Chris Lalancette, Ivan Santiago Paunovic, Nikolai Morin, hsgwa, tomoya

6.1.0 (2020-12-10)
------------------
* Add getters to rclcpp::qos and rclcpp::Policy enum classes (`#1467 <https://github.com/ros2/rclcpp/issues/1467>`_)
* Change nullptr checks to use ASSERT_TRUE. (`#1486 <https://github.com/ros2/rclcpp/issues/1486>`_)
* Adjust logic around finding and erasing guard_condition (`#1474 <https://github.com/ros2/rclcpp/issues/1474>`_)
* Update QDs to QL 1 (`#1477 <https://github.com/ros2/rclcpp/issues/1477>`_)
* Add performance tests for parameter transport (`#1463 <https://github.com/ros2/rclcpp/issues/1463>`_)
* Contributors: Chris Lalancette, Ivan Santiago Paunovic, Scott K Logan, Stephen Brawner

6.0.0 (2020-11-18)
------------------
* Move ownership of shutdown_guard_condition to executors/graph_listener (`#1404 <https://github.com/ros2/rclcpp/issues/1404>`_)
* Add options to automatically declare qos parameters when creating a publisher/subscription (`#1465 <https://github.com/ros2/rclcpp/issues/1465>`_)
* Add `take_data` to `Waitable` and `data` to `AnyExecutable` (`#1241 <https://github.com/ros2/rclcpp/issues/1241>`_)
* Add benchmarks for node parameters interface (`#1444 <https://github.com/ros2/rclcpp/issues/1444>`_)
* Remove allocation from executor::remove_node() (`#1448 <https://github.com/ros2/rclcpp/issues/1448>`_)
* Fix test crashes on CentOS 7 (`#1449 <https://github.com/ros2/rclcpp/issues/1449>`_)
* Bump rclcpp packages to Quality Level 2 (`#1445 <https://github.com/ros2/rclcpp/issues/1445>`_)
* Added executor benchmark tests (`#1413 <https://github.com/ros2/rclcpp/issues/1413>`_)
* Add fully-qualified namespace to WeakCallbackGroupsToNodesMap (`#1435 <https://github.com/ros2/rclcpp/issues/1435>`_)
* Contributors: Alejandro Hernández Cordero, Audrow Nash, Chris Lalancette, Ivan Santiago Paunovic, Louise Poubel, Scott K Logan, brawner

5.1.0 (2020-11-02)
------------------
* Deprecate Duration(rcl_duration_value_t) in favor of static Duration::from_nanoseconds(rcl_duration_value_t) (`#1432 <https://github.com/ros2/rclcpp/issues/1432>`_)
* Avoid parsing arguments twice in `rclcpp::init_and_remove_ros_arguments` (`#1415 <https://github.com/ros2/rclcpp/issues/1415>`_)
* Add service and client benchmarks (`#1425 <https://github.com/ros2/rclcpp/issues/1425>`_)
* Set CMakeLists to only use default rmw for benchmarks (`#1427 <https://github.com/ros2/rclcpp/issues/1427>`_)
* Update tracetools' QL in rclcpp's QD (`#1428 <https://github.com/ros2/rclcpp/issues/1428>`_)
* Add missing locking to the rclcpp_action::ServerBase. (`#1421 <https://github.com/ros2/rclcpp/issues/1421>`_)
* Initial benchmark tests for rclcpp::init/shutdown create/destroy node (`#1411 <https://github.com/ros2/rclcpp/issues/1411>`_)
* Refactor test CMakeLists in prep for benchmarks (`#1422 <https://github.com/ros2/rclcpp/issues/1422>`_)
* Add methods in topic and service interface to resolve a name (`#1410 <https://github.com/ros2/rclcpp/issues/1410>`_)
* Update deprecated gtest macros (`#1370 <https://github.com/ros2/rclcpp/issues/1370>`_)
* Clear members for StaticExecutorEntitiesCollector to avoid shared_ptr dependency (`#1303 <https://github.com/ros2/rclcpp/issues/1303>`_)
* Increase test timeouts of slow running tests with rmw_connext_cpp (`#1400 <https://github.com/ros2/rclcpp/issues/1400>`_)
* Avoid self dependency that not destoryed (`#1301 <https://github.com/ros2/rclcpp/issues/1301>`_)
* Update maintainers (`#1384 <https://github.com/ros2/rclcpp/issues/1384>`_)
* Add clock qos to node options (`#1375 <https://github.com/ros2/rclcpp/issues/1375>`_)
* Fix NodeOptions copy constructor (`#1376 <https://github.com/ros2/rclcpp/issues/1376>`_)
* Make sure to clean the external client/service handle. (`#1296 <https://github.com/ros2/rclcpp/issues/1296>`_)
* Increase coverage of WaitSetTemplate (`#1368 <https://github.com/ros2/rclcpp/issues/1368>`_)
* Increase coverage of guard_condition.cpp to 100% (`#1369 <https://github.com/ros2/rclcpp/issues/1369>`_)
* Add coverage statement (`#1367 <https://github.com/ros2/rclcpp/issues/1367>`_)
* Tests for LoanedMessage with mocked loaned message publisher (`#1366 <https://github.com/ros2/rclcpp/issues/1366>`_)
* Add unit tests for qos and qos_event files (`#1352 <https://github.com/ros2/rclcpp/issues/1352>`_)
* Finish coverage of publisher API (`#1365 <https://github.com/ros2/rclcpp/issues/1365>`_)
* Finish API coverage on executors. (`#1364 <https://github.com/ros2/rclcpp/issues/1364>`_)
* Add test for ParameterService (`#1355 <https://github.com/ros2/rclcpp/issues/1355>`_)
* Add time API coverage tests (`#1347 <https://github.com/ros2/rclcpp/issues/1347>`_)
* Add timer coverage tests (`#1363 <https://github.com/ros2/rclcpp/issues/1363>`_)
* Add in additional tests for parameter_client.cpp coverage.
* Minor fixes to the parameter_service.cpp file.
* reset rcl_context shared_ptr after calling rcl_init sucessfully (`#1357 <https://github.com/ros2/rclcpp/issues/1357>`_)
* Improved test publisher - zero qos history depth value exception (`#1360 <https://github.com/ros2/rclcpp/issues/1360>`_)
* Covered resolve_use_intra_process (`#1359 <https://github.com/ros2/rclcpp/issues/1359>`_)
* Improve test_subscription_options (`#1358 <https://github.com/ros2/rclcpp/issues/1358>`_)
* Add in more tests for init_options coverage. (`#1353 <https://github.com/ros2/rclcpp/issues/1353>`_)
* Test the remaining node public API (`#1342 <https://github.com/ros2/rclcpp/issues/1342>`_)
* Complete coverage of Parameter and ParameterValue API (`#1344 <https://github.com/ros2/rclcpp/issues/1344>`_)
* Add in more tests for the utilities. (`#1349 <https://github.com/ros2/rclcpp/issues/1349>`_)
* Add in two more tests for expand_topic_or_service_name. (`#1350 <https://github.com/ros2/rclcpp/issues/1350>`_)
* Add tests for node_options API (`#1343 <https://github.com/ros2/rclcpp/issues/1343>`_)
* Add in more coverage for expand_topic_or_service_name. (`#1346 <https://github.com/ros2/rclcpp/issues/1346>`_)
* Test exception in spin_until_future_complete. (`#1345 <https://github.com/ros2/rclcpp/issues/1345>`_)
* Add coverage tests graph_listener (`#1330 <https://github.com/ros2/rclcpp/issues/1330>`_)
* Add in unit tests for the Executor class.
* Allow mimick patching of methods with up to 9 arguments.
* Improve the error messages in the Executor class.
* Add coverage for client API (`#1329 <https://github.com/ros2/rclcpp/issues/1329>`_)
* Increase service coverage (`#1332 <https://github.com/ros2/rclcpp/issues/1332>`_)
* Make more of the static entity collector API private.
* Const-ify more of the static executor.
* Add more tests for the static single threaded executor.
* Many more tests for the static_executor_entities_collector.
* Get one more line of code coverage in memory_strategy.cpp
* Bugfix when adding callback group.
* Fix typos in comments.
* Remove deprecated executor::FutureReturnCode APIs. (`#1327 <https://github.com/ros2/rclcpp/issues/1327>`_)
* Increase coverage of publisher/subscription API (`#1325 <https://github.com/ros2/rclcpp/issues/1325>`_)
* Not finalize guard condition while destructing SubscriptionIntraProcess (`#1307 <https://github.com/ros2/rclcpp/issues/1307>`_)
* Expose qos setting for /rosout (`#1247 <https://github.com/ros2/rclcpp/issues/1247>`_)
* Add coverage for missing API (except executors) (`#1326 <https://github.com/ros2/rclcpp/issues/1326>`_)
* Include topic name in QoS mismatch warning messages (`#1286 <https://github.com/ros2/rclcpp/issues/1286>`_)
* Add coverage tests context functions (`#1321 <https://github.com/ros2/rclcpp/issues/1321>`_)
* Increase coverage of node_interfaces, including with mocking rcl errors (`#1322 <https://github.com/ros2/rclcpp/issues/1322>`_)
* Contributors: Ada-King, Alejandro Hernández Cordero, Audrow Nash, Barry Xu, Chen Lihui, Chris Lalancette, Christophe Bedard, Ivan Santiago Paunovic, Jorge Perez, Morgan Quigley, brawner

5.0.0 (2020-09-18)
------------------
* Make node_graph::count_graph_users() const (`#1320 <https://github.com/ros2/rclcpp/issues/1320>`_)
* Add coverage for wait_set_policies (`#1316 <https://github.com/ros2/rclcpp/issues/1316>`_)
* Only exchange intra_process waitable if nonnull (`#1317 <https://github.com/ros2/rclcpp/issues/1317>`_)
* Check waitable for nullptr during constructor (`#1315 <https://github.com/ros2/rclcpp/issues/1315>`_)
* Call vector.erase with end iterator overload (`#1314 <https://github.com/ros2/rclcpp/issues/1314>`_)
* Use best effort, keep last, history depth 1 QoS Profile for '/clock' subscriptions (`#1312 <https://github.com/ros2/rclcpp/issues/1312>`_)
* Add tests type_support module (`#1308 <https://github.com/ros2/rclcpp/issues/1308>`_)
* Replace std_msgs with test_msgs in executors test (`#1310 <https://github.com/ros2/rclcpp/issues/1310>`_)
* Add set_level for rclcpp::Logger (`#1284 <https://github.com/ros2/rclcpp/issues/1284>`_)
* Remove unused private function (rclcpp::Node and rclcpp_lifecycle::Node) (`#1294 <https://github.com/ros2/rclcpp/issues/1294>`_)
* Adding tests basic getters (`#1291 <https://github.com/ros2/rclcpp/issues/1291>`_)
* Adding callback groups in executor (`#1218 <https://github.com/ros2/rclcpp/issues/1218>`_)
* Refactor Subscription Topic Statistics Tests (`#1281 <https://github.com/ros2/rclcpp/issues/1281>`_)
* Add operator!= for duration (`#1236 <https://github.com/ros2/rclcpp/issues/1236>`_)
* Fix clock thread issue (`#1266 <https://github.com/ros2/rclcpp/issues/1266>`_) (`#1267 <https://github.com/ros2/rclcpp/issues/1267>`_)
* Fix topic stats test, wait for more messages, only check the ones with samples (`#1274 <https://github.com/ros2/rclcpp/issues/1274>`_)
* Add get_domain_id method to rclcpp::Context (`#1271 <https://github.com/ros2/rclcpp/issues/1271>`_)
* Fixes for unit tests that fail under cyclonedds (`#1270 <https://github.com/ros2/rclcpp/issues/1270>`_)
* initialize_logging\_ should be copied (`#1272 <https://github.com/ros2/rclcpp/issues/1272>`_)
* Use static_cast instead of C-style cast for instrumentation (`#1263 <https://github.com/ros2/rclcpp/issues/1263>`_)
* Make parameter clients use template constructors (`#1249 <https://github.com/ros2/rclcpp/issues/1249>`_)
* Ability to configure domain_id via InitOptions. (`#1165 <https://github.com/ros2/rclcpp/issues/1165>`_)
* Simplify and fix allocator memory strategy unit test for connext (`#1252 <https://github.com/ros2/rclcpp/issues/1252>`_)
* Use global namespace for parameter events subscription topic (`#1257 <https://github.com/ros2/rclcpp/issues/1257>`_)
* Increase timeouts for connext for long tests (`#1253 <https://github.com/ros2/rclcpp/issues/1253>`_)
* Adjust test_static_executor_entities_collector for rmw_connext_cpp (`#1251 <https://github.com/ros2/rclcpp/issues/1251>`_)
* Fix failing test with Connext since it doesn't wait for discovery (`#1246 <https://github.com/ros2/rclcpp/issues/1246>`_)
* Fix node graph test with Connext and CycloneDDS returning actual data (`#1245 <https://github.com/ros2/rclcpp/issues/1245>`_)
* Warn about unused result of add_on_set_parameters_callback (`#1238 <https://github.com/ros2/rclcpp/issues/1238>`_)
* Unittests for memory strategy files, except allocator_memory_strategy (`#1189 <https://github.com/ros2/rclcpp/issues/1189>`_)
* EXPECT_THROW_EQ and ASSERT_THROW_EQ macros for unittests (`#1232 <https://github.com/ros2/rclcpp/issues/1232>`_)
* Add unit test for static_executor_entities_collector (`#1221 <https://github.com/ros2/rclcpp/issues/1221>`_)
* Parameterize test executors for all executor types (`#1222 <https://github.com/ros2/rclcpp/issues/1222>`_)
* Unit tests for allocator_memory_strategy.cpp part 2 (`#1198 <https://github.com/ros2/rclcpp/issues/1198>`_)
* Unit tests for allocator_memory_strategy.hpp (`#1197 <https://github.com/ros2/rclcpp/issues/1197>`_)
* Derive and throw exception in spin_some spin_all for StaticSingleThreadedExecutor (`#1220 <https://github.com/ros2/rclcpp/issues/1220>`_)
* Make ring buffer thread-safe (`#1213 <https://github.com/ros2/rclcpp/issues/1213>`_)
* Add missing RCLCPP_PUBLIC to ~StaticExecutorEntitiesCollector (`#1227 <https://github.com/ros2/rclcpp/issues/1227>`_)
* Document graph functions don't apply remap rules (`#1225 <https://github.com/ros2/rclcpp/issues/1225>`_)
* Remove recreation of entities_collector (`#1217 <https://github.com/ros2/rclcpp/issues/1217>`_)
* Contributors: Audrow Nash, Chen Lihui, Christophe Bedard, Daisuke Sato, Devin Bonnie, Dirk Thomas, Ivan Santiago Paunovic, Jacob Perron, Jannik Abbenseth, Jorge Perez, Pedro Pena, Shane Loretz, Stephen Brawner, Tomoya Fujita

4.0.0 (2020-07-09)
------------------
* Fix rclcpp::NodeOptions::operator= (`#1211 <https://github.com/ros2/rclcpp/issues/1211>`_)
* Link against thread library where necessary (`#1210 <https://github.com/ros2/rclcpp/issues/1210>`_)
* Unit tests for node interfaces (`#1202 <https://github.com/ros2/rclcpp/issues/1202>`_)
* Remove usage of domain id in node options (`#1205 <https://github.com/ros2/rclcpp/issues/1205>`_)
* Remove deprecated set_on_parameters_set_callback function (`#1199 <https://github.com/ros2/rclcpp/issues/1199>`_)
* Fix conversion of negative durations to messages (`#1188 <https://github.com/ros2/rclcpp/issues/1188>`_)
* Fix implementation of NodeOptions::use_global_arguments() (`#1176 <https://github.com/ros2/rclcpp/issues/1176>`_)
* Bump to QD to level 3 and fixed links (`#1158 <https://github.com/ros2/rclcpp/issues/1158>`_)
* Fix pub/sub count API tests (`#1203 <https://github.com/ros2/rclcpp/issues/1203>`_)
* Update tracetools' QL to 2 in rclcpp's QD (`#1187 <https://github.com/ros2/rclcpp/issues/1187>`_)
* Fix exception message on rcl_clock_init (`#1182 <https://github.com/ros2/rclcpp/issues/1182>`_)
* Throw exception if rcl_timer_init fails (`#1179 <https://github.com/ros2/rclcpp/issues/1179>`_)
* Unit tests for some header-only functions/classes (`#1181 <https://github.com/ros2/rclcpp/issues/1181>`_)
* Callback should be perfectly-forwarded (`#1183 <https://github.com/ros2/rclcpp/issues/1183>`_)
* Add unit tests for logging functionality (`#1184 <https://github.com/ros2/rclcpp/issues/1184>`_)
* Add create_publisher include to create_subscription (`#1180 <https://github.com/ros2/rclcpp/issues/1180>`_)
* Contributors: Alejandro Hernández Cordero, Christophe Bedard, Claire Wang, Dirk Thomas, Ivan Santiago Paunovic, Johannes Meyer, Michel Hidalgo, Stephen Brawner, tomoya

3.0.0 (2020-06-18)
------------------
* Check period duration in create_wall_timer (`#1178 <https://github.com/ros2/rclcpp/issues/1178>`_)
* Fix get_node_time_source_interface() docstring (`#988 <https://github.com/ros2/rclcpp/issues/988>`_)
* Add message lost subscription event (`#1164 <https://github.com/ros2/rclcpp/issues/1164>`_)
* Add spin_all method to Executor (`#1156 <https://github.com/ros2/rclcpp/issues/1156>`_)
* Reorganize test directory and split CMakeLists.txt (`#1173 <https://github.com/ros2/rclcpp/issues/1173>`_)
* Check if context is valid when looping in spin_some (`#1167 <https://github.com/ros2/rclcpp/issues/1167>`_)
* Add check for invalid topic statistics publish period (`#1151 <https://github.com/ros2/rclcpp/issues/1151>`_)
* Fix spin_until_future_complete: check spinning value (`#1023 <https://github.com/ros2/rclcpp/issues/1023>`_)
* Fix doxygen warnings (`#1163 <https://github.com/ros2/rclcpp/issues/1163>`_)
* Fix reference to rclcpp in its Quality declaration (`#1161 <https://github.com/ros2/rclcpp/issues/1161>`_)
* Allow spin_until_future_complete to accept any future like object (`#1113 <https://github.com/ros2/rclcpp/issues/1113>`_)
* Contributors: Alejandro Hernández Cordero, Christophe Bedard, Devin Bonnie, Dirk Thomas, DongheeYe, Ivan Santiago Paunovic, Jacob Perron, Sarthak Mittal, brawner, tomoya

2.0.0 (2020-06-01)
------------------
* Added missing virtual destructors. (`#1149 <https://github.com/ros2/rclcpp/issues/1149>`_)
* Fixed a test which was using different types on the same topic. (`#1150 <https://github.com/ros2/rclcpp/issues/1150>`_)
* Made ``test_rate`` more reliable on Windows and improve error output when it fails (`#1146 <https://github.com/ros2/rclcpp/issues/1146>`_)
* Added Security Vulnerability Policy pointing to REP-2006. (`#1130 <https://github.com/ros2/rclcpp/issues/1130>`_)
* Added missing header in ``logging_mutex.cpp``. (`#1145 <https://github.com/ros2/rclcpp/issues/1145>`_)
* Changed the WaitSet API to pass a shared pointer by value instead than by const reference when possible. (`#1141 <https://github.com/ros2/rclcpp/issues/1141>`_)
* Changed ``SubscriptionBase::get_subscription_handle() const`` to return a shared pointer to const value. (`#1140 <https://github.com/ros2/rclcpp/issues/1140>`_)
* Extended the lifetime of ``rcl_publisher_t`` by holding onto the shared pointer in order to avoid a use after free situation. (`#1119 <https://github.com/ros2/rclcpp/issues/1119>`_)
* Improved some docblocks (`#1127 <https://github.com/ros2/rclcpp/issues/1127>`_)
* Fixed a lock-order-inversion (potential deadlock) (`#1135 <https://github.com/ros2/rclcpp/issues/1135>`_)
* Fixed a potential Construction/Destruction order problem between global contexts vector and Context of static lifetime (`#1132 <https://github.com/ros2/rclcpp/issues/1132>`_)
* Contributors: Alejandro Hernández Cordero, Chris Lalancette, Ivan Santiago Paunovic, Michel Hidalgo, tomoya

1.1.0 (2020-05-26)
------------------
* Deprecate set_on_parameters_set_callback (`#1123 <https://github.com/ros2/rclcpp/issues/1123>`_)
* Expose get_service_names_and_types_by_node from rcl in rclcpp (`#1131 <https://github.com/ros2/rclcpp/issues/1131>`_)
* Fix thread safety issues related to logging (`#1125 <https://github.com/ros2/rclcpp/issues/1125>`_)
* Make sure rmw_publisher_options is initialized in to_rcl_publisher_options (`#1099 <https://github.com/ros2/rclcpp/issues/1099>`_)
* Remove empty lines within method signatures (`#1128 <https://github.com/ros2/rclcpp/issues/1128>`_)
* Add API review March 2020 document (`#1031 <https://github.com/ros2/rclcpp/issues/1031>`_)
* Improve documentation (`#1106 <https://github.com/ros2/rclcpp/issues/1106>`_)
* Make test multi threaded executor more reliable (`#1105 <https://github.com/ros2/rclcpp/issues/1105>`_)
* Fixed rep links and added more details to dependencies in quality declaration (`#1116 <https://github.com/ros2/rclcpp/issues/1116>`_)
* Update quality declarations to reflect version 1.0 (`#1115 <https://github.com/ros2/rclcpp/issues/1115>`_)
* Contributors: Alejandro Hernández Cordero, ChenYing Kuo, Claire Wang, Dirk Thomas, Ivan Santiago Paunovic, William Woodall, Stephen Brawner

1.0.0 (2020-05-12)
------------------
* Remove MANUAL_BY_NODE liveliness API (`#1107 <https://github.com/ros2/rclcpp/issues/1107>`_)
* Use rosidl_default_generators dependency in test (`#1114 <https://github.com/ros2/rclcpp/issues/1114>`_)
* Make sure to include what you use (`#1112 <https://github.com/ros2/rclcpp/issues/1112>`_)
* Mark flaky test with xfail: TestMultiThreadedExecutor (`#1109 <https://github.com/ros2/rclcpp/issues/1109>`_)
* Contributors: Chris Lalancette, Ivan Santiago Paunovic, Karsten Knese, Louise Poubel

0.9.1 (2020-05-08)
------------------
* Fix tests that were not properly torn down (`#1073 <https://github.com/ros2/rclcpp/issues/1073>`_)
* Added docblock in rclcpp (`#1103 <https://github.com/ros2/rclcpp/issues/1103>`_)
* Added Quality declaration: rclcpp, rclpp_action, rclcpp_components andrclcpp_lifecycle (`#1100 <https://github.com/ros2/rclcpp/issues/1100>`_)
* Use RCL_RET_SERVICE_TAKE_FAILED and not RCL_RET_CLIENT_TAKE_FAILED when checking a request take (`#1101 <https://github.com/ros2/rclcpp/issues/1101>`_)
* Update comment about return value in Executor::get_next_ready_executable (`#1085 <https://github.com/ros2/rclcpp/issues/1085>`_)
* Contributors: Alejandro Hernández Cordero, Christophe Bedard, Devin Bonnie, Ivan Santiago Paunovic

0.9.0 (2020-04-29)
------------------
* Serialized message move constructor (`#1097 <https://github.com/ros2/rclcpp/issues/1097>`_)
* Enforce a precedence for wildcard matching in parameter overrides. (`#1094 <https://github.com/ros2/rclcpp/issues/1094>`_)
* Add serialized_message.hpp header (`#1095 <https://github.com/ros2/rclcpp/issues/1095>`_)
* Add received message age metric to topic statistics (`#1080 <https://github.com/ros2/rclcpp/issues/1080>`_)
* Deprecate redundant namespaces (`#1083 <https://github.com/ros2/rclcpp/issues/1083>`_)
* Export targets in addition to include directories / libraries (`#1088 <https://github.com/ros2/rclcpp/issues/1088>`_)
* Ensure logging is initialized just once (`#998 <https://github.com/ros2/rclcpp/issues/998>`_)
* Adapt subscription traits to rclcpp::SerializedMessage (`#1092 <https://github.com/ros2/rclcpp/issues/1092>`_)
* Protect subscriber_statistics_collectors\_ with a mutex (`#1084 <https://github.com/ros2/rclcpp/issues/1084>`_)
* Remove unused test variable (`#1087 <https://github.com/ros2/rclcpp/issues/1087>`_)
* Use serialized message (`#1081 <https://github.com/ros2/rclcpp/issues/1081>`_)
* Integrate topic statistics (`#1072 <https://github.com/ros2/rclcpp/issues/1072>`_)
* Fix rclcpp interface traits test (`#1086 <https://github.com/ros2/rclcpp/issues/1086>`_)
* Generate node interfaces' getters and traits (`#1069 <https://github.com/ros2/rclcpp/issues/1069>`_)
* Use composition for serialized message (`#1082 <https://github.com/ros2/rclcpp/issues/1082>`_)
* Dnae adas/serialized message (`#1075 <https://github.com/ros2/rclcpp/issues/1075>`_)
* Reflect changes in rclcpp API (`#1079 <https://github.com/ros2/rclcpp/issues/1079>`_)
* Fix build regression (`#1078 <https://github.com/ros2/rclcpp/issues/1078>`_)
* Add NodeDefault option for enabling topic statistics (`#1074 <https://github.com/ros2/rclcpp/issues/1074>`_)
* Topic Statistics: Add SubscriptionTopicStatistics class (`#1050 <https://github.com/ros2/rclcpp/issues/1050>`_)
* Add SubscriptionOptions for topic statistics (`#1057 <https://github.com/ros2/rclcpp/issues/1057>`_)
* Remove warning message from failing to register default callback (`#1067 <https://github.com/ros2/rclcpp/issues/1067>`_)
* Create a default warning for qos incompatibility (`#1051 <https://github.com/ros2/rclcpp/issues/1051>`_)
* Add WaitSet class and modify entities to work without executor (`#1047 <https://github.com/ros2/rclcpp/issues/1047>`_)
* Include what you use (`#1059 <https://github.com/ros2/rclcpp/issues/1059>`_)
* Rename rosidl_generator_cpp namespace to rosidl_runtime_cpp (`#1060 <https://github.com/ros2/rclcpp/issues/1060>`_)
* Changed rosidl_generator_c/cpp to rosidl_runtime_c/cpp (`#1014 <https://github.com/ros2/rclcpp/issues/1014>`_)
* Use constexpr for endpoint type name (`#1055 <https://github.com/ros2/rclcpp/issues/1055>`_)
* Add InvalidParameterTypeException (`#1027 <https://github.com/ros2/rclcpp/issues/1027>`_)
* Support for ON_REQUESTED_INCOMPATIBLE_QOS and ON_OFFERED_INCOMPATIBLE_QOS events (`#924 <https://github.com/ros2/rclcpp/issues/924>`_)
* Fixup clang warning (`#1040 <https://github.com/ros2/rclcpp/issues/1040>`_)
* Adding a "static" single threaded executor (`#1034 <https://github.com/ros2/rclcpp/issues/1034>`_)
* Add equality operators for QoS profile (`#1032 <https://github.com/ros2/rclcpp/issues/1032>`_)
* Remove extra vertical whitespace (`#1030 <https://github.com/ros2/rclcpp/issues/1030>`_)
* Switch IntraProcessMessage to test_msgs/Empty (`#1017 <https://github.com/ros2/rclcpp/issues/1017>`_)
* Add new type of exception that may be thrown during creation of publisher/subscription (`#1026 <https://github.com/ros2/rclcpp/issues/1026>`_)
* Don't check lifespan on publisher QoS (`#1002 <https://github.com/ros2/rclcpp/issues/1002>`_)
* Fix get_parameter_tyeps of AsyncPrameterClient results are always empty (`#1019 <https://github.com/ros2/rclcpp/issues/1019>`_)
* Cleanup node interfaces includes (`#1016 <https://github.com/ros2/rclcpp/issues/1016>`_)
* Add ifdefs to remove tracing-related calls if tracing is disabled (`#1001 <https://github.com/ros2/rclcpp/issues/1001>`_)
* Include missing header in node_graph.cpp (`#994 <https://github.com/ros2/rclcpp/issues/994>`_)
* Add missing includes of logging.hpp (`#995 <https://github.com/ros2/rclcpp/issues/995>`_)
* Zero initialize publisher GID in subscription intra process callback (`#1011 <https://github.com/ros2/rclcpp/issues/1011>`_)
* Removed ament_cmake dependency (`#989 <https://github.com/ros2/rclcpp/issues/989>`_)
* Switch to using new rcutils_strerror (`#993 <https://github.com/ros2/rclcpp/issues/993>`_)
* Ensure all rclcpp::Clock accesses are thread-safe
* Use a PIMPL for rclcpp::Clock implementation
* Replace rmw_implementation for rmw dependency in package.xml (`#990 <https://github.com/ros2/rclcpp/issues/990>`_)
* Add missing service callback registration tracepoint (`#986 <https://github.com/ros2/rclcpp/issues/986>`_)
* Rename rmw_topic_endpoint_info_array count to size (`#996 <https://github.com/ros2/rclcpp/issues/996>`_)
* Implement functions to get publisher and subcription informations like QoS policies from topic name (`#960 <https://github.com/ros2/rclcpp/issues/960>`_)
* Code style only: wrap after open parenthesis if not in one line (`#977 <https://github.com/ros2/rclcpp/issues/977>`_)
* Accept taking an rvalue ref future in spin_until_future_complete (`#971 <https://github.com/ros2/rclcpp/issues/971>`_)
* Allow node clock use in logging macros (`#969 <https://github.com/ros2/rclcpp/issues/969>`_) (`#970 <https://github.com/ros2/rclcpp/issues/970>`_)
* Change order of deprecated and visibility attributes (`#968 <https://github.com/ros2/rclcpp/issues/968>`_)
* Deprecated is_initialized() (`#967 <https://github.com/ros2/rclcpp/issues/967>`_)
* Don't specify calling convention in std::_Binder template (`#952 <https://github.com/ros2/rclcpp/issues/952>`_)
* Added missing include to logging.hpp (`#964 <https://github.com/ros2/rclcpp/issues/964>`_)
* Assigning make_shared result to variables in test (`#963 <https://github.com/ros2/rclcpp/issues/963>`_)
* Fix unused parameter warning (`#962 <https://github.com/ros2/rclcpp/issues/962>`_)
* Stop retaining ownership of the rcl context in GraphListener (`#946 <https://github.com/ros2/rclcpp/issues/946>`_)
* Clear sub contexts when starting another init-shutdown cycle (`#947 <https://github.com/ros2/rclcpp/issues/947>`_)
* Avoid possible UB in Clock jump callbacks (`#954 <https://github.com/ros2/rclcpp/issues/954>`_)
* Handle unknown global ROS arguments (`#951 <https://github.com/ros2/rclcpp/issues/951>`_)
* Mark get_clock() as override to fix clang warnings (`#939 <https://github.com/ros2/rclcpp/issues/939>`_)
* Create node clock calls const (try 2) (`#922 <https://github.com/ros2/rclcpp/issues/922>`_)
* Fix asserts on shared_ptr::use_count; expects long, got uint32 (`#936 <https://github.com/ros2/rclcpp/issues/936>`_)
* Use absolute topic name for parameter events (`#929 <https://github.com/ros2/rclcpp/issues/929>`_)
* Add enable_rosout into NodeOptions. (`#900 <https://github.com/ros2/rclcpp/issues/900>`_)
* Removing "virtual", adding "override" keywords (`#897 <https://github.com/ros2/rclcpp/issues/897>`_)
* Use weak_ptr to store context in GraphListener (`#906 <https://github.com/ros2/rclcpp/issues/906>`_)
* Complete published event message when declaring a parameter (`#928 <https://github.com/ros2/rclcpp/issues/928>`_)
* Fix duration.cpp lint error (`#930 <https://github.com/ros2/rclcpp/issues/930>`_)
* Intra-process subscriber should use RMW actual qos. (ros2`#913 <https://github.com/ros2/rclcpp/issues/913>`_) (`#914 <https://github.com/ros2/rclcpp/issues/914>`_)
* Type conversions fixes (`#901 <https://github.com/ros2/rclcpp/issues/901>`_)
* Add override keyword to functions
* Remove unnecessary virtual keywords
* Only check for new work once in spin_some (`#471 <https://github.com/ros2/rclcpp/issues/471>`_) (`#844 <https://github.com/ros2/rclcpp/issues/844>`_)
* Add addition/subtraction assignment operators to Time (`#748 <https://github.com/ros2/rclcpp/issues/748>`_)
* Contributors: Alberto Soragna, Alejandro Hernández Cordero, Barry Xu, Chris Lalancette, Christophe Bedard, Claire Wang, Dan Rose, DensoADAS, Devin Bonnie, Dino Hüllmann, Dirk Thomas, DongheeYe, Emerson Knapp, Ivan Santiago Paunovic, Jacob Perron, Jaison Titus, Karsten Knese, Matt Schickler, Miaofei Mei, Michel Hidalgo, Mikael Arguedas, Monika Idzik, Prajakta Gokhale, Roger Strain, Scott K Logan, Sean Kelly, Stephen Brawner, Steven Macenski, Steven! Ragnarök, Todd Malsbary, Tomoya Fujita, William Woodall, Zachary Michaels

0.8.3 (2019-11-19)
------------------

0.8.2 (2019-11-18)
------------------
* Updated tracing logic to match changes in rclcpp's intra-process system (`#918 <https://github.com/ros2/rclcpp/issues/918>`_)
* Fixed a bug that prevented the ``shutdown_on_sigint`` option to not work correctly (`#850 <https://github.com/ros2/rclcpp/issues/850>`_)
* Added support for STREAM logging macros (`#926 <https://github.com/ros2/rclcpp/issues/926>`_)
* Relaxed multithreaded test constraint (`#907 <https://github.com/ros2/rclcpp/issues/907>`_)
* Contributors: Anas Abou Allaban, Christophe Bedard, Dirk Thomas, alexfneves

0.8.1 (2019-10-23)
------------------
* De-flake tests for rmw_connext (`#899 <https://github.com/ros2/rclcpp/issues/899>`_)
* rename return functions for loaned messages (`#896 <https://github.com/ros2/rclcpp/issues/896>`_)
* Enable throttling logs (`#879 <https://github.com/ros2/rclcpp/issues/879>`_)
* New Intra-Process Communication (`#778 <https://github.com/ros2/rclcpp/issues/778>`_)
* Instrumentation update (`#789 <https://github.com/ros2/rclcpp/issues/789>`_)
* Zero copy api (`#864 <https://github.com/ros2/rclcpp/issues/864>`_)
* Drop rclcpp remove_ros_arguments_null test case. (`#894 <https://github.com/ros2/rclcpp/issues/894>`_)
* add mechanism to pass rmw impl specific payloads during pub/sub creation (`#882 <https://github.com/ros2/rclcpp/issues/882>`_)
* make get_actual_qos return a rclcpp::QoS (`#883 <https://github.com/ros2/rclcpp/issues/883>`_)
* Fix Compiler Warning (`#881 <https://github.com/ros2/rclcpp/issues/881>`_)
* Add callback handler for use_sim_time parameter `#802 <https://github.com/ros2/rclcpp/issues/802>`_ (`#875 <https://github.com/ros2/rclcpp/issues/875>`_)
* Contributors: Alberto Soragna, Brian Marchi, Hunter L. Allen, Ingo Lütkebohle, Karsten Knese, Michael Carroll, Michel Hidalgo, William Woodall

0.8.0 (2019-09-26)
------------------
* clean up publisher and subscription creation logic (`#867 <https://github.com/ros2/rclcpp/issues/867>`_)
* Take parameter overrides provided through the CLI. (`#865 <https://github.com/ros2/rclcpp/issues/865>`_)
* add more context to exception message (`#858 <https://github.com/ros2/rclcpp/issues/858>`_)
* remove features and related code which were deprecated in dashing (`#852 <https://github.com/ros2/rclcpp/issues/852>`_)
* check valid timer handler 1st to reduce the time window for scan. (`#841 <https://github.com/ros2/rclcpp/issues/841>`_)
* Add throwing parameter name if parameter is not set (`#833 <https://github.com/ros2/rclcpp/issues/833>`_)
* Fix typo in deprecated warning. (`#848 <https://github.com/ros2/rclcpp/issues/848>`_)
* Fail on invalid and unknown ROS specific arguments (`#842 <https://github.com/ros2/rclcpp/issues/842>`_)
* Force explicit --ros-args in NodeOptions::arguments(). (`#845 <https://github.com/ros2/rclcpp/issues/845>`_)
* Use of -r/--remap flags where appropriate. (`#834 <https://github.com/ros2/rclcpp/issues/834>`_)
* Fix hang with timers in MultiThreadedExecutor (`#835 <https://github.com/ros2/rclcpp/issues/835>`_) (`#836 <https://github.com/ros2/rclcpp/issues/836>`_)
* add mutex in add/remove_node and wait_for_work to protect concurrent use/change of memory_strategy\_ (`#837 <https://github.com/ros2/rclcpp/issues/837>`_)
* Crash in callback group pointer vector iterator (`#814 <https://github.com/ros2/rclcpp/issues/814>`_)
* Wrap documentation examples in code blocks (`#830 <https://github.com/ros2/rclcpp/issues/830>`_)
* add callback group as member variable and constructor arg (`#811 <https://github.com/ros2/rclcpp/issues/811>`_)
* Fix get_node_interfaces functions taking a pointer (`#821 <https://github.com/ros2/rclcpp/issues/821>`_)
* Delete unnecessary call for get_node_by_group (`#823 <https://github.com/ros2/rclcpp/issues/823>`_)
* Allow passing logger by const ref (`#820 <https://github.com/ros2/rclcpp/issues/820>`_)
* Explain return value of spin_until_future_complete (`#792 <https://github.com/ros2/rclcpp/issues/792>`_)
* Adapt to '--ros-args ... [--]'-based ROS args extraction (`#816 <https://github.com/ros2/rclcpp/issues/816>`_)
* Add line break after first open paren in multiline function call (`#785 <https://github.com/ros2/rclcpp/issues/785>`_)
* remove mock msgs from rclcpp (`#800 <https://github.com/ros2/rclcpp/issues/800>`_)
* Make TimeSource ignore use_sim_time events coming from other nodes. (`#799 <https://github.com/ros2/rclcpp/issues/799>`_)
* Allow registering multiple on_parameters_set_callback (`#772 <https://github.com/ros2/rclcpp/issues/772>`_)
* Add free function for creating service clients (`#788 <https://github.com/ros2/rclcpp/issues/788>`_)
* Include missing rcl headers in use. (`#782 <https://github.com/ros2/rclcpp/issues/782>`_)
* Switch the NodeParameters lock to recursive. (`#781 <https://github.com/ros2/rclcpp/issues/781>`_)
* changed on_parameter_event qos profile to rmw_qos_profile_parameter_events (`#774 <https://github.com/ros2/rclcpp/issues/774>`_)
* Adding a factory method to create a Duration from seconds (`#567 <https://github.com/ros2/rclcpp/issues/567>`_)
* Fix a comparison with a sign mismatch (`#771 <https://github.com/ros2/rclcpp/issues/771>`_)
* delete superfluous spaces (`#770 <https://github.com/ros2/rclcpp/issues/770>`_)
* Use params from node '/\*\*' from parameter YAML file (`#762 <https://github.com/ros2/rclcpp/issues/762>`_)
* Add ignore override argument to declare parameter (`#767 <https://github.com/ros2/rclcpp/issues/767>`_)
* use default parameter descriptor in parameters interface (`#765 <https://github.com/ros2/rclcpp/issues/765>`_)
* Added support for const member functions (`#763 <https://github.com/ros2/rclcpp/issues/763>`_)
* add get_actual_qos() feature to subscriptions (`#754 <https://github.com/ros2/rclcpp/issues/754>`_)
* Ignore parameters overrides in set parameter methods when allowing undeclared parameters (`#756 <https://github.com/ros2/rclcpp/issues/756>`_)
* Add rclcpp::create_timer() (`#757 <https://github.com/ros2/rclcpp/issues/757>`_)
* checking origin of intra-process msg before taking them (`#753 <https://github.com/ros2/rclcpp/issues/753>`_)
* Contributors: Alberto Soragna, Carl Delsey, Chris Lalancette, Dan Rose, Dirk Thomas, Esteve Fernandez, Guillaume Autran, Jacob Perron, Karsten Knese, Luca Della Vedova, M. M, Michel Hidalgo, Scott K Logan, Shane Loretz, Todd Malsbary, William Woodall, bpwilcox, fujitatomoya, ivanpauno

0.7.5 (2019-05-30)
------------------
* Avoid 'Intra process message no longer being stored when trying to handle it' warning (`#749 <https://github.com/ros2/rclcpp/issues/749>`_)
* Contributors: ivanpauno

0.7.4 (2019-05-29)
------------------
* Rename parameter options (`#745 <https://github.com/ros2/rclcpp/issues/745>`_)
* Bionic use of strerror_r (`#742 <https://github.com/ros2/rclcpp/issues/742>`_)
* Enforce parameter ranges (`#735 <https://github.com/ros2/rclcpp/issues/735>`_)
* removed not used parameter client (`#740 <https://github.com/ros2/rclcpp/issues/740>`_)
* ensure removal of guard conditions of expired nodes from memory strategy (`#741 <https://github.com/ros2/rclcpp/issues/741>`_)
* Fix typo in log warning message (`#737 <https://github.com/ros2/rclcpp/issues/737>`_)
* Throw nice errors when creating a publisher with intraprocess communication and incompatible qos policy (`#729 <https://github.com/ros2/rclcpp/issues/729>`_)
* Contributors: Alberto Soragna, Dirk Thomas, Jacob Perron, William Woodall, ivanpauno, roderick-koehle

0.7.3 (2019-05-20)
------------------
* Fixed misspelling, volitile -> volatile (`#724 <https://github.com/ros2/rclcpp/issues/724>`_), and then fixed that since it is a C++ keyword to be ``durability_volatile`` (`#725 <https://github.com/ros2/rclcpp/issues/725>`_)
* Fixed a clang warning (`#723 <https://github.com/ros2/rclcpp/issues/723>`_)
* Added ``on_parameter_event`` static method to the ``AsyncParametersClient`` (`#688 <https://github.com/ros2/rclcpp/issues/688>`_)
* Added a guard against ``ParameterNotDeclaredException`` throwing from within the parameter service callbacks. (`#718 <https://github.com/ros2/rclcpp/issues/718>`_)
* Added missing template functionality to lifecycle_node. (`#707 <https://github.com/ros2/rclcpp/issues/707>`_)
* Fixed heap-use-after-free and memory leaks reported from ``test_node.cpp`` (`#719 <https://github.com/ros2/rclcpp/issues/719>`_)
* Contributors: Alberto Soragna, Dirk Thomas, Emerson Knapp, Jacob Perron, Michael Jeronimo, Prajakta Gokhale

0.7.2 (2019-05-08)
------------------
* Added new way to specify QoS settings for publishers and subscriptions. (`#713 <https://github.com/ros2/rclcpp/issues/713>`_)
  * The new way requires that you specify a history depth when creating a publisher or subscription.
  * In the past it was possible to create one without specifying any history depth, but these signatures have been deprecated.
* Deprecated ``shared_ptr`` and raw pointer versions of ``Publisher<T>::publish()``. (`#709 <https://github.com/ros2/rclcpp/issues/709>`_)
* Implemented API to set callbacks for liveliness and deadline QoS events for publishers and subscriptions. (`#695 <https://github.com/ros2/rclcpp/issues/695>`_)
* Fixed a segmentation fault when publishing a parameter event when they ought to be disabled. (`#714 <https://github.com/ros2/rclcpp/issues/714>`_)
* Changes required for upcoming pre-allocation API. (`#711 <https://github.com/ros2/rclcpp/issues/711>`_)
* Changed ``Node::get_node_names()`` to return the full node names rather than just the base name. (`#698 <https://github.com/ros2/rclcpp/issues/698>`_)
* Remove logic made redundant by the `ros2/rcl#255 <https://github.com/ros2/rcl/issues/255>`_ pull request. (`#712 <https://github.com/ros2/rclcpp/issues/712>`_)
* Various improvements for ``rclcpp::Clock``. (`#696 <https://github.com/ros2/rclcpp/issues/696>`_)
  * Fixed uninitialized bool in ``clock.cpp``.
  * Fixed up includes of ``clock.hpp/cpp``.
  * Added documentation for exceptions to ``clock.hpp``.
  * Adjusted function signature of getters of ``clock.hpp/cpp``.
  * Removed raw pointers to ``Clock::create_jump_callback``.
  * Removed unnecessary ``rclcpp`` namespace reference from ``clock.cpp``.
  * Changed exception to ``bad_alloc`` on ``JumpHandler`` allocation failure.
  * Fixed missing ``nullptr`` check in ``Clock::on_time_jump``.
  * Added ``JumpHandler::callback`` types.
  * Added warning for lifetime of Clock and JumpHandler
* Fixed bug left over from the `pull request #495 <https://github.com/ros2/rclcpp/pull/495>`_. (`#708 <https://github.com/ros2/rclcpp/issues/708>`_)
* Changed the ``IntraProcessManager`` to be capable of storing ``shared_ptr<const T>`` in addition to ``unique_ptr<T>``. (`#690 <https://github.com/ros2/rclcpp/issues/690>`_)
* Contributors: Alberto Soragna, Dima Dorezyuk, M. M, Michael Carroll, Michael Jeronimo, Tully Foote, William Woodall, ivanpauno, jhdcs

0.7.1 (2019-04-26)
------------------
* Added read only parameters. (`#495 <https://github.com/ros2/rclcpp/issues/495>`_)
* Fixed a concurrency problem in the multithreaded executor. (`#703 <https://github.com/ros2/rclcpp/issues/703>`_)
* Fixup utilities. (`#692 <https://github.com/ros2/rclcpp/issues/692>`_)
* Added method to read timer cancellation. (`#697 <https://github.com/ros2/rclcpp/issues/697>`_)
* Added Exception Generator function for implementing "from_rcl_error". (`#678 <https://github.com/ros2/rclcpp/issues/678>`_)
* Updated initialization of rmw_qos_profile_t struct instances. (`#691 <https://github.com/ros2/rclcpp/issues/691>`_)
* Removed the const value from the logger before comparison. (`#680 <https://github.com/ros2/rclcpp/issues/680>`_)
* Contributors: Devin Bonnie, Dima Dorezyuk, Guillaume Autran, M. M, Shane Loretz, Víctor Mayoral Vilches, William Woodall, jhdcs

0.7.0 (2019-04-14)
------------------
* Added Options-struct interfaces for creating publishers/subscribers (pre-QoS, standalone). (`#673 <https://github.com/ros2/rclcpp/issues/673>`_)
* Replaced strncpy with memcpy. (`#684 <https://github.com/ros2/rclcpp/issues/684>`_)
* Replaced const char * with a std::array<char, TOPIC_NAME_LENGTH> as the key of IPM IDTopicMap. (`#671 <https://github.com/ros2/rclcpp/issues/671>`_)
* Refactored SignalHandler logger to avoid race during destruction. (`#682 <https://github.com/ros2/rclcpp/issues/682>`_)
* Introduce rclcpp_components to implement composition. (`#665 <https://github.com/ros2/rclcpp/issues/665>`_)
* Added QoS policy check when configuring intraprocess, skip interprocess publish when possible. (`#674 <https://github.com/ros2/rclcpp/issues/674>`_)
* Updated to use do { .. } while(0) around content of logging macros. (`#681 <https://github.com/ros2/rclcpp/issues/681>`_)
* Added function to get publisher's actual QoS settings. (`#667 <https://github.com/ros2/rclcpp/issues/667>`_)
* Updated to avoid race that triggers timer too often. (`#621 <https://github.com/ros2/rclcpp/issues/621>`_)
* Exposed get_fully_qualified_name in NodeBase API. (`#662 <https://github.com/ros2/rclcpp/issues/662>`_)
* Updated to use ament_target_dependencies where possible. (`#659 <https://github.com/ros2/rclcpp/issues/659>`_)
* Fixed wait for service memory leak bug. (`#656 <https://github.com/ros2/rclcpp/issues/656>`_)
* Fixed test_time_source test. (`#639 <https://github.com/ros2/rclcpp/issues/639>`_)
* Fixed hard-coded duration type representation so int64_t isn't assumed. (`#648 <https://github.com/ros2/rclcpp/issues/648>`_)
* Fixed cppcheck warning. (`#646 <https://github.com/ros2/rclcpp/issues/646>`_)
* Added count matching api and intra-process subscriber count. (`#628 <https://github.com/ros2/rclcpp/issues/628>`_)
* Added Sub Node alternative. (`#581 <https://github.com/ros2/rclcpp/issues/581>`_)
* Replaced 'auto' with 'const auto &'. (`#630 <https://github.com/ros2/rclcpp/issues/630>`_)
* Set Parameter Event Publisher settings. `#591 <https://github.com/ros2/rclcpp/issues/591>`_ (`#614 <https://github.com/ros2/rclcpp/issues/614>`_)
* Replaced node constructor arguments with NodeOptions. (`#622 <https://github.com/ros2/rclcpp/issues/622>`_)
* Updated to pass context to wait set (`#617 <https://github.com/ros2/rclcpp/issues/617>`_)
* Added API to get parameters in a map. (`#575 <https://github.com/ros2/rclcpp/issues/575>`_)
* Updated Bind usage since it is is no longer in std::__1. (`#618 <https://github.com/ros2/rclcpp/issues/618>`_)
* Fixed errors from uncrustify v0.68. (`#613 <https://github.com/ros2/rclcpp/issues/613>`_)
* Added new constructors for SyncParameterClient. (`#612 <https://github.com/ros2/rclcpp/issues/612>`_)
* Contributors: Alberto Soragna, Chris Lalancette, Dirk Thomas, Emerson Knapp, Francisco Martín Rico, Jacob Perron, Marko Durkovic, Michael Carroll, Peter Baughman, Shane Loretz, Wei Liu, William Woodall, Yutaka Kondo, ivanpauno, kuzai, rarvolt

0.6.2 (2018-12-13)
------------------
* Updated to use signal safe synchronization with platform specific semaphores (`#607 <https://github.com/ros2/rclcpp/issues/607>`_)
* Resolved startup race condition for sim time (`#608 <https://github.com/ros2/rclcpp/issues/608>`_)
  Resolves `#595 <https://github.com/ros2/rclcpp/issues/595>`_
* Contributors: Tully Foote, William Woodall

0.6.1 (2018-12-07)
------------------
* Added wait_for_action_server() for action clients (`#598 <https://github.com/ros2/rclcpp/issues/598>`_)
* Added node path and time stamp to parameter event message (`#584 <https://github.com/ros2/rclcpp/issues/584>`_)
* Updated to allow removing a waitable (`#597 <https://github.com/ros2/rclcpp/issues/597>`_)
* Refactored init to allow for non-global init (`#587 <https://github.com/ros2/rclcpp/issues/587>`_)
* Fixed wrong use of constructor and hanging test (`#596 <https://github.com/ros2/rclcpp/issues/596>`_)
* Added class Waitable (`#589 <https://github.com/ros2/rclcpp/issues/589>`_)
* Updated rcl_wait_set_add\_* calls (`#586 <https://github.com/ros2/rclcpp/issues/586>`_)
* Contributors: Dirk Thomas, Jacob Perron, Shane Loretz, William Woodall, bpwilcox

0.6.0 (2018-11-19)
------------------
* Updated to use new error handling API from rcutils (`#577 <https://github.com/ros2/rclcpp/issues/577>`_)
* Added a warning when publishing if publisher is not active (`#574 <https://github.com/ros2/rclcpp/issues/574>`_)
* Added logging macro signature that accepts std::string (`#573 <https://github.com/ros2/rclcpp/issues/573>`_)
* Added virtual destructors to classes with virtual functions. (`#566 <https://github.com/ros2/rclcpp/issues/566>`_)
* Added semicolons to all RCLCPP and RCUTILS macros. (`#565 <https://github.com/ros2/rclcpp/issues/565>`_)
* Removed std::binary_function usage (`#561 <https://github.com/ros2/rclcpp/issues/561>`_)
* Updated to avoid auto-activating ROS time if clock topic is being published (`#559 <https://github.com/ros2/rclcpp/issues/559>`_)
* Fixed cpplint on xenial (`#556 <https://github.com/ros2/rclcpp/issues/556>`_)
* Added get_parameter_or_set_default. (`#551 <https://github.com/ros2/rclcpp/issues/551>`_)
* Added max_duration to spin_some() (`#558 <https://github.com/ros2/rclcpp/issues/558>`_)
* Updated to output rcl error message when yaml parsing fails (`#557 <https://github.com/ros2/rclcpp/issues/557>`_)
* Updated to make sure timer is fini'd before clock (`#553 <https://github.com/ros2/rclcpp/issues/553>`_)
* Get node names and namespaces (`#545 <https://github.com/ros2/rclcpp/issues/545>`_)
* Fixed and improved documentation  (`#546 <https://github.com/ros2/rclcpp/issues/546>`_)
* Updated to use rcl_clock_t jump callbacks (`#543 <https://github.com/ros2/rclcpp/issues/543>`_)
* Updated to use rcl consolidated wait set functions (`#540 <https://github.com/ros2/rclcpp/issues/540>`_)
* Addeed TIME_MAX and DURATION_MAX functions (`#538 <https://github.com/ros2/rclcpp/issues/538>`_)
* Updated to publish shared_ptr of rcl_serialized_message (`#541 <https://github.com/ros2/rclcpp/issues/541>`_)
* Added Time::is_zero and Duration::seconds (`#536 <https://github.com/ros2/rclcpp/issues/536>`_)
* Changed to log an error message instead of throwing exception in destructor (`#535 <https://github.com/ros2/rclcpp/issues/535>`_)
* Updated to relax tolerance of now test because timing affected by OS scheduling (`#533 <https://github.com/ros2/rclcpp/issues/533>`_)
* Removed incorrect exception on sec < 0 (`#527 <https://github.com/ros2/rclcpp/issues/527>`_)
* Added rclcpp::Time::seconds() (`#526 <https://github.com/ros2/rclcpp/issues/526>`_)
* Updated Timer API to construct TimerBase/GenericTimer with Clock (`#523 <https://github.com/ros2/rclcpp/issues/523>`_)
* Added rclcpp::is_initialized() (`#522 <https://github.com/ros2/rclcpp/issues/522>`_)
* Added support for jump handlers with only pre- or post-jump callback (`#517 <https://github.com/ros2/rclcpp/issues/517>`_)
* Removed use of uninitialized CMake var (`#512 <https://github.com/ros2/rclcpp/issues/512>`_)
* Updated for Uncrustify 0.67 (`#510 <https://github.com/ros2/rclcpp/issues/510>`_)
* Added get_node_names API from node. (`#508 <https://github.com/ros2/rclcpp/issues/508>`_)
* Contributors: Anis Ladram, Chris Lalancette, Dirk Thomas, Francisco Martín Rico, Karsten Knese, Michael Carroll, Mikael Arguedas, Sagnik Basu, Shane Loretz, Sriram Raghunathan, William Woodall, chapulina, dhood

0.5.0 (2018-06-25)
------------------
* Fixed a bug in the multi-threaded executor which could cause it to take a timer (potentially other types of wait-able items) more than once to be worked one. (`#383 <https://github.com/ros2/rclcpp/issues/383>`_)
  * Specifically this could result in a timer getting called more often that it should when using the multi-threaded executor.
* Added functions that allow you to publish serialized messages and received serialized messages in your subscription callback. (`#388 <https://github.com/ros2/rclcpp/issues/388>`_)
* Changed code to always get the Service name from ``rcl`` to ensure the remapped name is returned. (`#498 <https://github.com/ros2/rclcpp/issues/498>`_)
* Added previously missing ``set_parameters_atomically()`` method to the Service client interface. (`#494 <https://github.com/ros2/rclcpp/issues/494>`_)
* Added ability to initialize parameter values in a Node via a YAML file passed on the command line. (`#488 <https://github.com/ros2/rclcpp/issues/488>`_)
* Fixed the ROS parameter interface which got parameters that aren't set. (`#493 <https://github.com/ros2/rclcpp/issues/493>`_)
* Added ability to initialize parameter values in a node with an argument to the Node constructor. (`#486 <https://github.com/ros2/rclcpp/issues/486>`_)
* Added a ``Subscription`` tests which uses ``std::bind`` to a class member callback. (`#480 <https://github.com/ros2/rclcpp/issues/480>`_)
* Refactored the ``ParameterVariant`` class into the ``Parameter`` and ``ParameterValue`` classes. (`#481 <https://github.com/ros2/rclcpp/issues/481>`_)
* Relaxed template matching rules for ``std::bind`` and ``GNU C++ >= 7.1``. (`#484 <https://github.com/ros2/rclcpp/issues/484>`_)
* Changed to use the new ``rosgraph_msgs/Clock`` message type for the ``/clock`` topic. (`#474 <https://github.com/ros2/rclcpp/issues/474>`_)
* Fixed a flaky ROS time test due to not spinning before getting the time. (`#483 <https://github.com/ros2/rclcpp/issues/483>`_)
* Nodes now autostart the ROS parameter services which let you get, set, and list parameters in a node. (`#478 <https://github.com/ros2/rclcpp/issues/478>`_)
* Added support for arrays in Parameters. (`#443 <https://github.com/ros2/rclcpp/issues/443>`_)
* Changed how executors use ``AnyExecutable`` objects so that they are a reference instead of a shared pointer, in order to avoid memory allocation in the "common case". (`#463 <https://github.com/ros2/rclcpp/issues/463>`_)
* Added ability to pass command line arguments to the Node constructor. (`#461 <https://github.com/ros2/rclcpp/issues/461>`_)
* Added an argument to specify the number of threads a multithreaded executor should create. (`#442 <https://github.com/ros2/rclcpp/issues/442>`_)
* Changed library export order for static linking. (`#446 <https://github.com/ros2/rclcpp/issues/446>`_)
* Fixed some typos in the time unit tests. (`#453 <https://github.com/ros2/rclcpp/issues/453>`_)
  Obviously it mean RCL_SYSTEM_TIME but not RCL_ROS_TIME in some test cases
  * Signed-off-by: jwang <jing.j.wang@intel.com>
* Added the scale operation to ``rclcpp::Duration``.
  * Signed-off-by: jwang <jing.j.wang@intel.com>
* Changed API of the log location parameter to be ``const``. (`#451 <https://github.com/ros2/rclcpp/issues/451>`_)
* Changed how the subscriber, client, service, and timer handles are stored to resolve shutdown order issues. (`#431 <https://github.com/ros2/rclcpp/issues/431>`_ and `#448 <https://github.com/ros2/rclcpp/issues/448>`_)
* Updated to get the node's logger name from ``rcl``. (`#433 <https://github.com/ros2/rclcpp/issues/433>`_)
* Now depends on ``ament_cmake_ros``. (`#444 <https://github.com/ros2/rclcpp/issues/444>`_)
* Updaed code to use logging macros rather than ``fprintf()``. (`#439 <https://github.com/ros2/rclcpp/issues/439>`_)
* Fixed a bug that was using an invalid iterator when erasing items using an iterator in a loop. (`#436 <https://github.com/ros2/rclcpp/issues/436>`_)
* Changed code to support move of ``rcutils_time_point_value_t`` type from ``uint64_t`` to ``int64_t``. (`#429 <https://github.com/ros2/rclcpp/issues/429>`_)
* Renamed parameter byte type to ``byte_values`` from ``bytes_value``. (`#428 <https://github.com/ros2/rclcpp/issues/428>`_)
* Changed executor code to clear the wait set before resizing and waiting. (`#427 <https://github.com/ros2/rclcpp/issues/427>`_)
* Fixed a potential dereference of nullptr in the topic name validation error string. (`#405 <https://github.com/ros2/rclcpp/issues/405>`_)
  * Signed-off-by: Ethan Gao <ethan.gao@linux.intel.com>
* Changed to use ``rcl_count_publishers()`` like API's rather than the lower level ``rmw_count_publishers()`` API. (`#425 <https://github.com/ros2/rclcpp/issues/425>`_)
  * Signed-off-by: Sriram Raghunathan <rsriram7@visteon.com>
* Fix potential segmentation fault due to ``get_topic_name()`` or ``rcl_service_get_service_name()`` returning nullptr and that not being checked before access in ``rclcpp``. (`#426 <https://github.com/ros2/rclcpp/issues/426>`_)
  * Signed-off-by: Ethan Gao <ethan.gao@linux.intel.com>
* Contributors: Denise Eng, Dirk Thomas, Ernesto Corbellini, Esteve Fernandez, Ethan Gao, Guillaume Autran, Karsten Knese, Matthew, Michael Carroll, Mikael Arguedas, Shane Loretz, Sriram Raghunathan, Tom Moore, William Woodall, dhood, jwang, jwang11, serge-nikulin
