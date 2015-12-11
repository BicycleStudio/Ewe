#ifndef COMMAND_H_
#define COMMAND_H_

namespace patterns {

class Command {
public:
  virtual void execute() = 0;
};

template<class Subject>
class MethodExecuteCommand : public Command {
protected:
  using Action = void (Subject:: *) ();

  Subject* subject_;
  Action action_;
public:
  MethodExecuteCommand(Subject* s, Action a)
    : subject_(s) { action_ = a; }

  ~MethodExecuteCommand() {
    subject_ = nullptr;
  }

  void execute() {
    (this->subject_ ->* this->action_)();
  }
};

}

#endif // COMMAND_H_
