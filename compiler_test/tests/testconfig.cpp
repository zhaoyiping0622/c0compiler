//
// Created by zhaoyiping on 2020/3/18.
//

#include "config.h"
#include "cstdlib"
#include "string"
#include "vector"
#include "gtest/gtest.h"

class ConfigDeathTest : public testing::Test {
 protected:
	std::vector<std::string> tmpfiles;
	virtual void SetUp() {
		for (int i = 0; i < 8; i++) {
			char s[] = {"tmp-XXXXXX"};
			mkstemp(s);
			tmpfiles.push_back(s);
		}
	}
	virtual void TearDown() {
		for (auto &x:tmpfiles) {
			unlink(x.c_str());
		}
	}
};
class ConfigTest : public testing::Test {
 protected:
	std::vector<std::string> tmpfiles;
	virtual void SetUp() {
		for (int i = 0; i < 8; i++) {
			char s[] = {"tmp-XXXXXX"};
			mkstemp(s);
			tmpfiles.push_back(s);
		}
	}
	virtual void TearDown() {
		for (auto &x:tmpfiles) {
			unlink(x.c_str());
		}
	}
};

/*
 * test file argument
 */

TEST_F(ConfigDeathTest, 0file) {
	const char *argv[] = {"compiler"};
	ASSERT_DEATH(Config(1, argv), "");
}

TEST_F(ConfigDeathTest, 1file) {
	const char *filename = tmpfiles[0].c_str();
	const char *argv[] = {"compiler", filename};
	Config config(2, argv);
	ASSERT_EQ(config.inputFileName, filename);
	ASSERT_EQ(config.debug, false);
}

TEST_F(ConfigDeathTest, manyfile) {
	const char *filename1 = tmpfiles[0].c_str();
	const char *filename2 = tmpfiles[1].c_str();
	const char *argv[] = {"compiler", filename1, filename2};
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
	const char *filename = tmpfiles[0].c_str();
	const char *argv[] = {"compiler", filename};
	Config config(2, argv);
	ASSERT_EQ(config.outputFileName, "a.asm");
}
TEST_F(ConfigTest, notDefaultOutputFile1) {
	const char *inputfilename = tmpfiles[0].c_str();
	const char *outputfilename = tmpfiles[1].c_str();
	const char *argv[] = {"compiler", inputfilename, "-o", outputfilename};
	Config config(4, argv);
	ASSERT_EQ(config.outputFileName, outputfilename);
}
TEST_F(ConfigTest, notDefaultOutputFile2) {
	const char *inputfilename = tmpfiles[0].c_str();
	const char *outputfilename = tmpfiles[1].c_str();
	const char *argv[] = {"compiler", inputfilename, "--output", outputfilename};
	Config config(4, argv);
	ASSERT_EQ(config.outputFileName, outputfilename);
}

/*
 * test debug mode
 */
TEST_F(ConfigTest, debugmode) {
	const char *filename = tmpfiles[0].c_str();
	const char *argv[] = {"compiler", filename, "--debug"};
	Config config(3, argv);
	ASSERT_EQ(config.debug, true);
}

/*
 * wrong argument
 */
TEST_F(ConfigDeathTest, wrongArgument) {
	const char *filename = tmpfiles[0].c_str();
	const char *argv[] = {"compiler", filename, "--asdf"};
	ASSERT_DEATH(Config(3, argv), "");
}

/*
 * help message
 */
TEST_F(ConfigDeathTest, helpMessage) {
	const char *filename = tmpfiles[0].c_str();
	const char *argv[] = {"compiler", filename, "--help"};
	testing::internal::CaptureStdout();
	ASSERT_EXIT(Config(3, argv), testing::ExitedWithCode(0), "");
	std::string helpMessage = testing::internal::GetCapturedStdout();
	ASSERT_TRUE(helpMessage.find("Usage:") != std::string::npos);
}

/*
 * output
 */
TEST_F(ConfigTest, output) {
	const char *filename1 = tmpfiles[0].c_str();
	const char *filename2 = tmpfiles[1].c_str();
	const char *argv[] = {"compiler", filename1, "-o", filename2};
	testing::internal::CaptureStderr();
	Config config(4,argv);
	config.output();
	std::string output=testing::internal::GetCapturedStderr();
	ASSERT_EQ(output,
			"inputFileName "+config.inputFileName+"\n"+
					"outputFileName "+config.outputFileName+"\n"+
					"debug "+std::to_string(config.debug) +"\n"
	);
}