/* Logical function */
#ifndef LOGICFUNCTION_H
#define LOGICFUNCTION_H


class LogicFunction {
public:

    // a logic function take k(numinputs) inputs, and return a single bool value output
	// inputs/output are char: 't'(true),'f'(false),'x'(unknown)

	// @parameter numinputs:
	//   the size of input for this logic function
	LogicFunction(const char *name, int numinputs);
	~LogicFunction();

    // find the existing logicFunction registered as the target name:
	static LogicFunction *findFunction(const char *name);

    // calculate the logic function result of the numinputs inputs: 't'(true) or 'f'(false) or 'x'(uknown -- cannot be inferred from existing rules):
	virtual char calculate(char *inputs){ return 'x'; };

	int m_numinputs;
	char *m_name;
};

class LogicFunctionByTable: public LogicFunction{
public:
    // @parameter table:
    //   table defines its calculation rules, each line a rule: input = line[0:n-1], output = line[n-1]
    //   each line are a string of numinputs inputs + resulting output
    //   for example:
    //      LogicFunction f_and2("and2",2, {"txt", "xtt", "fff", 0})
    //      "txt" means f(true, any_value) = true
    //      "fff" means f(false, false) = false
    //   (!!!remind!!!: the last line of table should be 0 to indicate the end of table)
    LogicFunctionByTable(const char *name, int numinputs, const char **table);
    char calculate(char *inputs);
    const char **m_table;
};

class CubeLogicFunction: public LogicFunction {
public:
    CubeLogicFunction(const char *name, int cubeSize):
        m_cubesize(cubeSize),LogicFunction(name, cubeSize*cubeSize){}
    char calculate(char *inputs){ return 'x'; }
    int m_cubesize;
};

class HorizontalCubeLogicFunction: public CubeLogicFunction {
public:
    HorizontalCubeLogicFunction(const char *name, int cubeSize): CubeLogicFunction(name, cubeSize){}
    char calculate(char *inputs);
};

class VerticalCubeLogicFunction: public CubeLogicFunction {
public:
    VerticalCubeLogicFunction(const char *name, int cubeSize): CubeLogicFunction(name, cubeSize){}
    char calculate(char *inputs);
};

class RotateCubeLogicFunction: public CubeLogicFunction {
public:
    RotateCubeLogicFunction(const char *name, int cubeSize): CubeLogicFunction(name, cubeSize){}
    char calculate(char *inputs);
};

class LogicProcessor {
public:
	LogicProcessor( LogicFunction *function );
	~LogicProcessor();

    // each input could be a sub formula(LogicFunction) or a raw value(true/false):
	void setInput(int input, LogicProcessor *lf);
	void setInput(int input, char * source);

	char process();

	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	LogicFunction *m_logicfunction;
};

#endif // LOGICFUNCTION_H
