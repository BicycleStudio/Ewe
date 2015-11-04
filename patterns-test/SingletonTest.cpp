#include "Dependencies.h"

using namespace System;
using namespace System::Text;
using namespace System::Threading;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

#include <Singleton.h>

const int intTestValue = 42;

class SingletonImpl : public patterns::Singleton<SingletonImpl>{
private:
  SingletonImpl() {}
  SingletonImpl(const SingletonImpl&) {}
  friend class patterns::Singleton<SingletonImpl>;

private:
  int value;
public:
  void setValue(int v) {
    value = v;
  }
  int getValue() {
    return value;
  }
};

namespace patternstest
{
  [TestClass]
  public ref class SingletonTest
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
    void GetInstanceWasCorrect()
    {
      SingletonImpl* s1 = SingletonImpl::getInstance();
      SingletonImpl* s2 = SingletonImpl::getInstance();

      int val1 = reinterpret_cast<int>(s1);
      int val2 = reinterpret_cast<int>(s2);
      
      Assert::AreEqual(val1, val2);
    };

    [TestMethod]
    void CorrectWorkWithSingletonObjectFunctionality()
    {
      SingletonImpl* s1 = SingletonImpl::getInstance();
      s1->setValue(intTestValue);

      SingletonImpl* s2 = SingletonImpl::getInstance();
      int value = s2->getValue();

      Assert::AreEqual(intTestValue, value);
    };

    // method for test 'ThreadSynchronizeSingletonTest'
    void ForThreadTestMethod() {
      SingletonImpl* s1 = SingletonImpl::getInstance();
      s1->setValue(intTestValue);
    }

    [TestMethod]
    void ThreadSynchronizeSingletonTest() {
      SingletonTest^ some = gcnew SingletonTest();

      Thread^ t1 = gcnew Thread(gcnew ThreadStart(some, &SingletonTest::ForThreadTestMethod));
      t1->Start();

      Thread::Sleep(500);

      Assert::AreEqual(t1->ThreadState, ThreadState::Stopped);

      SingletonImpl* s1 = SingletonImpl::getInstance();
      Assert::AreEqual(s1->getValue(), intTestValue);
    }
  };
}