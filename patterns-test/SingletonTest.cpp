#include "Dependencies.h"

#include "TestMacros.h"
#include "ThreadWorker.h"

#include "SingletonTestData.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace patterns_test
{
  void setTestValueToSingleton() {
    SingletonImpl::getInstance()->set(intTestValue);
  }

  [TestClass]
  public ref class SingletonTest
  {
    INIT_TEST_CONTEXT

    [TestCleanup()]
    void AfterEach() {
      SingletonImpl::getInstance()->set(intTestClearValue);
      SingletonImpl2::getInstance()->set(intTestClearValue);
    };

    [TestMethod]
    void GetInstanceWasCorrect()
    {
      int val1 = reinterpret_cast<int>(SingletonImpl::getInstance());
      int val2 = reinterpret_cast<int>(SingletonImpl::getInstance());
      
      Assert::AreEqual(val1, val2);
    };

    [TestMethod]
    void TwoSingletonImplementationTest() {
      int val1 = reinterpret_cast<int>(SingletonImpl::getInstance());
      int val2 = reinterpret_cast<int>(SingletonImpl2::getInstance());

      Assert::AreNotEqual(val1, val2);
    }

    [TestMethod]
    void CorrectWorkWithSingletonObjectFunctionality()
    {
      SingletonImpl::getInstance()->set(intTestValue);
      int value = SingletonImpl::getInstance()->get();

      Assert::AreEqual(intTestValue, value);
    };

    [TestMethod]
    void ThreadSynchronizeSingletonTest() {
      ThreadWorker::run(gcnew TestDelegate(setTestValueToSingleton));
      int value = SingletonImpl::getInstance()->get();

      Assert::AreEqual(value, intTestValue);
    }
  };
}