#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

#include "RegexTool.h"


struct RegexToolTestFixture
{
    std::vector<std::string> StrVectorURL;
    std::vector<std::string> StrVectorBadURL;
    const char* Finalpattern = "^(ht|f)tp(s?)://[[:alnum:]][[:graph:]]*$";
    
    RegexToolTestFixture()
    {
    }

    ~RegexToolTestFixture()
    {
    }

    void FormStrVectorURL(){
        StrVectorURL.push_back("http://foo.com/blah_blah");
        StrVectorURL.push_back("http://foo.com/blah_blah/");
        StrVectorURL.push_back("http://foo.com/blah_blah_(wikipedia)");
        StrVectorURL.push_back("http://foo.com/blah_blah_(wikipedia)_(again)");
        StrVectorURL.push_back("http://www.example.com/wpstyle/?p=364");
        StrVectorURL.push_back("https://www.example.com/foo/?bar=baz&amp;inga=42&amp;quux");
        StrVectorURL.push_back("http://userid:password@example.com:8080");
        StrVectorURL.push_back("http://userid:password@example.com:8080/");
        StrVectorURL.push_back("http://userid@example.com");
        StrVectorURL.push_back("http://userid@example.com/");
        StrVectorURL.push_back("http://userid@example.com:8080");
        StrVectorURL.push_back("http://userid@example.com:8080/");
        StrVectorURL.push_back("http://userid:password@example.com");
        StrVectorURL.push_back("http://userid:password@example.com/");
        StrVectorURL.push_back("http://142.42.1.1/");
        StrVectorURL.push_back("http://142.42.1.1:8080/");
        StrVectorURL.push_back("http://foo.com/blah_(wikipedia)#cite-1");
        StrVectorURL.push_back("http://foo.com/blah_(wikipedia)_blah#cite-1");
        StrVectorURL.push_back("http://foo.com/unicode_(?)_in_parens");
        StrVectorURL.push_back("http://foo.com/(something)?after=parens");
        StrVectorURL.push_back("http://code.google.com/events/#&amp;product=browser");
        StrVectorURL.push_back("http://j.mp");
        StrVectorURL.push_back("ftp://foo.bar/baz");
        StrVectorURL.push_back("http://foo.bar/?q=Test%20URL-encoded%20stuff");
        StrVectorURL.push_back("http://1337.net");
        StrVectorURL.push_back("http://a.b-c.de");
        StrVectorURL.push_back("http://a.b-c-d-e.de/de-a-b");
        StrVectorURL.push_back("http://223.255.255.254");
        StrVectorURL.push_back("https://foo_bar.example.com/");
    }

    void FormStrVectorBadURL(){
        StrVectorBadURL.push_back("http://");
        StrVectorBadURL.push_back("http://.");
        StrVectorBadURL.push_back("http://..");
        StrVectorBadURL.push_back("http://../");
        StrVectorBadURL.push_back("http://?");
        StrVectorBadURL.push_back("http://??");
        StrVectorBadURL.push_back("http://#");
        StrVectorBadURL.push_back("http://##");
        StrVectorBadURL.push_back("http://##/");
        StrVectorBadURL.push_back("http://foo.bar?q=Spaces should be encoded");
        StrVectorBadURL.push_back("//");
        StrVectorBadURL.push_back("//a");
        StrVectorBadURL.push_back("///a");
        StrVectorBadURL.push_back("///");
        StrVectorBadURL.push_back("http:///a");
        StrVectorBadURL.push_back("foo.com");
        StrVectorBadURL.push_back("rdar://1234");
        StrVectorBadURL.push_back("h://test");
        StrVectorBadURL.push_back("http:// shouldfail.com");
        StrVectorBadURL.push_back(":// should fail");
        StrVectorBadURL.push_back("http://foo.bar/foo(bar)baz quux");
        StrVectorBadURL.push_back("ftps://foo.bar/");
        StrVectorBadURL.push_back("http://-error-.invalid/");
        StrVectorBadURL.push_back("http://a.b--c.de/");
        StrVectorBadURL.push_back("http://-a.b.co");
        StrVectorBadURL.push_back("http://a.b-.co");
        StrVectorBadURL.push_back("http://.www.foo.bar/");
        StrVectorBadURL.push_back("http://www.foo.bar./");
        StrVectorBadURL.push_back("http://.www.foo.bar./");
    }
};

BOOST_FIXTURE_TEST_SUITE(RegexToolTest, RegexToolTestFixture)

BOOST_AUTO_TEST_CASE(DigitRegex)
{
    const char* digits = "123";
    const char* pattern = "[1-9]";

    BOOST_REQUIRE_EQUAL(RegexTool::Match(digits, pattern), true);
}

BOOST_AUTO_TEST_CASE(DigitRegex_NonAccepted)
{
    const char* digits = "987";
    const char* pattern = "[1-6]";

    BOOST_REQUIRE_EQUAL(RegexTool::Match(digits, pattern), false);
}

BOOST_AUTO_TEST_CASE(TextRegex)
{
    const char* text = "abc";
    const char* pattern = "[a-z]";

    BOOST_REQUIRE_EQUAL(RegexTool::Match(text, pattern), true);
}

BOOST_AUTO_TEST_CASE(TextRegex_NonAccepted)
{
    const char* text = "åöä";
    const char* pattern = "[a-z]";

    BOOST_REQUIRE_EQUAL(RegexTool::Match(text, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_StandardProtocol)
{
    const char* pattern = "^(ht|f)tp(s?)://";

    const char* correctUrlHTTP = "http://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftp://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coap://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_SecureProtocol)
{
    const char* pattern = "^(ht|f)tp(s?)://";

    const char* correctUrlHTTP = "https://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftps://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coaps://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_StandardWithoutwww)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]";

    const char* correctUrlHTTP = "http://example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftp://example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coap://example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_SecuredWithoutwww)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]";

    const char* correctUrlHTTP = "https://example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftps://example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coaps://example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_StandardOptionalwww)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])";

    const char* correctUrlHTTP = "http://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftp://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coap://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_SecureOptionalwww)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])";

    const char* correctUrlHTTP = "https://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftps://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coaps://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_StandardPortNumbers)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*(:(0-9)*)";

    const char* correctUrlHTTP = "http://142.42.1.1:8080/";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftp://142.42.1.1:8080/";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coap://142.42.1.1:8080/";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_SecurePortNumbers)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*(:(0-9)*)";

    const char* correctUrlHTTP = "https://142.42.1.1:8080/";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftps://142.42.1.1:8080/";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coaps://142.42.1.1:8080/";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_StandardEndOfHostname)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*(:(0-9)*)*(\\/?)([a-zA-Z0-9\\-\\.\\?\\,\\'/\\\\\\+&%\\$#_]*)";

    const char* correctUrlHTTP = "http://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftp://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coap://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}

BOOST_AUTO_TEST_CASE(UrlRegex_SecureEndOfHostname)
{
    const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*(:(0-9)*)*(\\/?)([a-zA-Z0-9\\-\\.\\?\\,\\'/\\\\\\+&%\\$#_]*)";

    const char* correctUrlHTTP = "https://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlHTTP, pattern), true);

    const char* correctUrlFTP = "ftps://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(correctUrlFTP, pattern), true);

    const char* badUrlCOAP = "coaps://www.example.com/foo/?bar=baz&inga=42&quux";
    BOOST_REQUIRE_EQUAL(RegexTool::Match(badUrlCOAP, pattern), false);
}


BOOST_AUTO_TEST_CASE(UrlRegex_GoodUrlVectorTest)
{
    FormStrVectorURL();
    //const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*(:(0-9)*)*(\\/?)([a-zA-Z0-9\\-\\.\\?\\,\\'/\\\\+&%\\$#_]*)";

    for(auto &url : StrVectorURL)
    {
        const char* testUrl = &url[0];
        BOOST_CHECK_EQUAL(RegexTool::Match(testUrl, Finalpattern), true);
        //If check fails, print the current testUrl
        BOOST_TEST_INFO(testUrl);
    }
}

BOOST_AUTO_TEST_CASE(UrlRegex_BadUrlVectorTest)
{
    FormStrVectorBadURL();
    //const char* pattern = "^(ht|f)tp(s?)://[0-9a-zA-Z]([-.\\w]*[0-9a-zA-Z])*(:(0-9)*)*(\\/?)([a-zA-Z0-9\\-\\.\\?\\,\\'/\\\\+&%\\$#_]*)";

    for(auto &url : StrVectorBadURL)
    {
        const char* testUrl = &url[0];
        BOOST_CHECK_EQUAL(RegexTool::Match(testUrl, Finalpattern), false);
        //If check fails, print the current testUrl
        BOOST_TEST_INFO(testUrl);
    }
}

BOOST_AUTO_TEST_SUITE_END()
