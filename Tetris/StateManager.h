#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

//#define NOMINMAX
//#include <windows.h>

#include <iostream>
#include <stack>
#include <memory>

#include "State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

    void Add(std::unique_ptr<State> toAdd, bool replace = false);
    void PopCurrent();
    void PopAll();
    void ProcessStateChange();
    std::unique_ptr<State>& GetCurrent();
    bool IsEmpty() const;

private:
    std::stack<std::unique_ptr<State>> m_stateStack;
    std::unique_ptr<State> m_newState;

    bool m_add;
    bool m_replace;
    bool m_remove;
    bool m_removeAll;

};

#endif // !__STATEMANAGER_H__

