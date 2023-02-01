#include"tw.h"
#include"functional"
#include <sys/time.h>
#include<bits/stdc++.h>
#include<vector>
#define first_stage_num 60
#define second_stage_num 60
#define third_stage_num 24
class tw_manager{
    private:
        
        std::vector<std::vector<Timer *>> first,second,third;
        int p_first,p_second,p_third;
        unsigned long long getCurrentMillisecs(){
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
            return ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
        }
    public:
        int memory;
        tw_manager(){
            first = std::vector<std::vector<Timer *>>(first_stage_num);
            second = std::vector<std::vector<Timer *>>(second_stage_num);
            third = std::vector<std::vector<Timer *>>(third_stage_num);

            p_first = getCurrentMillisecs() % first_stage_num;
            p_second = 0;
            p_third = 0;
            memory = 0;
        }
        Timer* add_timer(int timeout,std::function<void(void)>fun,void *arg){
            //int curr = getCurrentMillisecs() % first_stage_num;
            int now =  getCurrentMillisecs() % first_stage_num;
            int expire = timeout + now;
            Timer *timer;
            memory++;
            if( (now + timeout) < first_stage_num){
                int slot = expire % first_stage_num;
                timer = new Timer(expire,slot,fun,nullptr);
                first[slot].push_back(timer);
            }
            if((now + timeout)>=first_stage_num && (now + timeout) < (first_stage_num*second_stage_num)){
                int slot = (expire / first_stage_num + p_second) % second_stage_num;
                timer = new Timer(expire,slot,fun,nullptr);
                second[slot].push_back(timer);
            }
            if((now + timeout)>=(first_stage_num*second_stage_num)){
                int slot = (expire / (first_stage_num * second_stage_num) + p_third) % third_stage_num;
                timer = new Timer(expire,slot,fun,nullptr);
                third[slot].push_back(timer);
            }
            return timer;
        }
        void del_timer(Timer *timer){
            timer->delete_();
        }
        void deal_timers(){  //1ms

            int sec=0,thir=0;
            p_first = (p_first + 1) % first_stage_num;

            if(p_first == 0){
                sec = 1 ; 
               p_second = (p_second + 1) % second_stage_num;
            }
            if(p_second == 0 && sec == 1){
                thir = 1;
                p_third = (p_third + 1) % third_stage_num;
            }

            if(thir == 1){
                for(std::vector<Timer *>::iterator it = third[p_third].begin();it!=third[p_third].end();){
                   int thr_to_sec= (((*it)->get_expire()) % (first_stage_num * second_stage_num)) / first_stage_num;
                   second[thr_to_sec].push_back(*it);
                   it = third[p_third].erase(it);
                }
            }

            if(sec == 1){
                for(std::vector<Timer *>::iterator it = second[p_second].begin();it!=second[p_second].end();){
                   int sec_to_fir = ((*it)->get_expire()) % first_stage_num;
                   first[sec_to_fir].push_back(*it);
                   it = second[p_second].erase(it);
                }
            }
            
            std::vector<Timer *>::iterator it;
            for(it = first[p_first].begin();it !=  first[p_first].end();){
                (*it)->do_func();
                std::cout<<getCurrentMillisecs()<<std::endl;;
                delete (*it);
                memory--;
                it = first[p_first].erase(it);
            }

        }

};