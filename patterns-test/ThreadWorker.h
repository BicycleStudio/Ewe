using namespace System::Threading;

public delegate void TestDelegate();

public ref class ThreadWorker {
  TestDelegate^ _del;
public:
  void callback() {
    _del();
  }

  void set(TestDelegate^ del) {
    _del = del;
  }

  static void run(TestDelegate^ del) {
    ThreadWorker^ some = gcnew ThreadWorker();
    some->set(del);
    Thread^ t1 = gcnew Thread(gcnew ThreadStart(some, &ThreadWorker::callback));
    t1->Start(); t1->Join();
  }
};