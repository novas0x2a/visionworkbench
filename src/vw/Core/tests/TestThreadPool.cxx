// __BEGIN_LICENSE__
// Copyright (C) 2006-2010 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#include <gtest/gtest.h>

#include <vw/Core/ThreadPool.h>

#include <iostream>

using namespace vw;

class TestTask : public Task {

  TestTask(TestTask& copy) {}
  void operator=(TestTask& copy) {}
  public:
  int m_value;
  bool terminate;
  Mutex test;
  TestTask() : m_value(0), terminate(false) {}

  void operator()() {
    m_value = 1;

    int count = 0;
    while( !terminate ) {
      Thread::sleep_ms(10);
      if (count++ > 500) {
        std::cerr << "Test thread iterated 100 times... it shouldn't take this long.  Maybe deadlock occured?" << std::endl;
        exit(-1);
      }
    }
    m_value = 3;
  }

  int value() { return m_value; }

  void kill() {
    terminate = true;
  }
};

TEST(ThreadPool, Basic) {
  boost::shared_ptr<TestTask> task1 (new TestTask);
  boost::shared_ptr<TestTask> task2 (new TestTask);
  boost::shared_ptr<TestTask> task3 (new TestTask);

  ASSERT_EQ( 0, task1->value() );
  ASSERT_EQ( 0, task2->value() );
  ASSERT_EQ( 0, task3->value() );

  set_debug_level(VerboseDebugMessage);

  FifoWorkQueue queue(4);
  queue.add_task(task1);
  queue.add_task(task2);
  queue.add_task(task3);

  // TODO: This is slightly sloppy -- we just wait a few hundred ms for the
  // thread to start and to set the flag.  This should really be done with a
  // condition variable.
  Thread::sleep_ms(200);
  EXPECT_EQ( 1, task1->value() );
  EXPECT_EQ( 1, task2->value() );
  EXPECT_EQ( 1, task3->value() );

  Thread::sleep_ms(100);
  task3->kill();
  Thread::sleep_ms(100);
  EXPECT_EQ( 1, task1->value() );
  EXPECT_EQ( 1, task2->value() );
  EXPECT_EQ( 3, task3->value() );

  task2->kill();
  task1->kill();

  queue.join_all();
}

TEST(ThreadPool, LimitedThreads) {
  boost::shared_ptr<TestTask> task1 (new TestTask);
  boost::shared_ptr<TestTask> task2 (new TestTask);
  boost::shared_ptr<TestTask> task3 (new TestTask);
  ASSERT_EQ( 0, task1->value() );
  ASSERT_EQ( 0, task2->value() );
  ASSERT_EQ( 0, task3->value() );

  set_debug_level(VerboseDebugMessage);
  FifoWorkQueue queue(2);
  queue.add_task(task1);
  queue.add_task(task2);
  queue.add_task(task3);

  // TODO: This is slightly sloppy -- we just wait a few hundred ms for
  // the thread to start and to set the flag.  This should really be
  // done with a condition variable.
  Thread::sleep_ms(200);
  EXPECT_EQ( 1, task1->value() );
  EXPECT_EQ( 1, task2->value() );

  Thread::sleep_ms(100);
  EXPECT_EQ( 0, task3->value() );
  task1->kill();
  Thread::sleep_ms(100);
  EXPECT_EQ( 3, task1->value() );
  EXPECT_EQ( 1, task2->value() );
  EXPECT_EQ( 1, task3->value() );

  task2->kill();
  task3->kill();

  queue.join_all();
}


TEST(ThreadPool, SpawnNewTasks) {
  boost::shared_ptr<TestTask> task1 (new TestTask);
  boost::shared_ptr<TestTask> task2 (new TestTask);
  boost::shared_ptr<TestTask> task3 (new TestTask);
  ASSERT_EQ( 0, task1->value() );
  ASSERT_EQ( 0, task2->value() );
  ASSERT_EQ( 0, task3->value() );

  set_debug_level(VerboseDebugMessage);
  FifoWorkQueue queue(8);
  queue.add_task(task1);
  queue.add_task(task2);
  queue.add_task(task3);

  // Give the tasks a chance to start up...
  Thread::sleep_ms(100);

  set_debug_level(VerboseDebugMessage);

  EXPECT_EQ( 1, task1->value() );
  EXPECT_EQ( 1, task2->value() );

  Thread::sleep_ms(100);
  task1->kill();
  Thread::sleep_ms(100);
  EXPECT_EQ( 3, task1->value() );
  EXPECT_EQ( 1, task2->value() );
  EXPECT_EQ( 1, task3->value() );


  boost::shared_ptr<TestTask> task4 (new TestTask);
  boost::shared_ptr<TestTask> task5 (new TestTask);
  boost::shared_ptr<TestTask> task6 (new TestTask);
  ASSERT_EQ( 0, task4->value() );
  ASSERT_EQ( 0, task5->value() );
  ASSERT_EQ( 0, task6->value() );
  queue.add_task(task4);
  queue.add_task(task5);
  queue.add_task(task6);
  Thread::sleep_ms(100);
  EXPECT_EQ( 1, task4->value() );
  EXPECT_EQ( 1, task5->value() );
  EXPECT_EQ( 1, task6->value() );

  task2->kill();
  task3->kill();

  task4->kill();
  task5->kill();
  task6->kill();
  queue.join_all();
}

TEST(ThreadPool, OrderedWorkQueue) {
  boost::shared_ptr<TestTask> task1 (new TestTask);
  boost::shared_ptr<TestTask> task2 (new TestTask);
  boost::shared_ptr<TestTask> task3 (new TestTask);
  ASSERT_EQ( 0, task1->value() );
  ASSERT_EQ( 0, task2->value() );
  ASSERT_EQ( 0, task3->value() );

  set_debug_level(VerboseDebugMessage);
  OrderedWorkQueue queue(8);
  queue.add_task(task1, 2);
  queue.add_task(task2, 1);

  set_debug_level(VerboseDebugMessage);

  ASSERT_EQ( 0, task1->value() );
  ASSERT_EQ( 0, task2->value() );
  ASSERT_EQ( 0, task3->value() );

  queue.add_task(task3, 0);
  Thread::sleep_ms(100);
  EXPECT_EQ( 1, task1->value() );
  EXPECT_EQ( 1, task2->value() );
  EXPECT_EQ( 1, task3->value() );

  boost::shared_ptr<TestTask> task4 (new TestTask);
  boost::shared_ptr<TestTask> task5 (new TestTask);
  boost::shared_ptr<TestTask> task6 (new TestTask);
  ASSERT_EQ( 0, task4->value() );
  ASSERT_EQ( 0, task5->value() );
  ASSERT_EQ( 0, task6->value() );
  queue.add_task(task4, 3);
  queue.add_task(task5, 5);
  Thread::sleep_ms(100);
  EXPECT_EQ( 1, task4->value() );
  EXPECT_EQ( 0, task5->value() );
  EXPECT_EQ( 0, task6->value() );

  queue.add_task(task6, 4);
  Thread::sleep_ms(100);
  EXPECT_EQ( 1, task4->value() );
  EXPECT_EQ( 1, task5->value() );
  EXPECT_EQ( 1, task6->value() );

  task1->kill();
  task2->kill();
  task3->kill();
  task4->kill();
  task5->kill();
  task6->kill();

  queue.join_all();
}
