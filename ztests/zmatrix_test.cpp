#include "zscript/zutils/zmatrix.h"
#include <string>
#include <array>
#include <catch2/catch_test_macros.hpp>

using namespace zst::zutils;

TEST_CASE("Constructors") {
    zmatrix A; // 1×1 с 0.0
    REQUIRE( A(0,0) == 0.0 );

    zmatrix B(3.5); // 1×1 с 3.5
    REQUIRE( B(0,0) == 3.5 );

    zmatrix C(std::vector<double>{1,2,3,4}, 2, 2);
    REQUIRE( C(0,0) == 1 );
    REQUIRE( C(0,1) == 2 );
    REQUIRE( C(1,0) == 3 );
    REQUIRE( C(1,1) == 4 );

    zmatrix D(2, 3);
    D(0,0) = 7;
    REQUIRE( D(0,0) == 7 );

    zmatrix V1(std::vector<double>{5,6,7}, 1, 3);
    REQUIRE( V1(0,0) == 5 );
    REQUIRE( V1(0,1) == 6 );
    REQUIRE( V1(0,2) == 7 );

    zmatrix V2(std::vector<double>{8,9,10}, 3, 1);
    REQUIRE( V2(0,0) == 8 );
    REQUIRE( V2(1,0) == 9 );
    REQUIRE( V2(2,0) == 10 );
}

TEST_CASE("any_of / all_of") {
    zmatrix X(std::vector<double>{0.5,1.2},1,2);
    REQUIRE( X.any_of()(0,0) == 1.0 );
    REQUIRE( X.all_of()(0,0) == 0.0 );

    zmatrix Y(std::vector<double>{1,1,1},3,1);
    REQUIRE( Y.all_of()(0,0) == 1.0 );
    REQUIRE( Y.any_of()(0,0) == 1.0 );

    REQUIRE( zmatrix(2.0).all_of()(0,0) == 1.0 );
    REQUIRE( zmatrix(0.0).any_of()(0,0) == 0.0 );
}

TEST_CASE("Comparisons") {
    zmatrix E(std::vector<double>{1,2},1,2);
    zmatrix F(std::vector<double>{1,3},1,2);
    auto eq = E == F;
    REQUIRE( eq(0,0) == 1.0 );
    REQUIRE( eq(0,1) == 0.0 );

    auto ne = E != F;
    REQUIRE( ne(0,0) == 0.0 );
    REQUIRE( ne(0,1) == 1.0 );

    auto lt = E < F;
    REQUIRE( lt(0,0) == 0.0 );
    REQUIRE( lt(0,1) == 1.0 );

    auto gt = E > F;
    REQUIRE( gt(0,0) == 0.0 );
    REQUIRE( gt(0,1) == 0.0 );

    auto le = E <= F;
    REQUIRE( le(0,0) == 1.0 );
    REQUIRE( le(0,1) == 1.0 );

    auto ge = E >= F;
    REQUIRE( ge(0,0) == 1.0 );
    REQUIRE( ge(0,1) == 0.0 );

    zmatrix C1(std::vector<double>{2,4,6},3,1);
    zmatrix C2(std::vector<double>{1,5,5},3,1);
    auto cmp = C1 >= C2;
    REQUIRE( cmp(0,0) == 1.0 );
    REQUIRE( cmp(1,0) == 0.0 );
    REQUIRE( cmp(2,0) == 1.0 );
}


TEST_CASE("Scalars") {
    zmatrix G(std::vector<double>{1, 2, 3}, 1, 3);
    auto sp = G + static_cast<zst::zutils::zmatrix>(2.0);
    REQUIRE(sp(0,0)==3);
    REQUIRE(sp(0,1)==4);
    REQUIRE(sp(0,2)==5);
    auto sm = G - 1.0;
    REQUIRE(sm(0,0)==0);
    REQUIRE(sm(0,1)==1);
    REQUIRE(sm(0,2)==2);
    auto st = G * 2.0;
    REQUIRE(st(0,0)==2);
    REQUIRE(st(0,1)==4);
    REQUIRE(st(0,2)==6);
    auto sd = G / 2.0;
    REQUIRE(sd(0,0)==0.5);
    REQUIRE(sd(0,1)==1);
    REQUIRE(sd(0,2)==1.5);
}

TEST_CASE("Elementwise") {
    zmatrix G(std::vector<double>{1,2,3},1,3);
    zmatrix H(std::vector<double>{3,2,1},1,3);

    auto sum = G + H;
    REQUIRE( sum(0,0) == 4 );
    REQUIRE( sum(0,1) == 4 );
    REQUIRE( sum(0,2) == 4 );

    auto diff = G - H;
    REQUIRE( diff(0,0) == -2 );
    REQUIRE( diff(0,1) == 0 );
    REQUIRE( diff(0,2) == 2 );

    auto prod = G * H;
    REQUIRE( prod(0,0) == 3 );
    REQUIRE( prod(0,1) == 4 );
    REQUIRE( prod(0,2) == 3 );

    auto divm = G / H;
    REQUIRE( divm(0,1) == 1 );
    REQUIRE( divm(0,2) == 3 );

    auto s_add = zmatrix(2.0) + 3.0;
    REQUIRE( s_add(0,0) == 5.0 );

    zmatrix V1(std::vector<double>{5,6,7},1,3);
    auto v_mul = V1 * 2.0;
    REQUIRE( v_mul(0,0) == 10 );
    REQUIRE( v_mul(0,1) == 12 );
    REQUIRE( v_mul(0,2) == 14 );
}

TEST_CASE("Matmul") {
    zmatrix M1(std::vector<double>{1,2,3,4},2,2);
    zmatrix M2(std::vector<double>{5,6,7,8},2,2);
    auto M3 = M1.dot(M2);
    REQUIRE( M3(0,0) == 19 );
    REQUIRE( M3(0,1) == 22 );
    REQUIRE( M3(1,0) == 43 );
    REQUIRE( M3(1,1) == 50 );

    zmatrix G(std::vector<double>{1,2,3},1,3);
    auto dot_v = G.dot(zmatrix(std::vector<double>{4,5,6},3,1));
    REQUIRE( dot_v.dimensions()[0] == 1 );
    REQUIRE( dot_v.dimensions()[1] == 1 );
    REQUIRE( dot_v(0,0) == 32);
}
