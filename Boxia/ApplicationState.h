#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

class ZD3DApp;

enum ApplicationStates
{
  Undefined = 0,

  Menu = 1,
  Game,
  Quit,
};

class ApplicationState
{
public:
  ApplicationState(ZD3DApp* application)
  {
    this->application = application;
  }

  virtual void Activate() = 0;
  virtual void Deactivate() = 0;
  virtual void Initialize() = 0;
  virtual void Update(double delta) = 0;
  virtual void Draw() = 0;
  inline virtual ApplicationStates GetNextState()
  {
    return this->nextState;
  }
protected:
  ZD3DApp* GetApplication() const { return application; }
  inline virtual void SetNextState(ApplicationStates nextState)
  {
    this->nextState = nextState;
  }
private:
  ApplicationStates nextState;
  ZD3DApp* application;
};

#endif