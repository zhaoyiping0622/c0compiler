//
// Created by zhaoyiping on 2020/3/18.
//

#include "test.h"
#include "config.h"
#include "cstdlib"
#include "string"
#include "vector"

#ifdef __MSC_VER
#define mktemp _mktemp
#endif  // __MSC_VER

class ConfigTest : public testing::Test {
 protected:
  std::vector<std::string> tmpfiles;
  std::string getTmp() {
    char s[] = {"tmp-XXXXXX"};
    tmpfiles.push_back(mktemp(s));
    auto f = std::ofstream(tmpfiles.back());
    f.close();
    return tmpfiles.back();
  }
  virtual void TearDown() {
    for (auto &x : tmpfiles) {
      unlink(x.c_str());
    }
  }
};

class ConfigDeathTest : public ConfigTest {};

/*
 * test file argument
 */

TEST_F(ConfigDeathTest, 0file) {
  const char *argv[] = {"compiler"};
  ASSERT_DEATH(Config(1, argv), "");
}

TEST_F(ConfigDeathTest, 1file) {
  std::string filename = getTmp();
  const char *argv[] = {"compiler", filename.c_str()};
  Config config(2, argv);
  ASSERT_EQ(config.inputFileName, filename);
  ASSERT_EQ(config.debug, false);
}

TEST_F(ConfigDeathTest, manyfile) {
  std::string filename1 = getTmp();
  std::string filename2 = getTmp();
  const char *argv[] = {"compiler", filename1.c_str(), filename2.c_str()};
  ASSERT_DEATH(Config(3, argv), "");
}

TEST_F(ConfigDeathTest, wrongfile) {
  const char *argv[] = {"compiler", "asdf"};
  ASSERT_DEATH(Config(2, argv), "file \"asdf\" not found");
}

/*
 * test output argument
 */
TEST_F(ConfigTest, defaultOutputFile) {
  std::string filename = getTmp();
  const char *argv[] = {"compiler", filename.c_str()};
  Config config(2, argv);
  ASSERT_EQ(config.outputFileName, "a.asm");
}
TEST_F(ConfigTest, notDefaultOutputFile1) {
  std::string inputfilename = getTmp();
  std::string outputfilename = getTmp();
  const char *argv[] = {"compiler", inputfilename.c_str(), "-o",
                        outputfilename.c_str()};
  Config config(4, argv);
  ASSERT_EQ(config.outputFileName, outputfilename);
}
TEST_F(ConfigTest, notDefaultOutputFile2) {
  std::string inputfilename = getTmp();
  std::string outputfilename = getTmp();
  const char *argv[] = {"compiler", inputfilename.c_str(), "--output",
                        outputfilename.c_str()};
  Config config(4, argv);
  ASSERT_EQ(config.outputFileName, outputfilename);
}

/*
 * test debug mode
 */
TEST_F(ConfigTest, debugmode) {
  std::string filename = getTmp();
  const char *argv[] = {"compiler", filename.c_str(), "--debug"};
  Config config(3, argv);
  ASSERT_EQ(config.debug, true);
}

/*
 * wrong argument
 */
TEST_F(ConfigDeathTest, wrongArgument) {
  std::string filename = getTmp();
  const char *argv[] = {"compiler", filename.c_str(), "--asdf"};
  ASSERT_DEATH(Config(3, argv), "");
}

/*
 * help message
 */
TEST_F(ConfigDeathTest, helpMessage) {
  std::string filename = getTmp();
  const char *argv[] = {"compiler", filename.c_str(), "--help"};
  testing::internal::CaptureStdout();
  ASSERT_EXIT(Config(3, argv), testing::ExitedWithCode(0), "");
  std::string helpMessage = testing::internal::GetCapturedStdout();
  std::cerr << helpMessage << std::endl;
  ASSERT_TRUE(helpMessage.find("Usage:") != std::string::npos);
}

/*
 * output
 */
TEST_F(ConfigTest, output) {
  std::string filename1 = getTmp();
  std::string filename2 = getTmp();
  const char *argv[] = {"compiler", filename1.c_str(), "-o", filename2.c_str()};
  testing::internal::CaptureStderr();
  Config config(4, argv);
  config.output();
  std::string output = testing::internal::GetCapturedStderr();
  ASSERT_EQ(output, "inputFileName " + config.inputFileName + "\n" +
                        "outputFileName " + config.outputFileName + "\n" +
                        "debug " + std::to_string(config.debug) + "\n");
}