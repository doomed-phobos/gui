#include <catch2/catch_all.hpp>

#include "base/shared_ptr.hpp"
#include "base/referenceable.hpp"

int constructors = 0;
int destructors = 0;
class A : public base::Referenceable 
{
public:
   A(int v) : v(v) {++constructors;}
   ~A() {++destructors;}

   int v = 0;
};

int b_destructors = 0;
class AVirt : public base::Referenceable
{
public:
   AVirt(int v) : v(v) {++constructors;}
   ~AVirt() {++destructors;}

   int v = 0;
};

class BVirt : public AVirt
{
public:
   BVirt(int v) : AVirt(v) {}
   ~BVirt() {++b_destructors;}
};

TEST_CASE("Stack")
{
   constructors = destructors = 0;
   {
      base::SharedPtr<A> pA(new A(2));
      REQUIRE(pA->v == 2);
   }
   REQUIRE(constructors == 1);
   REQUIRE(destructors == 1);
}

TEST_CASE("Assign Operator")
{
   constructors = destructors = 0;
   {
      auto pA = base::make_shared<A>(1);
      auto pB = base::make_shared<A>(2);
      auto pC = pA;
      REQUIRE(pC->v == 1);
      REQUIRE(pA.use_count() == 2);
      pC = pB;
      REQUIRE(pC->v == 2);
      REQUIRE(pB.use_count() == 2);
      pA.reset();
   }
   REQUIRE(constructors == 2);
   REQUIRE(destructors == 2);
}

TEST_CASE("Swap")
{
   constructors = destructors = 0;
   {
      auto pA = base::make_shared<A>(1);
      auto pB = base::make_shared<A>(2);
      std::swap(pA, pB);
      REQUIRE(pA->v == 2);
      REQUIRE(pB->v == 1);
   }
   REQUIRE(constructors == 2);
   REQUIRE(destructors == 2);
}

TEST_CASE("Dynamic Cast")
{
   constructors = destructors = 0;
   {
      auto pB = base::make_shared<BVirt>(1);        
      {
         auto pA = base::dynamic_pointer_cast<AVirt>(pB);
         REQUIRE(pA.use_count() == 2);
         REQUIRE(pA->v == 1);
         REQUIRE(constructors == 1);
      }
      REQUIRE(pB.use_count() == 1);
   }
   REQUIRE(destructors == 1);
}