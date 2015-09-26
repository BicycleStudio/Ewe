#include <vector>
#include <queue>
#include <memory>
#include <thread>

namespace thread_manager {
  using std::vector;
  using std::queue;
  using std::shared_ptr;
  using std::thread;
  using std::for_each;

  class ThreadSubject;

  class ThreadCommand: public MethodExecuteCommand<ThreadSubject> {
  private:
    void (ThreadSubject:: *KillMethod) ();
    shared_ptr<KillMethod> remove_;
  public:
    ThreadCommand (shared_ptr<ThreadSubject> s, shared_ptr<Action> a, shared_ptr<KillMethod> k)
      : remove_(k) { this->MethodExecuteCommand(s, a); }
  };

  /**
    ������ �����: �������� ������� ��������� �������� ������� ������ � �������.
    ��������� � �������, ������ �� ������
    ����� �������� ������� ������������������, ����������������.
    �� ��� ���� ������ ��� ��� �� ����������� ��� ������.
    ���� ��������� ��� � ThreadSubject ����������� �������� �������������� �������, ��� �� ���� �����������.
    ������ ��� ����� ��������� ��� "���������", � �� �� ����� ����� �������� � ����������� ���������.
    ���� ��������, ��� ������ ����� �����������������.
    ����� ��������� ����������� ����� ���������
  */

  class ThreadManager {
  private:
    vector<ThreadCommand> threads;
  public:
    void start() {
      for_each(threads.begin, threads.end, [] (ThreadCommand tc) { tc.execute(); });
    };
    void stop();
  };
}