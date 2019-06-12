/* Logical function */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logicfunction.h"

class LogicFunctionList {
private:

	struct LogicFunctionElm {
		LogicFunction *m_function;
		struct LogicFunctionElm *m_next;
	};

	static struct LogicFunctionElm *head;

public:
	static void insert(LogicFunction *f);
	static void remove(LogicFunction *f);
	static LogicFunction *find(const char *name);
};

LogicFunctionList::LogicFunctionElm *LogicFunctionList::head;

void LogicFunctionList::insert(LogicFunction *f)
{
	struct LogicFunctionElm *oldhead=head;
	head = new struct LogicFunctionElm;
	head->m_function = f;
	head->m_next = oldhead;
}


void LogicFunctionList::remove(LogicFunction *f)
{
    LogicFunctionElm **elm=&head;
    while(*elm){
        if ((*elm)->m_function == f)
        {
            LogicFunctionElm *next = (*elm)->m_next;
            delete (*elm);
            (*elm) = next;
            //return;       # fix: don't return directly, allow f occurs more than once in the list
        }
        else
        {
            elm=&((*elm)->m_next);
        }
    }
}

LogicFunction *LogicFunctionList::find(const char *name)
{
	for (LogicFunctionElm *elm=head; elm; elm=elm->m_next)
	{
		if (0 == strcmp(name, elm->m_function->m_name) )
		{
			return elm->m_function;
		}
	}
	return 0;
}


LogicFunction::LogicFunction(const char *name, int numinputs) :
        m_numinputs(numinputs)
{
    if (LogicFunction *lf = LogicFunctionList::find(name))
    {
        fprintf(stderr, "Warning: Duplicate definition of LogicFunction \"%s\"\n", name);
        delete lf;
    }
    m_name = strdup(name);
    LogicFunctionList::insert(this);
}

LogicFunctionByTable::LogicFunctionByTable(const char *name, int numinputs, const char **table) :
	m_table(table), LogicFunction(name, numinputs)
{
}

LogicFunction::~LogicFunction()
{
	LogicFunctionList::remove(this);
	free(m_name);
}

LogicFunction *LogicFunction::findFunction(const char *name)
{
	return LogicFunctionList::find(name);
}

char LogicFunctionByTable::calculate(char *inputs)
{
	for (const char **t=m_table; *t ; t++)
	{
		int i;
		for (i=0; (*t)[i] == 'x' || inputs[i] == (*t)[i] ; )
		{
			if (++i == m_numinputs )
				return (*t)[i];
		}
	}
	return 'x';
}

char HorizontalCubeLogicFunction::calculate(char *inputs) {
    for(int i=0; i<m_cubesize; i++){
        for(int j=0; j<m_cubesize-1-j; j++)
            if(inputs[i*m_cubesize + j] != inputs[i*m_cubesize + m_cubesize-1-j])
                return 'f';
    }
    return 't';
}

char VerticalCubeLogicFunction::calculate(char *inputs) {
    for(int j=0; j<m_cubesize; j++){
        for(int i=0; i<m_cubesize-1-i; i++)
            if(inputs[i*m_cubesize+j] != inputs[(m_cubesize-1-i)*m_cubesize+j])
                return 'f';
    }
    return 't';
}


char RotateCubeLogicFunction::calculate(char *inputs) {
    // (i, j) -> (j, n-1-i) -> (n-1-i, n-1-j) -> (n-1-j, i)
    for(int i=0; i<m_cubesize; i++)
        for(int j=0; j<m_cubesize; j++){
            int from = i*m_cubesize + j;
            int to = j*m_cubesize + m_cubesize-1-i;
            if(inputs[from] != inputs[to])
                return 'f';
        }
    return 't';
}

LogicProcessor::LogicProcessor( LogicFunction *function )
	: m_logicfunction ( function )
{
	m_inputsources = new char * [ function->m_numinputs ];
	m_inputfunctions = new LogicProcessor * [ function->m_numinputs ];
	for (int i=0; i<function->m_numinputs; i++)
	{
		m_inputsources[i] = 0;
		m_inputfunctions[i] = 0;
	}
}


LogicProcessor::~LogicProcessor()
{
	delete [] m_inputsources;
	delete [] m_inputfunctions;
}

void LogicProcessor::setInput(int input, LogicProcessor *lf)
{
	m_inputfunctions[input] = lf;
}

void LogicProcessor::setInput(int input, char * source)
{
	m_inputsources[input] = source;
}

char LogicProcessor::process()
{
	char *inputs = new char [ m_logicfunction->m_numinputs ];

	for (int i=0;i<m_logicfunction->m_numinputs;i++)
	{
		inputs[i] =  m_inputsources[i] ? *m_inputsources[i] :
			m_inputfunctions[i] ? m_inputfunctions[i]->process() : 'x';
	}
	char output=m_logicfunction->calculate(inputs);
	delete [] inputs;
	return output;
}

