#define INIT_TEST_CONTEXT \
private: \
  TestContext^ testContextInstance; \
public: \
  property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext \
  { \
    Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get() \
    { \
      return testContextInstance; \
    } \
    System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value) \
    { \
      testContextInstance = value; \
    } \
  };
