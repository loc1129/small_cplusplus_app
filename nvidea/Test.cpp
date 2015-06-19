//
//   NVIDIA Development Tools Team
//       C++ Programming Test
//
// ---------------------------------
// 
// This test should be self-explanatory.  All places where you are expected to
// write code are marked with TODO:  Each question is assigned a score.  Please
// see the SCORING comment below.  You are welcome to implement the questions
// in any order.
//
// This project should compile and run as-is, but all the tests will fail.
// Please implement all the TODOs as described below to get all the tests to
// succeed.
//
// You may use whatever other #includes you need.  You may use the C++ STL
// unless otherwise specified.  If you need help with STL (e.g. using fstream
// to read/write files), use the Josuttis book or read through the exercises
// and their tests, which have some file I/O already implemented.
//
// You're welcome to use stdio.h and C-style programming instead, but you'll
// have to convince us either way that your memory management is safe, even
// when ridiculous input values are passed to your functions.
//
// SCORING
// Total : 100 points
// 1. TestStringRotation                20 points
//   a. RotateString                       10 points
//   b. RotateStringOptimal                 5 points
//   c. Test vectors                        5 points
// 2. TestRgbConversions                20 points
//   a. CreateRGBA8888                      2 points
//   b. CreateRGB565                        3 points
//   c. ConvertRGBA8888ToRGB565             5 points
//   d. ConvertRGB565ToRGBA8888             5 points
//   e. Test vectors                        5 points
// 3. TestHierarchy                     45 points
//   a. Parser & data structure            15 points
//   b. Test vectors                       10 points
//   c. PrintHierarchyDepthFirst            5 points
//   d. PrintHierarchyDepthFirstSorted      5 points
//   e. PrintHierarchyBreadthFirst         10 points
// 4. TestClock                         15 points
//   a. AngleBetweenClockHands              3 points
//   b. GetMinuteHandFromHourHand           7 points
//   c. Test vectors                        5 points

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

// Append data filenames to this string to read/write files in the data folder.
const std::string data_location("../Data/");

#define CHECK(cond, errstream, oss) \
    if (!(cond)) \
    { \
        (oss) << errstream << std::endl; \
        return; \
    }

// -----------------------------------------------------------------------------
// Problem:
// RotateString rotates S by R positions.  If R is positive rotate to the right.
// If R is negative rotate to the left.  "Rotating" means that characters
// shifted off one end roll back around to the other end.
//
// Explain how much time and memory your solution takes based on R and the
// length of S.  Note:  Please start out with a simple algorithm, and only come
// back and optimize it if you have time left after doing the other exercises.
//

void RotateS(std::string & s, int from, int to)
{
	while(from < to)
	{
		char t = s[from];
		s[from++] = s[to];
		s[to--] = t;
	}
}

void rightone(std::string & s)
{
	int n = s.length();
	char t = s[n-1];
	for(int i = n - 1; i > 0; --i)
	{
		s[i] = s[i-1];	
	}
	s[0] = t;
}

void RotateString(std::string& S, int R)
{
    // TODO: Implement this function
	if(S.empty() || 
		R == 0 ||
		abs(R) == S.length())
	{
		return;
	}
	
	if(R < 0)
	{
		while ( R < 0)
		{
			R += S.length();	
		}
	}

	while(R--)
	{
		rightone(S);
	}
}

void RotateStringOptimal(std::string& S, int R)
{
    // TODO: Provide an optimal implementation in space and time.
	if(S.empty() || 
		R == 0 ||
		abs(R) == S.length())
	{
		return;
	}

	int len = S.length();
	int r = len - R;
	if(r > 0)
	{
		r = r % len;
	}
	else
	{
		while (r < 0)
		{
			r += len;
		}

		r = r % len;
	}
	
	RotateS(S, 0, r - 1);
	RotateS(S, r, len - 1);
	RotateS(S, 0, len -1);
}

void TestStringRotation(std::ostream& error_output)
{
    const std::string failure_msg("String rotation failed!");

    std::string s1("sprint");
    RotateString(s1, 5);
    CHECK(s1 == "prints", failure_msg, error_output);

    std::string s2("StringRotated");
    RotateString(s2, 7);
    CHECK(s2 == "RotatedString", failure_msg, error_output);

	std::string s3;
	RotateString(s3, 1);
	CHECK(s3 == "", failure_msg, error_output);

	std::string s4 = "test";
	RotateString(s4, 0);
	CHECK(s4 == "test", failure_msg, error_output);

	std::string s5 = "test";
	RotateString(s5, 4);
	CHECK(s5 == "test", failure_msg, error_output);

	std::string s6 = "test";
	RotateString(s6, -4);
	CHECK(s6 == "test", failure_msg, error_output);

	std::string s7 = "abcde";
	RotateString(s7, 6);
	CHECK(s7 == "eabcd", failure_msg, error_output);

	std::string s8 = "abcde";
	RotateString(s8, -1);
	CHECK(s8 == "bcdea", failure_msg, error_output);

    std::string s11("sprint");
    RotateStringOptimal(s11, 5);
    CHECK(s11 == "prints", failure_msg, error_output);

    std::string s21("StringRotated");
    RotateStringOptimal(s21, 7);
    CHECK(s21 == "RotatedString", failure_msg, error_output);

	std::string s31;
	RotateStringOptimal(s31, 1);
	CHECK(s31 == "", failure_msg, error_output);

	std::string s41 = "test";
	RotateStringOptimal(s41, 0);
	CHECK(s41 == "test", failure_msg, error_output);

	std::string s51 = "test";
	RotateStringOptimal(s51, 4);
	CHECK(s51 == "test", failure_msg, error_output);

	std::string s61 = "test";
	RotateStringOptimal(s61, -4);
	CHECK(s61 == "test", failure_msg, error_output);

	std::string s71 = "abcde";
	RotateStringOptimal(s71, 6);
	CHECK(s71 == "eabcd", failure_msg, error_output);

	std::string s81 = "abcde";
	RotateStringOptimal(s81, -1);
	CHECK(s81 == "bcdea", failure_msg, error_output);

    // TODO: Add more tests that demonstrate how you handle edge cases.
}


// -----------------------------------------------------------------------------
// Problem:
// In computer graphics color is often represented in the RGB color space using
// a packed 16-bit or 32-bit integer data type.
//
// This exercise converts between two of the common formats.
//
// RGBA8888 is a 32-bit pixel format which contains the Red, Green, and Blue
// color channel as well as an alpha which represents the pixels translucency.
//
//    [rrrrrrrr|gggggggg|bbbbbbbb|aaaaaaaa]  <-- Least significant bits
//
// RGB565 is a 16-bit pixel format which contains the Red, Green, and Blue
// color channel, but does not have an alpha channel.
//
//    [rrrrrggg|gggbbbbb] <-- Least significant bits
//

typedef unsigned char u8;
typedef unsigned int rgba8888;
typedef unsigned short rgb565;

// r, g, b, a have range 0-255
rgba8888 CreateRGBA8888( u8 r, u8 g, u8 b, u8 a )
{
    // TODO: Implement this function
	rgba8888 color = r << 24 | g << 16 | b << 8 | a;
    return color;
}

// r, g, b have range 0-255
rgb565 CreateRGB565( u8 r, u8 g, u8 b )
{
    // TODO: Implement this function
    return 0;
}

// Discard alpha component
rgb565 ConvertRGBA8888ToRGB565( rgba8888 src )
{
    // TODO: Implement this function
    return 0;
}

// Set alpha to FF (opaque)
rgba8888 ConvertRGB565ToRGBA8888( rgb565 src )
{
    // TODO: Implement this function
    // Add a comment detailing your algorithm decision
    // in terms of precision, round tripping, and performance.
    return 0;
}


// Extend the tests to identify and report any errors.
void TestRgbConversions(std::ostream& error_output)
{
    rgba8888 p32 = 0;
    rgb565 p16 = 0;

    p32 = CreateRGBA8888( 0xFF, 0x00, 0x00, 0x00 );
    p32 = CreateRGBA8888( 0x00, 0xFF, 0x00, 0x00 );
    p32 = CreateRGBA8888( 0x00, 0x00, 0xFF, 0x00 );
    p32 = CreateRGBA8888( 0x00, 0x00, 0x00, 0xFF );

    p16 = CreateRGB565( 0xFF, 0x00, 0x00 );
    p16 = CreateRGB565( 0x00, 0xFF, 0x00 );
    p16 = CreateRGB565( 0xFF, 0x00, 0xFF );

    p16 = ConvertRGBA8888ToRGB565( 0xFFFFFFFF );
    p16 = ConvertRGBA8888ToRGB565( 0x00000000 );
    p16 = ConvertRGBA8888ToRGB565( 0xFF000000 );
    p16 = ConvertRGBA8888ToRGB565( 0x00FF0000 );
    p16 = ConvertRGBA8888ToRGB565( 0x0000FF00 );
    p16 = ConvertRGBA8888ToRGB565( 0x000000FF );
    p16 = ConvertRGBA8888ToRGB565( 0x80808080 );

    p32 = ConvertRGB565ToRGBA8888( 0 );
    p32 = ConvertRGB565ToRGBA8888( CreateRGB565( 0xFF, 0x00, 0x00 ) );
    p32 = ConvertRGB565ToRGBA8888( CreateRGB565( 0x00, 0xFF, 0x00 ) );
    p32 = ConvertRGB565ToRGBA8888( CreateRGB565( 0x00, 0x00, 0xFF ) );
    p32 = ConvertRGB565ToRGBA8888( 0xFFFF );

    error_output << "RGB tests not yet implemented!" << std::endl;
}


// -----------------------------------------------------------------------------
// Problem:
// Create a simple data structure to represent a hierarchy.  Write a parser to
// read files in the following format and store them in your hierarchy structure.
// Then write functions that print the hierarhcy using multiple traversal
// algorithms.
// 
// Example input file format (note that leading periods specify depth):
//
// grandpa & grandma
// .mom
// ..me
// ...my son
// ...my daughter
// ..my brother
// ..my sister
// ...my nephew
// .aunt gertrude
// ..my annoying cousin
//
// Make sure that you detect nonsensical structures in the input file.  For
// example, the first line should not contain any leading periods, and a line
// with one leading period cannot be followed by a line with more than two
// leading periods.
//
// Because the printing functions are external to your class, you'll need to
// choose what type of interface to expose for those functions to be simple.
//
// The logic for the printing functions should NOT be contained within the class.

class Hierarchy
{
	struct tNode
	{
		std::string data;
		std::vector<std::shared_ptr<tNode> > children;
		std::weak_ptr<tNode> parent;
	};

	std::shared_ptr<tNode> root;

    // TODO: Implement this class

public:
	bool isValidRoot(const std::string & s)
	{
		bool findOtherCharacter = false;
		
		if(s.empty())
		{
			return false;
		}

		for(std::string::const_iterator it = s.begin(); it != s.end(); ++it)
		{
			if(*it == '.')
			{
				return !findOtherCharacter;
			}
			else
			{
				findOtherCharacter = true;
			}
		}

		return true;
	}

	std::shared_ptr<tNode> makeNode(const std::string & s, std::shared_ptr<tNode> parent)
	{
		std::shared_ptr<tNode> node(new tNode);
		if(!parent)
		{
			root = node;
			return node;
		}

		parent->children.push_back(node);
		node->parent = std::weak_ptr<tNode>(parent);
		return node;
	}

	int getLeadingPeriodNum(const std::string & s)
	{
		int num = 0; 
		for(std::string::const_iterator it = s.begin(); it != s.end(); ++it)
		{
			if(*it != '.')
			{
				break;
			}
			else
			{
				++num;
			}
		}
		return num;
	}

	std::shared_ptr<tNode> findParent(const std::string & s, std::shared_ptr<tNode> lastReadNode)
	{
		std::shared_ptr<tNode> emptyParent;
		int currentPeriodNum = getLeadingPeriodNum(s);		
		int periodNum = 0;

		std::shared_ptr<tNode> indexNode = lastReadNode;
		while(!indexNode)
		{
			periodNum = getLeadingPeriodNum(indexNode->data);
			int diff = periodNum - currentPeriodNum;	
			if(diff > 1)
			{
				return emptyParent;
			}
			else if(diff == 1)
			{
				return indexNode;
			}
			else
			{
				std::shared_ptr<tNode> parent = indexNode->parent.lock();
				if(!parent)
				{
					return emptyParent;
				}

				indexNode = parent;
			}
		}

		return emptyParent;
	}

    // Return false if any I/O errors occur, otherwise return true.
    // The input file should have only one root node.
    bool LoadFromFile(const std::string& infile)
    {
        // NOTE: Feel free to replace the entire implementation of
        // this function.
        std::ifstream ifs(infile.c_str());
        if (!ifs)
        {
            return false;
        }

        std::string line;
		bool createRoot = false;
		std::shared_ptr<tNode> lastReadNode;
        while (std::getline(ifs, line))
        {
			if(!createRoot)
			{
				if(!isValidRoot(line))
				{
					std::cout << "invalid first line" << std::endl;		
				}
				
				lastReadNode = makeNode(line, std::shared_ptr<tNode>());
			}
			else
			{
				std::shared_ptr<tNode> parent = findParent(line, lastReadNode);
				lastReadNode = makeNode(line, parent);
			}
            // TODO: Parse input line by line.  Handle bad input data elegantly,
            // or at least describe how in comments.
        }

        return true;
    }

};

// TODO: Implement the following Print* functions.
// - Print each node on a separate line, with two leading spaces for each depth
//   level.
// - The depth-first output should match the input order but the prefix should be
//   different.
// - The sorted depth-first output should sort the children of each node before
//   recursing into their children.
// - The breadth-first output should print all nodes at a given depth before
//   printing any deeper nodes.
// - See the files HierarchyTest_expected_*.txt in the
// Data directory for the expected results.
//
// Feel free to use helper functions, but don't change the signatures of these
// functions.

void PrintHierarchyDepthFirst(const Hierarchy& h, std::ostream& output)
{

}

void PrintHierarchyDepthFirstSorted(const Hierarchy& h, std::ostream& output)
{

}

void PrintHierarchyBreadthFirst(const Hierarchy& h, std::ostream& output)
{

}

// TODO: Improve this test method.
// How could you verify your outputs are correct?
// How could you refactor this function to be less repetitive?
void TestHierarchy(std::ostream& error_output)
{
    const std::string failure_msg("Hierarchy tests failed!");

    bool success = false;

    Hierarchy h;
    std::string infile(data_location + "HierarchyTest.txt");
    success = h.LoadFromFile(infile);
    CHECK(success, failure_msg << "  I/O error.", error_output);

    std::string outfile;
    std::string testfile;
    bool identical(false);

    outfile = data_location + "HierarchyTest_output_DFS.txt";
    {
        std::ofstream ofs(outfile.c_str());
        // If this fails, manually verify that outfile is not read-only on disk
        CHECK(ofs, failure_msg << "  I/O error.", error_output);

        PrintHierarchyDepthFirst(h, ofs);
    }

    outfile = data_location + "HierarchyTest_output_DFS_sorted.txt";
    {
        std::ofstream ofs(outfile.c_str());
        // If this fails, manually verify that outfile is not read-only on disk
        CHECK(ofs, failure_msg << "  I/O error.", error_output);

        PrintHierarchyDepthFirstSorted(h, ofs);
    }

    outfile = data_location + "HierarchyTest_output_BFS.txt";
    {
        std::ofstream ofs(outfile.c_str());
        // If this fails, manually verify that outfile is not read-only on disk
        CHECK(ofs, failure_msg << "  I/O error.", error_output);

        PrintHierarchyBreadthFirst(h, ofs);
    }
}

// -----------------------------------------------------------------------------
// Problem:
// Given a time of day (an integer for the hour, an integer for the minute),
// compute the angle between the clock hands in degrees.  The answer should
// always be between 0 and 180, inclusively.

float AngleBetweenClockHands(int hour, int minute)
{
    // TODO: Implement this function
    return 0.f;
}

// Given a pair of 2D points (C, the center of a clock, and H, the tip of the
// hour hand), you can figure out exactly what time it is.  We are assuming that
// at 12:00, the hour hand is pointing along the positive Y axis, and at 3:00,
// the hour hand is pointing along the positive X axis.  Compute the location of
// a third point M, the tip of the minute hand.  The minute hand is 20% longer
// than the hour hand.
struct float2
{
    float x;
    float y;
};
float2 GetMinuteHandFromHourHand(float2 C, float2 H)
{
    float2 M;

    // TODO: Implement this function
    M.x = 0.f;
    M.y = 0.f;

    return M;
}

void TestClock(std::ostream& error_output)
{
    // TODO: Add tests for AngleBetweenClockHands and GetMinuteHandFromHourHand.
    error_output << "Clock tests not yet implemented!" << std::endl;
}

// -----------------------------------------------------------------------------

int main()
{
    std::ostringstream oss;

    TestStringRotation(oss);
    TestRgbConversions(oss);
    TestHierarchy(oss);
    TestClock(oss);

    // Print output to console
    std::cout << oss.str();
    
    // Pause...
    std::cout << std::endl
        << std::endl
        << "Press enter to quit..." << std::endl;
    char c;
    std::cin >> std::noskipws >> c;

    return 0;
}
