#include "../include/class_redis.h"

int main(){
    My_redis redis;
    redis.command("SADD bikes:racing:france bike:1");
    redis.command("SADD bikes:racing:france bike:1");
    redis.command("SADD bikes:racing:france bike:2 bike:3");
    redis.command("SADD bikes:racing:usa bike:1 bike:4");

    redis.command("SISMEMBER bikes:racing:usa bike:1");
    redis.command("SISMEMBER bikes:racing:usa bike:2");
    
    redis.command("SINTER bikes:racing:usa bikes:racing:france");
    redis.command("SCARD bikes:racing:france");

    redis.command("SISMEMBER bikes:racing:france bike1");
    redis.command("SMISMEMBER bikes:racing:france bike:2 bike:3 bike:4");

    redis.command("SDIFF bikes:racing:france bikes:racing:usa");

    redis.command("SUNION bikes:racing:france bikes:racing:usa");
    
    //对于SMEMBER数据量大的时候他会返回所有的数据，导致性能大大受损，可以使用SSCAN代替
}
