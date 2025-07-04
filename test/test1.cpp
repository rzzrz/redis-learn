#include<iostream>

#include<hiredis/hiredis.h>

int main(){
    redisContext *c = redisConnect("127.0.0.1",6379);

    if(c == nullptr || c->err){
        if(c != nullptr){
            std::cout<<"errot : "<<c->err<<std::endl;
        }else{
            std::cout<<"can`t allocate redis context"<<std::endl;
        }
    }
    redisReply* reply = 
    static_cast<redisReply*>(redisCommand(c,"SET string learn-redis!!"));

    std::cout<<reply->str<<std::endl;

    reply = static_cast<redisReply*>(redisCommand(c,"GET string"));

    std::cout<<reply->str<<std::endl;

    freeReplyObject(reply);
    redisFree(c);

    std::cout<<"测试成功"<<std::endl;

    return 0;
}