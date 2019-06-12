/* Logical function */
#ifndef LOGICFUNCTION_H
#define LOGICFUNCTION_H


// Represent a bitwise logic function that takes k(numinputs) bool value inputs, and outputs a single bool value result
// For example: and/or/xor are logic functions takes 2 inputs, 'not' is a logic function take 1 input
// Here inputs/output use char to represent the bool value: 't'(true),'f'(false),'x'(unknown)
// This is an abstract base class with the pure virtual function calculate() to be implemented.
class LogicFunction {
public:
	LogicFunction(const char *name, int numinputs);
	virtual ~LogicFunction();

	static LogicFunction *findFunction(const char *name);
	virtual char calculate(char *inputs) = 0;
	const char *name()const { return m_name; }
    int numinputs()const { return m_numinputs; }

protected:
	const int m_numinputs;
	const char *m_name;
};

// LogicFunctionByTable is a LogicFunction implemented by a truth table.
// The truth table defines its calculation rules, each line table[i] represent a rule with numinputs+1 chars(t/f/x):
//    rule_i_input = table[i][0~(numinputs-1)], rule_i_output = table[i][numinputs]
// for example:
//    LogicFunctionByTable f_and2("and2",2, {"txt", "xtt", "fff", 0})
//    table line "txt" means f(true, any_value) => true
//    table line "fff" means f(false, false) => false
// (!!!remind!!!: the last line of table should be 0 to indicate the end of table)
class LogicFunctionByTable: public LogicFunction{
public:
    LogicFunctionByTable(const char *name, int numinputs, const char **table);
    char calculate(char *inputs);

private:
    const char **m_table;
};

// SquareLogicFunction will take the inputs array with k^2 chars to represent a k*k bitwise square
class SquareLogicFunction: public LogicFunction {
public:
    SquareLogicFunction(const char *name, int square_size):
        m_square_size(square_size),LogicFunction(name, square_size*square_size){}

protected:
    const int m_square_size;
};

// Calculate if the bitwise square is horizontal symmetric
class HorizontalSymmetricSquareLogicFunction: public SquareLogicFunction {
public:
    HorizontalSymmetricSquareLogicFunction(const char *name, int square_size): SquareLogicFunction(name, square_size){}
    char calculate(char *inputs);
};

class VerticalSymmetricSquareLogicFunction: public SquareLogicFunction {
public:
    VerticalSymmetricSquareLogicFunction(const char *name, int square_size): SquareLogicFunction(name, square_size){}
    char calculate(char *inputs);
};

class RotateSymmetricSquareLogicFunction: public SquareLogicFunction {
public:
    RotateSymmetricSquareLogicFunction(const char *name, int square_size): SquareLogicFunction(name, square_size){}
    char calculate(char *inputs);
};


// LogicProcessor represent complex bitwise formulas(making LogicFunction composable):
//   LogicFunction takes only char(true/false) value
//   But LogicProcessor could take inputs that each one could either be raw char(t/f) or a LogicProcessor
// After setting up input sources(raw value or sub logicProcessor), every time calling process() would return the result(t/f/x) of current inputs' values
//
// For example, a bitwise formula of 3 inputs: !(inputs[0] && inputs[1]) || inputs[2]
//   (Given existing LogicFunction lf_and, lf_or, lf_not
//   char inputs[3];
//
//   LogicProcessor lp_and(&lf_and)
//   and.setInput(0, inputs+0)
//   and.setInput(1, inputs+1)
//
//   LogicProcessor lp_not_and(&lf_not)
//   not_and.setInput(0, &lp_and)
//
//   LogicProcessor lp_composed(&lf_or)
//   composed.setInput(0, &lp_not_and)
//   composed.setInput(1, inputs+2)
//
//   //...each time assign values to the inputs[3]
//   lp_composed.process()
class LogicProcessor {
public:
	LogicProcessor( LogicFunction *function );
	~LogicProcessor();

	void setInput(int input, LogicProcessor *lf);
	void setInput(int input, char * source);

	char process();

private:
    char **m_inputsources;
    LogicProcessor **m_inputfunctions;
    LogicFunction *m_logicfunction;
};

#endif // LOGICFUNCTION_H
