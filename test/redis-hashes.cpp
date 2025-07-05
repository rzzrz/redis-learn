#include "../include/class_redis.h"

int main() {
  My_redis redis;
  redis.command("HSET school:class:student name rxf age 1 gender male");
  redis.command("HGET school:class:student name");
  redis.command("HMGET school:class:student name age gender");

  redis.command("HINCRBY school:class:student age 10");
  redis.command("HINCRBY school:class:student age 10");

}