#include "stdafx.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace patternstest
{
  [TestClass]
  public ref class UnitTest
  {
  private:
    TestContext^ testContextInstance;
  public:
    property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
    {
      Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
      {
        return testContextInstance;
      }
      System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
      {
        testContextInstance = value;
      }
    };

    [TestMethod]
    void TestMethod1()
    {
      Assert::AreEqual(524, 524);
    };

    [TestMethod]
    void TestMethod2()
    {
      Assert::AreEqual(524, 4);
    };
  };
}