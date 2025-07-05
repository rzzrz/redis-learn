#include "../include/class_redis.h"
#include <chrono>
#include <thread>

int main(){
    // // 这个list是一个双向链表
    // My_redis redis;

    // // 使用命令模拟队列，实现商品出库的逻辑
    // redis.command("LPUSH product:outbound p1 p2 p3 p4");
    // for(int i = 0;i<4;i++)
    //     redis.command("RPOP product:outbound");

    // // 使用一侧push和pop模拟stack
    // redis.command("RPUSH stack s1 s2 s3 s4");
    // for(int i = 0;i<4;++i)
    //     redis.command("RPOP stack");

    // // 使用 LMOVE
    // // "LMOVE l1 l2 left right"
    // // left pop l1 ,return val and right push l2
    // redis.command("RPUSH l1 a b c");
    // redis.command("RPUSH l2 x y z");
    // redis.command("LMOVE l1 l2 left right");
    // redis.command("LRANGE l1 0 -1");
    // redis.command("LRANGE l2 0 -1");

    // 阻塞队列
    // 可以实现一个处理队列
    // 一个线程阻塞等待另一个线程生成任务
    std::thread t1([](){
        My_redis redis;
        redis.command("BLPOP operations 10");
    }
    );// 阻塞等待 不用redis也可以实现用co_wait

    std::thread  t2([](){
        My_redis redis;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        // wait 2 sec 
        redis.command("LPUSH operations op1");
    });
    t1.join();
    t2.join();
    //对于redis中的数据类型除了stream类型其他类型的val为空时自动销毁对应的key-val对
}