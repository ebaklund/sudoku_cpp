#include "catch.h"

#include "../src/utils/assert.h"

SCENARIO( "asserting truthy", "[ASSERT]" ) {
  GIVEN( "nothing" ) {
    WHEN( "when evaluating truthy" ) {
      THEN( "does not throw" ) {
        REQUIRE_NOTHROW( ASSERT(true) );
      }
    }
    WHEN( "asserting falsy" ) {
      THEN( "does throw" ) {
        REQUIRE_THROWS( ASSERT(false) );
      }
    }
  }
}
