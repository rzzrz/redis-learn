#include <hiredis/hiredis.h>
#include <nlohmann/json.hpp>
#include <hiredis/read.h>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <thread>

using json = nlohmann::json;

class Product {
private:
    std::string id_;
    double price_;
    
public:
    Product(std::string id, double price) : id_(std::move(id)), price_(price) {}
    
    // 成员序列化函数
    json to_json() const {
        return json{
            {"id", id_},
            {"price", price_}
        };
    }
    
    // 成员反序列化函数
    static Product from_json(const json& j) {
        return Product(
            j.at("id").get<std::string>(),
            j.at("price").get<double>()
        );
    }
};

int main() {
  // 创建连接
  redisContext *c = redisConnect("127.0.0.1", 6379);

  // key : string
  // val : string

  // redisCommand 支持c风格的printf语法
  std::string key = "name";
  std::string val = "Jack";
  redisReply *reply = static_cast<redisReply *>(
      redisCommand(c, "SET %s %s", key.c_str(), val.c_str()));
  std::cout << reply->str << std::endl;
  reply = static_cast<redisReply *>(redisCommand(c, "GET name"));
  std::cout << reply->str << std::endl;
  freeReplyObject(reply);
  // 对于已经存在的键值对会被set覆盖
  reply = static_cast<redisReply *>(redisCommand(c, "SET name Rose"));
  std::cout << reply->str << std::endl;
  freeReplyObject(reply);

  reply = static_cast<redisReply *>(redisCommand(c, "GET name"));
  std::cout << reply->str << std::endl;
  freeReplyObject(reply);

  // 不存在的时候设置键值对
  reply = static_cast<redisReply *>(redisCommand(c, "SET name Jack nx"));
  if(reply->type == REDIS_REPLY_NIL)
    std::cout<<"nil"<<std::endl;
  else
    std::cout << reply->str << std::endl;
  freeReplyObject(reply);

  // 多次设置 键值对
  reply = static_cast<redisReply *>(redisCommand(c, "MSET age 10 gender male work cooder"));
  std::cout << reply->str << std::endl;
  freeReplyObject(reply);

  // 多次获取 val (返回的是一个relpy数组 类型是REDIS_REPLY_ARRAY)
  reply = static_cast<redisReply *>(redisCommand(c, "MGET age  gender  work "));
  if(reply != nullptr && reply->type == REDIS_REPLY_ARRAY){
    std::cout<<"Reply : "<<std::endl;
    for(int i = 0;i<reply->elements;++i){
        if(reply->element[i]->type == REDIS_REPLY_STRING)
            std::cout<<reply->element[i]->str<<std::endl;
        else if(reply->element[i]->type == REDIS_REPLY_NIL)
            std::cout<<"nil"<<std::endl;
    }
  }else{
    std::cout<<"返回非数组"<<reply->str<<std::endl;
  }
  freeReplyObject(reply);


  // 可以对 key val(可以被解析成integer的string)原子的增加减少
  // 我们设置两个线程不使用锁的前提下依旧能够得到争取的数量结果

  // 设置一个可计算的变量
  reply = static_cast<redisReply*>(
    redisCommand(c,"SET counter 0")
  );
  std::cout<<reply->str<<std::endl;
  freeReplyObject(reply);

  std::thread t1([](){
    redisContext* c = redisConnect("127.0.0.1",6379);
    redisReply* reply;
    for(int i = 0;i<1000;++i){
        reply = static_cast<redisReply*>(
        redisCommand(c,"INCR counter")
        );
        std::cout<<reply->integer<<std::endl;
        freeReplyObject(reply);
    }
    
  });

  std::thread t2([](){
    redisContext* c = redisConnect("127.0.0.1",6379);
    redisReply* reply;
    for(int i = 0;i<1000;++i){
        reply = static_cast<redisReply*>(
        redisCommand(c,"INCRBY counter 2")
        );
        std::cout<<reply->integer<<std::endl;
        freeReplyObject(reply);
    }
  });

  t1.join();
  t2.join();

  // 测试 string存入json

  Product p1("0",12.06);
  json j(p1.to_json());

  reply = static_cast<redisReply*>(
    redisCommand(c,"SET products:product_1 %s",j.dump().c_str())
  );
  if(reply!= nullptr && reply->type == REDIS_REPLY_STRING)
    std::cout<<reply->str<<std::endl;
  else
   std::cout<<"error"<<std::endl;
  freeReplyObject(reply);

  reply = static_cast<redisReply*>(
    redisCommand(c,"GET products:product_1")
  );
  if(reply!= nullptr && reply->type == REDIS_REPLY_STRING)
    std::cout<<reply->str<<std::endl;
  else
   std::cout<<"error"<<std::endl;
  freeReplyObject(reply);
  
  return 0;
}