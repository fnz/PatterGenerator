#include "gtest/gtest.h"

#include "Generator.h"
#include <map>
#include <iostream>

TEST(Generator, Triple) {
    EXPECT_EQ(Generator::checkTriple(""), true);
    EXPECT_EQ(Generator::checkTriple("K"), true);
    EXPECT_EQ(Generator::checkTriple("KK"), true);
    EXPECT_EQ(Generator::checkTriple("rlrK"), true);

    EXPECT_EQ(Generator::checkTriple("rrr"), false);
    EXPECT_EQ(Generator::checkTriple("Rrr"), false);
    EXPECT_EQ(Generator::checkTriple("RRR"), false);
    EXPECT_EQ(Generator::checkTriple("rKRR"), false);
    EXPECT_EQ(Generator::checkTriple("KKrK"), false);
    EXPECT_EQ(Generator::checkTriple("rrKlrrKKKR"), false);
    EXPECT_EQ(Generator::checkTriple("RRllllRR"), false);
    EXPECT_EQ(Generator::checkTriple("rlrlrlrlrlrlrr"), false);
}

TEST(Generator, DoubleAccent) {
    EXPECT_EQ(Generator::checkDoubleAccent(""), true);
    EXPECT_EQ(Generator::checkDoubleAccent("K"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("KK"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("KKK"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("r"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("rr"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("l"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("ll"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("rrL"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("rlrlrlrlrlrlr"), true);
    EXPECT_EQ(Generator::checkDoubleAccent("lKKKKKL"), true);

    EXPECT_EQ(Generator::checkDoubleAccent("rR"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("RR"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("LL"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("rLLr"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("KlLR"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("KKLl"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("rlrRlrlrlrlrlr"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("rlrlRRlrlrlrlr"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("rlrlLrlrlrlrlr"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("rlrlrlrLlrlrlr"), false);
    EXPECT_EQ(Generator::checkDoubleAccent("rlrlrlLRrRlrlrlr"), false);
}

TEST(Generator, Ends) {
    EXPECT_EQ(Generator::checkEnds(""), true);
    EXPECT_EQ(Generator::checkEnds("K"), true);
    EXPECT_EQ(Generator::checkEnds("KK"), true);
    EXPECT_EQ(Generator::checkEnds("KKK"), true);
    EXPECT_EQ(Generator::checkEnds("r"), true);
    EXPECT_EQ(Generator::checkEnds("rr"), true);
    EXPECT_EQ(Generator::checkEnds("rR"), true);
    EXPECT_EQ(Generator::checkEnds("RR"), true);
    EXPECT_EQ(Generator::checkEnds("LL"), true);
    EXPECT_EQ(Generator::checkEnds("RKRKRKRKRKL"), true);
    EXPECT_EQ(Generator::checkEnds("KRKRKRKRK"), true);

    EXPECT_EQ(Generator::checkEnds("rlrRlrlrlrlrlr"), false);
    EXPECT_EQ(Generator::checkEnds("rlllllllr"), false);
    EXPECT_EQ(Generator::checkEnds("llrlrlrLlrlrll"), false);
    EXPECT_EQ(Generator::checkEnds("lrlrlLRrRlrlrL"), false);
}

TEST(Generator, BackCount) {
    EXPECT_EQ(Generator::getBackCount("", ""), 0);
    EXPECT_EQ(Generator::getBackCount("RRLLKK", "RK"), 0);
    EXPECT_EQ(Generator::getBackCount("R", "R"), 1);
    EXPECT_EQ(Generator::getBackCount("RR", "R"), 2);
    EXPECT_EQ(Generator::getBackCount("RRRR", "R"), 4);
    EXPECT_EQ(Generator::getBackCount("KKLLRRR", "R"), 3);
    EXPECT_EQ(Generator::getBackCount("rkrlrkrrRrr", "r"), 2);
    EXPECT_EQ(Generator::getBackCount("KKKLKKKKK", "K"), 5);
}

TEST(Generator, WeightedRandom) {
    std::map<std::string, float> weights;
    weights["K"] = 0.0010;
    weights["L"] = 0.0007;
    weights["R"] = 0.0003;

    std::map<std::string, int> counts;
    for (int i = 0; i < 20000; i++) {
        counts[pick(weights)]++;
    }

    for (const auto& c : counts) {
        std::cout << c.first << " " << c.second << std::endl;
    }
}
