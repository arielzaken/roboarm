#pragma once
#include "../ICommand.h"


class LogDecorator : public ICommand{
    ICommand* decorable;
public:
    LogDecorator(ICommand* decorable);
    ~LogDecorator();
    ExecuteResult execute() override;
};