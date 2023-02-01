#include <sys/time.h>
#include"functional"
#include<vector>
#include<bits/stdc++.h>
class Timer{
    private:
      Timer *next_timer;
      std::function<void(void)>fun;
      void * arg;
      unsigned long long expire;
      int slot;
      bool check_if_delete;

      unsigned long long getCurrentMillisecs(){
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
            return ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
      }
    
    public:
      Timer(unsigned long long timeout,int slot,std::function<void(void)>fun,void *arg){
          expire = timeout;// + getCurrentMillisecs();
          this->fun = fun;
          this->arg = arg;
          this->slot = slot;
          this->check_if_delete = 0;
          next_timer = nullptr;
      }
      unsigned long long get_expire(){
          return expire;
      }
      void delete_(){
        this->check_if_delete = 1;
        return;
      }
      
      int get_slot(){
          return slot;
      }
      void do_func(){
        if(!check_if_delete){
          fun();
        }else{
          std::cout<<"已删除"<<std::endl;
        }
      }
};