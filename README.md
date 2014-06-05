#Support vector machine with no bias.
===================================

使用LBFGS求解支持向量机对偶问题，参考了Matlab Software by Mark Schmidt：
http://www.di.ens.fr/~mschmidt/Software/minConf.html

LBFGS用于求解大规模的优化问题，已经被广泛的应用在各种机器学习问题中。关于LBFGS的相关内容可以参见下面两个链接：

http://en.wikipedia.org/wiki/Broyden%E2%80%93Fletcher%E2%80%93Goldfarb%E2%80%93Shanno_algorithm
http://www.codelast.com/?p=2780

main.cpp是一个测试用例，数据集数量为250，选用其中150个作为训练集，其余的作为测试集。

关于这个程序我写了一个详细的文档，不久会发到网上~

编译程序：

	javier@javier-Lenovo:~/Workspace/libminconf_svm$ make
	Building obj/array.o...
	Building obj/minconf_tmp.o...
	Building obj/polyinterp.o...
	Building obj/svm.o...
	Building library...
	ar: creating ./lib/libminconf_tmp.a
	javier@javier-Lenovo:~/Workspace/libminconf_svm$ make svm
	Building train executable file ...
		
程序运行结果：
	
	javier@javier-Lenovo:~/Workspace/libminconf_svm$ ./bin/svm
	Reading data...

	 Iteration   FunEvals     Step Length    Function Val        Opt Cond
	 L-BFGS
			 1          2     6.66667e-03    -9.80344e-01     1.44103e+02
			 2          3     1.00000e+00    -1.73692e+01     1.70724e+02
			 3          4     1.00000e+00    -2.35502e+01     2.42023e+02
			 4          5     1.00000e+00    -3.09044e+01     1.42131e+02
			 5          6     1.00000e+00    -4.45824e+01     5.88137e+01
			 6          7     1.00000e+00    -4.80298e+01     3.81936e+01
			 7          8     1.00000e+00    -5.01108e+01     2.25573e+01
			 8          9     1.00000e+00    -5.17151e+01     1.12310e+01
			 9         10     1.00000e+00    -5.27262e+01     5.07007e+00
			10         11     1.00000e+00    -5.31558e+01     5.87705e+00
			11         12     1.00000e+00    -5.33095e+01     3.70394e+00
			12         13     1.00000e+00    -5.34772e+01     2.80490e+00
			13         14     1.00000e+00    -5.36690e+01     1.13255e+00
			14         15     1.00000e+00    -5.37062e+01     1.25654e+00
			15         16     1.00000e+00    -5.37273e+01     1.52874e+00
			16         17     1.00000e+00    -5.37440e+01     1.04342e+00
			17         18     1.00000e+00    -5.37573e+01     9.59199e-01
	Cubic Backtracking
			18         20     1.73880e-01    -5.37632e+01     7.25757e-01
			19         21     1.00000e+00    -5.37832e+01     6.12949e-01
			20         22     1.00000e+00    -5.38136e+01     7.61441e-01
			21         23     1.00000e+00    -5.38469e+01     1.08702e+00
			22         24     1.00000e+00    -5.38568e+01     2.50417e-01
			23         25     1.00000e+00    -5.38885e+01     4.04117e-01
	Cubic Backtracking
			24         27     1.23728e-01    -5.38940e+01     4.98169e-01
	Cubic Backtracking
			25         29     6.32498e-02    -5.39102e+01     2.00344e-01
	Cubic Backtracking
	Interpolated value too large, Adjusting
	Cubic Backtracking
	Interpolated value too large, Adjusting
	Cubic Backtracking
	Cubic Backtracking
	Cubic Backtracking
	Cubic Backtracking
			26         36     2.65240e-05    -5.39102e+01     2.00395e-01
	Cubic Backtracking
			27         38     1.14767e-01    -5.39160e+01     1.72371e-01
			28         39     1.00000e+00    -5.39226e+01     1.69142e-01
	Cubic Backtracking
			29         41     5.84268e-02    -5.39252e+01     1.63674e-01
	Cubic Backtracking
	Cubic Backtracking
			30         44     5.24389e-03    -5.39263e+01     1.47500e-01
	Cubic Backtracking
			31         46     3.22505e-02    -5.39272e+01     1.05750e-01
	Cubic Backtracking
	Cubic Backtracking
			32         49     1.23244e-02    -5.39280e+01     7.43283e-02
	Cubic Backtracking
			33         51     9.04024e-02    -5.39343e+01     3.80322e-02
	Cubic Backtracking
	Cubic Backtracking
	Cubic Backtracking
			34         55     5.70366e-03    -5.39346e+01     2.64456e-02
			35         56     1.00000e+00    -5.39352e+01     5.40733e-02
	Cubic Backtracking
			36         58     3.24925e-01    -5.39369e+01     3.56061e-02
			37         59     1.00000e+00    -5.39374e+01     1.15466e-02
			38         60     1.00000e+00    -5.39374e+01     9.66379e-03
			39         61     1.00000e+00    -5.39374e+01     9.28494e-03
			40         62     1.00000e+00    -5.39375e+01     1.64861e-02
			41         63     1.00000e+00    -5.39377e+01     2.97212e-02
			42         64     1.00000e+00    -5.39382e+01     5.28358e-02
	Cubic Backtracking
			43         66     2.41418e-01    -5.39385e+01     6.16050e-02
	Cubic Backtracking
			44         68     8.15000e-03    -5.39385e+01     6.23130e-02
	Cubic Backtracking
			45         70     1.99812e-02    -5.39388e+01     6.53351e-02
	Cubic Backtracking
			46         72     3.50423e-02    -5.39392e+01     7.06420e-02
	Cubic Backtracking
			47         74     1.61775e-01    -5.39401e+01     7.02310e-03
	Cubic Backtracking
			48         76     4.35884e-01    -5.39403e+01     6.15119e-03
			49         77     1.00000e+00    -5.39404e+01     1.05008e-03
			50         78     1.00000e+00    -5.39404e+01     2.88398e-17
	All working variables satisfy optimality condition.
	
	Train Error:0.06
	Generalized Error:0.06

