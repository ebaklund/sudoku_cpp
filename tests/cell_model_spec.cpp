#include "catch.h"

#include "../src/board/cell_model.h"
#include <iostream>

SCENARIO( "CellModel initialized", "[CellModel]" ) {
  GIVEN( "nothing" ) {
    WHEN( "CellModel created" ) {

      CellModel cell;

      THEN( "is valid" ) {
        REQUIRE( cell.size() == 9 );
        REQUIRE( cell.undecided() );
        REQUIRE( !cell.decided() );
        REQUIRE( cell.decision() == 0 );
        REQUIRE( !cell.defined() );
        REQUIRE( cell.definition() == 0 );
        for (uint32_t i = 1; i <= 9; ++i)
          REQUIRE( cell.possible(1) );
      }
    }
  }
}

SCENARIO( "CellModel defined", "[CellModel]" ) {

  GIVEN( "a new CellModel" ) {

    CellModel cell;

    WHEN( "defined with valid numbers" ) {

      uint32_t n = GENERATE(range(1, 10));
      cell.define(n);

      THEN( "colapses to single definition" ) {
        REQUIRE( cell.size() == 1 );
        REQUIRE( !cell.undecided() );
        REQUIRE( cell.decided() );
        for (uint32_t i = 1; i <= 9; ++i)
          REQUIRE( cell.possible(i) == (i == n) );
        REQUIRE( cell.decision() == n );
        REQUIRE( cell.defined() );
        REQUIRE( cell.definition() == n );
      }
    }
  }
}

SCENARIO( "CellModel invalidly defined", "[CellModel]" ) {

  GIVEN( "a new CellModel" ) {

    CellModel cell;

    WHEN( "defined with invalid numbers" ) {

      uint32_t n = GENERATE(0, 10);

      THEN( "throws exception" ) {
        REQUIRE_THROWS( cell.define(n) );
      }
    }
  }
}

SCENARIO( "CellModel partially reduced", "[CellModel]" ) {

  GIVEN( "a new CellModel" ) {

    CellModel cell;

    WHEN( "reduced with valid number" ) {

      uint32_t n = GENERATE(range(1, 10));
      cell.reduce(n);

      THEN( "reduced by one possibility" ) {
        REQUIRE( cell.size() == 8 );
        REQUIRE( cell.undecided() );
        REQUIRE( !cell.decided() );
        for (uint32_t i = 1; i <= 9; ++i)
          REQUIRE( cell.possible(i) == (i != n) );
        REQUIRE( cell.decision() == 0 );
        REQUIRE( !cell.defined() );
        REQUIRE( cell.definition() == 0 );
      }
    }
  }
}

SCENARIO( "CellModel fully reduced", "[CellModel]" ) {

  GIVEN( "a new CellModel" ) {

    CellModel cell;

    WHEN( "fully reduced with valid numbers" ) {

      uint32_t n = GENERATE(range(1, 10));
      for (uint32_t i = 1; i <= 9; ++i)
        if (i != n)
          cell.reduce(i);

      THEN( "single possibility left" ) {
        REQUIRE( cell.size() == 1 );
        REQUIRE( !cell.undecided() );
        REQUIRE( cell.decided() );
        for (uint32_t i = 1; i <= 9; ++i)
          REQUIRE( cell.possible(i) == (i == n) );
        REQUIRE( cell.decision() == n );
        REQUIRE( !cell.defined() );
        REQUIRE( cell.definition() == 0 );
      }
    }
  }
}

SCENARIO( "CellModel invalidly reduced", "[CellModel]" ) {

  GIVEN( "a new CellModel" ) {

    CellModel cell;

    WHEN( "reduced with invalid numbers" ) {

      uint32_t n = GENERATE(0, 10);

      THEN( "throws exception" ) {
        REQUIRE_THROWS( cell.reduce(n) );
      }
    }
  }
}
