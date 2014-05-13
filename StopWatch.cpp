#include "XApplication.h"
#include "StopWatch.h"
#include <sstream>
#include <time.h>

using namespace std;

namespace cs349 {

  StopWatch::StopWatch(EventQueue* eventQueue, const Rectangle & bounds) : XWindow(string("Stop Watch!"), bounds) {
    this->running = false;
    this->timer = new Timer(eventQueue, 100, true, this);
    timer->Start();
    startTimeMS = 0;
      this->mouseDown = false;
      this->paused = false;
      this->isReset = false;
      
      min = 0;
      sec = 0;
      time_sec = 0;
      milisec = 0;
      time = 0;
      pre_time = 0;
      
      down_at_start = false;
      down_at_pause = false;
      down_at_reset = false;
      in_start_button = false;
      in_pause_button = false;
      in_reset_button = false;
  }

  StopWatch::~StopWatch() {
    timer->Stop();
    delete timer;
    timer = NULL;
  }

  unsigned long StopWatch::GetCurTime() {
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
  }
    
	
  void StopWatch::Paint(Graphics* g) {
      // TODO CS349
      if(this->isReset){//check if now is at reset
          min = 0;
          sec = 0;
          milisec = 0;
      }
      stringstream ss;
      ss<<min;
      stringstream ss2;
      ss2<<sec;
      stringstream ss3;
      ss3<<milisec;
      //repaint stopwatch area
      g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
      g->FillRect(Rectangle(0,0,500,300));
      g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
      //start button
      if(down_at_start && in_start_button){//if the button has been press without release
          g->FillRect(Rectangle(50,230,60,40));
          g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
          if(!this->paused){
              g->DrawText(Point(65,250), string("start"));
          }else{
              g->DrawText(Point(55,250), string("continue"));
          }
          g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
      }else{
          g->DrawRect(Rectangle(50,230,60,40));
          if(!this->paused){
              g->DrawText(Point(65,250), string("start"));
          }else{
              g->DrawText(Point(55,250), string("continue"));
          }
      }
      //handle pause button
      if(down_at_pause && in_pause_button){
          g->FillRect(Rectangle(200,230,60,40));
          g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
          g->DrawText(Point(215,250), string("pause"));
          g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
      }else{
          g->DrawRect(Rectangle(200,230,60,40));
          g->DrawText(Point(215,250), string("pause"));
      }
      //reset button
      if(this->paused){
          if(down_at_reset && in_reset_button){
              g->FillRect(Rectangle(350,230,60,40));
              g->SetForegroundColor(XApplication::GetInstance()->GetBlackColor());
              g->DrawText(Point(365,250), string("reset"));
              g->SetForegroundColor(XApplication::GetInstance()->GetWhiteColor());
          }else{
              g->DrawRect(Rectangle(350,230,60,40));
              g->DrawText(Point(365,250), string("reset"));
          }
      }
      
      //percentage bar
      g->DrawRect(Rectangle(50,60,30,150));
      g->DrawRect(Rectangle(230,60,30,150));
      g->DrawRect(Rectangle(400,60,30,150));
      
      int min_p = min*100/10;//get the percentage of minute
      if(min_p>99){
          min_p %= 100;
      }
      int sec_p = sec*100/60;
      unsigned long milisec_p = milisec*100/1000;
      
      
      if(milisec_p<11 && min_p==0 && sec_p==0 && min>0){
          g->DrawText(Point(50,20), string("MIN: "));
          g->DrawText(Point(80,20), ss.str());
          stringstream ss4;
          ss4<<min_p;
          g->DrawText(Point(50,40), "100%");
          g->FillRect(Rectangle(50,60,30,150));
      }
      else{
          if(!(milisec_p<11 && sec_p==0 && time_sec>0)){
              g->DrawText(Point(50,20), string("MIN: "));
              g->DrawText(Point(80,20), ss.str());
              stringstream ss4;
              ss4<<min_p;
              g->DrawText(Point(50,40), ss4.str()+"%");
              g->FillRect(Rectangle(50,210-150*min_p/100,30,150*min_p/100));
          }
      }
      
      if(milisec_p<11 && sec_p==0 && time_sec>0){
          g->DrawText(Point(230,20), string("SEC: "));
          g->DrawText(Point(260,20), "60");
          g->DrawText(Point(230,40), "100%");
          g->FillRect(Rectangle(230,60,30,150));
      }
      else{
          g->DrawText(Point(230,20), string("SEC: "));
          g->DrawText(Point(260,20), ss2.str());
          stringstream ss5;
          ss5<<sec_p;
          g->DrawText(Point(230,40), ss5.str()+"%");
          g->FillRect(Rectangle(230,210-150*sec_p/100,30,150*sec_p/100));
      }
      
      if(milisec_p>=89){
          g->DrawText(Point(400,20), string("MILISEC: "));
          g->DrawText(Point(450,20), "1000");
          stringstream ss6;
          ss6<<milisec_p;
          g->DrawText(Point(400,40), "100%");
          g->FillRect(Rectangle(400,60,30,150));
          
      }
      else{
          g->DrawText(Point(400,20), string("MILISEC: "));
          g->DrawText(Point(450,20), ss3.str());
          stringstream ss6;
          ss6<<milisec_p;
          g->DrawText(Point(400,40), ss6.str()+"%");
          g->FillRect(Rectangle(400,210-150*milisec_p/100,30,150*milisec_p/100));
      }
      if(IsRunning()){//if now is running, recalculate elapsed time
          time = GetCurTime() - this->startTimeMS+pre_time;
          time_sec = (int)time/1000;
          milisec = time%1000;
          
          min = time_sec/60;
          sec = time_sec%60;
      }
  }
  bool StopWatch::HandleMouseEvent(const MouseEvent & e) {
    // TODO: Remove the following code and add your own event handling
    // code to handle mouse events
    LOG_DEBUG << "Received HandleMouseEvent in StopWatch" << e;

// TODO CS349
      switch (e.GetType()) {
          case MouseEvent::mouseDown:
              this->mouseDown = true;
              //mouse down at start button
              if(e.GetWhere().x>=50 && e.GetWhere().x<=110 && e.GetWhere().y>=230 &&e.GetWhere().y<=270){
                  down_at_start = true;
                  in_start_button = true;
                  if(!IsRunning())
                  Component::Repaint();
              }
              else if(e.GetWhere().x>=200 && e.GetWhere().x<=260 && e.GetWhere().y>=230 &&e.GetWhere().y<=270){
                  //pause button
                  down_at_pause = true;
                  in_pause_button = true;
                  if(!IsRunning())
                  Component::Repaint();
              }
              else if(e.GetWhere().x>=350 && e.GetWhere().x<=410 && e.GetWhere().y>=230 &&e.GetWhere().y<=270){
                  down_at_reset = true;
                  in_reset_button = true;
                  if(!IsRunning())
                  Component::Repaint();
              }else;
              break;
          case MouseEvent::mouseUp:
              //mouse up from start button
              if(e.GetWhere().x>=50 && e.GetWhere().x<=110 && e.GetWhere().y>=230 &&e.GetWhere().y<=270){
                  if(!IsRunning() && down_at_start){
                      down_at_start = false;
                      this->isReset = false;
                      this->paused = false;
                      Start();
                  }
              }
              else if(e.GetWhere().x>=200 && e.GetWhere().x<=260 && e.GetWhere().y>=230 &&e.GetWhere().y<=270){
                  //pause button
                  if(down_at_pause){
                      down_at_pause = false;
                      if(IsRunning()){
                          this->paused = true;
                          Stop();
                          pre_time = time;
                      }
                  }
              }
              else if(e.GetWhere().x>=350 && e.GetWhere().x<=410 && e.GetWhere().y>=230 &&e.GetWhere().y<=270){
                  if(!IsRunning() && down_at_reset && this->paused){
                      down_at_reset = false;
                      this->isReset = true;
                      pre_time = 0;
                      time = 0;
                      time_sec = 0;
                      this->paused = false;
                  }
              }else;
              down_at_start = false;
              down_at_pause = false;
              down_at_reset = false;
              if(!IsRunning())
              Component::Repaint();
              break;
          case MouseEvent::mouseMove:
              if(!(e.GetWhere().x>=50 && e.GetWhere().x<=110 && e.GetWhere().y>=230 &&e.GetWhere().y<=270)){
                  //the mouse is out of start button
                  in_start_button = false;
                  if(down_at_start && !IsRunning()){
                      Component::Repaint();
                  }
              }
              else{
                  in_start_button = true;
                  if(down_at_start && !IsRunning()){
                      Component::Repaint();
                  }
              }
              
              if(!(e.GetWhere().x>=200 && e.GetWhere().x<=260 && e.GetWhere().y>=230 &&e.GetWhere().y<=270)){
                  in_pause_button = false;
                  if(down_at_pause && !IsRunning()){
                      Component::Repaint();
                  }
              }
              else{
                  in_pause_button = true;
                  if(down_at_pause && !IsRunning()){
                      Component::Repaint();
                  }
              }
              
              if(!(e.GetWhere().x>=350 && e.GetWhere().x<=410 && e.GetWhere().y>=230 &&e.GetWhere().y<=270)){
                  in_reset_button = false;
                  if(down_at_reset && !IsRunning()){
                    Component::Repaint();
                  }
              }else{
                  in_reset_button = true;
                  if(down_at_reset && !IsRunning()){
                      Component::Repaint();
                  }
              }
              break;
          case MouseEvent::mouseDrag:
              break;
          default:
              break;
      }
    return true;
  }

  void StopWatch::HandleTimerEvent(const cs349::TimerEvent & e) {
    if (running) {
      Component::Repaint();
    }
  }

  void StopWatch::Start() {
    this->running = true;
    startTimeMS = GetCurTime();
  }

  void StopWatch::Stop() {
    this->running = false;
  }

  bool StopWatch::IsRunning() const {
    return running;
  }
}
