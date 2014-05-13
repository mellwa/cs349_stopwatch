// A1 Unit Tests

#include "gtest/gtest.h"
#include "StopWatch.h"
#include "XApplication.h"

using namespace cs349;

TEST(A1UnitTestDemos, Construction) {
  StopWatch* watch = new StopWatch(XApplication::GetInstance()->GetEventQueue(), Rectangle(0, 0, 200, 75));
  EXPECT_FALSE(watch == NULL);
  EXPECT_FALSE(watch->IsRunning());

  EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
  EXPECT_FALSE(queue == NULL);
  queue->ClearEventQueue(); // Need to clear the event queue of any
                            // events our watch window has generated,
                            // or else subsequent processing of those
                            // events will fail miserably (i.e., core
                            // dump) if events from the event queue
                            // are processed after the watch pointer
                            // is deleted below.

  delete watch;
}

TEST(A1UnitTestDemos, Interaction) {
  StopWatch* watch = new StopWatch(XApplication::GetInstance()->GetEventQueue(), Rectangle(0, 0, 200, 75));
  EXPECT_FALSE(watch == NULL);

  // Create a synthetic mouse event to test whether watch responds to it
  // or not. Note that this assumes that clicking in the location
  // specified amounts to pressing the start/stop button. Your actual
  // interaction will likely be different, making this test useless.
  // However, this should provide a template for how to do unit tests
  // for interaction.
    EXPECT_FALSE(watch->GetParentWindow() == NULL);
  MouseEvent* e = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseDown, Point(55, 235));
  MouseEvent* e2 = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseUp, Point(55, 235));

  EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
  EXPECT_FALSE(queue == NULL);

  EXPECT_FALSE(watch->IsRunning());
  queue->AddEventToQueue(e);
  queue->AddEventToQueue(e2);
  unsigned int max_num_tries_to_flush_queue = 10;
  while (max_num_tries_to_flush_queue-- > 0
         && queue->GetNumEventsInQueue() > 0
         && !watch->IsRunning())
    {
      queue->ProcessNextEvent();
    }
  EXPECT_TRUE(watch->IsRunning());

  queue->ClearEventQueue();
  delete watch;
  // We do not need to delete the mouse event that we created, because
  // it will be deleted automatically by the EventQueue.
}

//visible test for start stopwatch and pause stopwatch
TEST(A1UnitTestDemos, Start_Pause) {//To test press start button first and press pause later
    bool start = false;
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    //create timer
    unsigned long currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime2 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime3 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime4 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime5 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    XApplication* app = XApplication::GetInstance();
    
    StopWatch* watch = new StopWatch(app->GetEventQueue(), Rectangle(100, 100, 500, 300));
   
    watch->SetVisible(true);
    XFlush(app->XApplication::GetInstance()->GetXDisplay());
    MouseEvent* e;
    MouseEvent* e2;
    EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
    EXPECT_FALSE(watch->IsRunning());
    while(1){
        clock_gettime(CLOCK_REALTIME, &tp);
        currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        XFlush(app->XApplication::GetInstance()->GetXDisplay());
        queue->ProcessNextEvent();
        if(currentTime - preTime4 > 5000 && !start){
            e = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseDown, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime4 = currentTime;
        }
        if(currentTime - preTime5 > 5500 && !start){
            e = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseUp, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime5 = currentTime;
            start = true;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch->IsRunning());
        }
        
        if(currentTime - preTime > 10000){
            e = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseDown, Point(225, 235));
            queue->AddEventToQueue(e);
            preTime = currentTime;
            queue->ProcessNextEvent();
        }
        if (currentTime-preTime2 > 11000) {
            e2 = new MouseEvent(watch->GetParentWindow(), MouseEvent::mouseUp, Point(225, 235));
            queue->AddEventToQueue(e2);
            preTime2 = currentTime;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch->paused);
        }
        if(currentTime - preTime3 > 13000){//since the program has run 13 sec, terminate it.
            break;
        }
            
        usleep(20000);
    }
    queue->ClearEventQueue();
    delete watch;
}

TEST(A1UnitTestDemos, Start_Pause_continue_pause) {//Press start first and press pause few sec later, and then, test continue, and press pause again after restart stopwatch
    bool start = false;
    bool click_pause = false;
    bool restart = false;
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    unsigned long currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime2 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime3 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime4 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime5 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime6 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime7 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    XApplication* app = XApplication::GetInstance();
    
    StopWatch* watch2 = new StopWatch(app->GetEventQueue(), Rectangle(0, 0, 500, 300));

    watch2->SetVisible(true);
    XFlush(app->XApplication::GetInstance()->GetXDisplay());
    MouseEvent* e;
    MouseEvent* e2;
    EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
    EXPECT_FALSE(watch2->IsRunning());
    while(1){
        clock_gettime(CLOCK_REALTIME, &tp);
        currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        usleep(20000);
        queue->ProcessNextEvent();
        XFlush(app->XApplication::GetInstance()->GetXDisplay());
        if(currentTime - preTime4 > 2000 && !start){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseDown, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime4 = currentTime;
            queue->ProcessNextEvent();
        }
        if(currentTime - preTime5 > 2500 && !start){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseUp, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime5 = currentTime;
            start = true;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch2->IsRunning());
        }
        
        if(currentTime - preTime > 5000 && !click_pause){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseDown, Point(225, 235));
            queue->AddEventToQueue(e);
            preTime = currentTime;
            queue->ProcessNextEvent();
        }
        if (currentTime-preTime2 > 5500 && !click_pause) {
            e2 = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseUp, Point(225, 235));
            queue->AddEventToQueue(e2);
            preTime2 = currentTime;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch2->paused);
            click_pause = true;
        }
        if(currentTime - preTime3 > 8000 && !restart){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseDown, Point(55, 235));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            preTime3 = currentTime;
        }
        if(currentTime - preTime6 > 8500 && !restart){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseUp, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime6 = currentTime;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch2->IsRunning());
            restart = true;
            click_pause = false;
        }
        
        if(currentTime - preTime7 > 16000){//since the program has run 16 sec, terminate it.
            break;
        }
        
    }
    queue->ClearEventQueue();
    delete watch2;
}


TEST(A1UnitTestDemos, Start_Pause_Reset) {//test start -> pause -> reset
    bool start = false;
    bool click_pause = false;
    bool reset = false;
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    unsigned long currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime2 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime3 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime4 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime5 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime6 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    unsigned long preTime7 = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
    XApplication* app = XApplication::GetInstance();
    
    StopWatch* watch2 = new StopWatch(app->GetEventQueue(), Rectangle(0, 0, 500, 300));

    watch2->SetVisible(true);
    XFlush(app->XApplication::GetInstance()->GetXDisplay());
    MouseEvent* e;
    MouseEvent* e2;
    EventQueue* queue = XApplication::GetInstance()->GetEventQueue();
    EXPECT_FALSE(watch2->IsRunning());
    while(1){
        clock_gettime(CLOCK_REALTIME, &tp);
        currentTime = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        usleep(20000);
        queue->ProcessNextEvent();
        XFlush(app->XApplication::GetInstance()->GetXDisplay());
        if(currentTime - preTime4 > 5000 && !start){//press start button after 5 sec
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseDown, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime4 = currentTime;
            queue->ProcessNextEvent();
        }
        if(currentTime - preTime5 > 5500 && !start){//after 0.5 sec since press start, release button
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseUp, Point(55, 235));
            queue->AddEventToQueue(e);
            preTime5 = currentTime;
            start = true;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch2->IsRunning());
        }
        
        if(currentTime - preTime > 10000 && !click_pause){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseDown, Point(225, 235));
            queue->AddEventToQueue(e);
            preTime = currentTime;
            queue->ProcessNextEvent();
        }
        if (currentTime-preTime2 > 11000 && !click_pause) {
            e2 = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseUp, Point(225, 235));
            queue->AddEventToQueue(e2);
            preTime2 = currentTime;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_TRUE(watch2->paused);
            click_pause = true;
        }
        if(currentTime - preTime3 > 13000 && !reset){//after 13 sec press reset
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseDown, Point(375, 235));
            queue->AddEventToQueue(e);
            queue->ProcessNextEvent();
            preTime3 = currentTime;
        }
        if(currentTime - preTime6 > 13500 && !reset){
            e = new MouseEvent(watch2->GetParentWindow(), MouseEvent::mouseUp, Point(375, 235));
            queue->AddEventToQueue(e);
            preTime6 = currentTime;
            while(queue->GetNumEventsInQueue()>0)
            queue->ProcessNextEvent();
            EXPECT_FALSE(watch2->IsRunning());
            reset = true;
        }
        
        if(currentTime - preTime7 > 16000){//since the program has run 16 sec, terminate it.
            break;
        }
        
    }
    queue->ClearEventQueue();
    delete watch2;
}
