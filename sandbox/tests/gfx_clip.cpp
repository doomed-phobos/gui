#include <catch2/catch_all.hpp>

#include "gfx/clip.hpp"
using namespace gfx;

TEST_CASE("ScaledClip") {
   Clip area;

   SECTION("Without Clip") {
      area = Clip(0, 0, 0, 0, 16, 16);
      REQUIRE(area(16, 16, 16, 16));
      REQUIRE(Clip(0, 0, 0, 0, 16, 16) == area);

      area = Clip(2, 2, 0, 0, 16, 16);
      REQUIRE(area(32, 32, 16, 16));
      REQUIRE(Clip(2, 2, 0, 0, 16, 16) == area);
   }

   SECTION("Fully Clipped") {
      area = Clip(32, 32, 0, 0, 16, 16);
      REQUIRE_FALSE(area(32, 32, 16, 16));

      area = Clip(-16, -16, 0, 0, 16, 16);
      REQUIRE_FALSE(area(32, 32, 16, 16));

      area = Clip(0, 0, 16, 16, 16, 16);
      REQUIRE_FALSE(area(32, 32, 16, 16));
   }

   SECTION("Without Zoom With Clip") {
      area = Clip(2, 3, 1, -1, 4, 3);
      REQUIRE(area(30, 29, 16, 16));
      REQUIRE(Clip(2, 4, 1, 0, 4, 2) == area);

      area = Clip(0, 0, -1, -4, 8, 5);
      REQUIRE(area(3, 32, 8, 8));
      REQUIRE(Clip(1, 4, 0, 0, 2, 1) == area);
   }

   SECTION("Zoom") {
      area = Clip(0, 0, 0, 0, 32, 32);
      REQUIRE(area(32, 32, 16, 16));
      REQUIRE(Clip(0, 0, 0, 0, 16, 16) == area);

      area = Clip(0, 0, 1, 2, 32, 32);
      REQUIRE(area(32, 32, 32, 32));
      REQUIRE(Clip(0, 0, 1, 2, 31, 30) == area);

      area = Clip(-1, 1, 1, -1, 4, 4);
      REQUIRE(area(6, 4, 9, 9));
      REQUIRE(Clip(0, 2, 2, 0, 3, 2) == area);
   }
}