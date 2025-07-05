#ifndef _CLASS_REDIS_H___
#define _CLASS_REDIS_H___

#include <hiredis/hiredis.h>
#include <hiredis/read.h>
#include <iostream>


class My_redis{
private:
    redisContext* context;
public:
    My_redis(){
        context = redisConnect("127.0.0.1",6379);
    }
    My_redis(redisContext* c){
        context = c;
    }

    template<typename ... Args>
    void command(Args&& ... args){
        redisReply* reply = 
        static_cast<redisReply*>(
            redisCommand(context,std::forward<Args>(args)...)
        );
        print_reply(reply);
        freeReplyObject(reply);
    }
    private:
    void print_reply(const redisReply* reply){
        if(reply != nullptr)
        switch(reply->type){
            case REDIS_REPLY_NIL:
                std::cout<<"nil"<<std::endl;
                break;
            case REDIS_REPLY_STRING:
                std::cout<<reply->str<<std::endl;
                break;
            case REDIS_REPLY_ARRAY:
                for(int i = 0 ;i< reply->elements;++i){
                    if(reply->element[i]->type == REDIS_REPLY_NIL)
                        std::cout<<"nil"<<std::endl;
                    else{
                        std::cout<<reply->element[i]->str<<std::endl;
                    }
                }
                break;
            case REDIS_REPLY_INTEGER:
                std::cout<<reply->integer<<std::endl;
                break;
            case REDIS_REPLY_ERROR:
                std::cout<<reply->str<<std::endl;
                break;
            default:
                std::cout<<"unkown reply type"<<std::endl;
        }
        else{
            std::cout<<"reply wrong"<<std::endl;
        }
    }
};

#endif // _CLASS_REDIS_H___