/*
 * Test software
 */

#include <stdio.h>

#include "logicfunction.h"
#include "testcases.h"

const char *or2_table [] =
{
"txt",
"xtt",
"fff",
0
};

const char *or3_table [] =
{
"txxt",
"xtxt",
"xxtt",
"ffff",
0
};

const char *and2_table [] =
{
"ttt",
"fxf",
"xff",
0
};

const char *and3_table [] =
{
"tttt",
"fxxf",
"xfxf",
"xxff",
0
};

const char *xor2_table  [] =
{
"tft",
"ftt",
"fff",
"ttf",
0
};

const char *xor3_table  [] =
{
"fftt",
"ftft",
"tfft",
"tttt",
"ffff",
"fttf",
"ttff",
"tftf",
0
};

const char *impl_table [] =
{
"xtt",
"fxt",
"tff",
0
};

const char *not_table [] =
{
"tf",
"ft",
0
};

const char *incl_table [] =
{
"txxt",
0
};

void processor_test(LogicProcessor *proc, int n, char *inp)
{
	int i;
	for (i=0;i<n;i++)
	{
		inp[i] = 'f';
	}

	bool done=false;
	do {
		for (i=0 ; i<n; i++)
			printf("%c ", inp[i]);
		printf(" -> %c\n", proc->process());

		for (i=0 ; i<n; i++)
		{
            // iterate all permutations by +1 each round: ttftf -> ffttf: "11010"(as reversed number 01011) => "00110"(01100)
			if (inp[i] == 'f')
			{
				inp[i] = 't';
				break;
			}
			else
			{
				inp[i] = 'f';
			}
		}
		done = i==n;
	} while (!done);

}


void function_test ( LogicFunction *func )
{
	char *inp;
	char n=func->m_numinputs;
	LogicProcessor proc(func);

	printf("Testing function: %s\n", func->m_name);
	inp = new char [n];
	for (int i=0; i<n; i++)
	{
		proc.setInput(i, inp+i);
	}

	processor_test(&proc, n, inp);

	delete [] inp;
}



int main()
{
	LogicFunctionByTable
		f_not("not",1,not_table),
		f_and2("and2",2,and2_table),
		f_and3("and3",3,and3_table),
		f_or2("or2",2,or2_table),
		f_or3("or3",3,or3_table),
		f_xor2("xor2",2,xor2_table),
		f_xor3("xor3",3,xor3_table),
		f_implies("implies", 2, impl_table);

    LogicFunctionByTable
		f_incomplete("incomplete",3, incl_table);

// Basic table tests
	function_test(&f_not);
	function_test(&f_and2);
	function_test(&f_and3);
	function_test(&f_or2);
	function_test(&f_or3);
	function_test(&f_xor2);
	function_test(&f_xor3);
	function_test(&f_implies);

	function_test(&f_incomplete);


// Combinatorial tests

	{
		printf("Testing combinatorial not (P and Q)\n");
		char inputs[2];
		LogicProcessor p_not(&f_not),  p_and(&f_and2);
		p_and.setInput(0,inputs);
		p_and.setInput(1,inputs + 1);
		p_not.setInput(0,&p_and);

		processor_test(&p_not, 2, inputs);
	}

	{
		printf("Testing combinatorial P and not (Q or not R)\n");
		//  A && !(B || !C)
		char inputs[3];
		LogicProcessor p_not0(&f_not), p_not1(&f_not), p_or(&f_or2), p_and(&f_and2);
		p_not0.setInput(0,inputs+2);
		p_or.setInput(0,inputs+1);
		p_or.setInput(1,&p_not0);
		p_not1.setInput(0,&p_or);
		p_and.setInput(0,inputs);
		p_and.setInput(1, &p_not1);

		processor_test(&p_and, 3, inputs);
	}

// cube tests:
    int cube_size = 8;
    HorizontalCubeLogicFunction horizontal("horizontal_symmetric", cube_size);
    VerticalCubeLogicFunction vertical("vertical_symmetric", cube_size);
    RotateCubeLogicFunction rotate("rotate_symmetric", cube_size);
    LogicProcessor p_hori(&horizontal), p_vert(&vertical), p_rotate(&rotate);

    char cube[cube_size*cube_size];
    for(int j=0; j<cube_size*cube_size; j++){
        p_hori.setInput(j, cube+j);
        p_vert.setInput(j, cube+j);
        p_rotate.setInput(j, cube+j);
    }

    for(int i=0; i < sizeof(testcases)/ sizeof(testcases[0]); i++){
        printf("test case %i:\n", i);
        for(int row=0; row<cube_size; row++) {
            for (int col = 0; col < cube_size; col++) {
                int j = row*cube_size+col;
                cube[j] = testcases[i][j];
                printf("%c", cube[j]);
            }
            printf("\n");
        }
        printf("--> horizontal: %c, vertical: %c, ratate: %c\n", p_hori.process(), p_vert.process(), p_rotate.process());
        printf("---\n");
    }

}
