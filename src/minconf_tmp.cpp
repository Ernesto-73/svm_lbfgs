/*
	\file: minconf_tmp.cpp
	\author: Javier Zhang
	\date: 3-20-2014
*/

#include "minconf_tmp.h"

double minConf_TMP::funObj(Array &x, Array &g)
{
	double *_g = g.getData();
	const double *_x = x.getData();
	assert(_g && _x);
	return (*func)(nInstance, _x, input, _g);
}

void minConf_TMP::setFunObj(FuncObj _ff)
{
	func = _ff;
}

minConf_TMP::minConf_TMP(Array _x, Array _LB, Array _UB)
{
	x = _x;
	LB = _LB;
	UB = _UB;
	nVars = x.size;
	opt = new Options;
}

minConf_TMP::minConf_TMP(Array _x, Array _LB, Array _UB, Options *_opt)
{
	x = _x;
	LB = _LB;
	UB =  _UB;
	nVars = x.size;
	opt = _opt;
}

minConf_TMP::~minConf_TMP()
{

}

void minConf_TMP::projectBounds(Array *_x)
{
	double *data;
	int len = 0;
	
	if(!_x)
	{
		data = x.getData();
		len = x.size;
	}
	else {
		data = _x->getData();
		len = _x->size;
	}
	
	double *_LB = LB.getData();
	double *_UB = UB.getData();
	
	for(int i = 0;i < len;++i)
	{
		if(data[i] < _LB[i])
			data[i] = _LB[i];
		
		if(data[i] > _UB[i])
			data[i] = _UB[i];
	}
}

void minConf_TMP::computeWorkingSet()
{
	working.clear();
	double *_x = x.getData();
	double *_LB = LB.getData();
	double *_UB = UB.getData();
	double *_g = g.getData();
	
	for(int i = 0;i < x.size;i++)
	{
		if(_x[i] < _LB[i] + opt->optTol * 2 && _g[i] >=0)
			continue;
		
		if(_x[i] > _UB[i] - opt->optTol * 2 && _g[i] <=0)
			continue;

		working.push_back(i);
	}
}

void minConf_TMP::lbfgsUpdate(const Array &y, const Array &s, int corrections, std::deque<Array> &old_dirs, std::deque<Array> &old_stps, double &Hdiag)
{
	double ys = arrayMul(y,s);
	
	if(ys > 1e-10)
	{
		int numCorrections = old_dirs.size();
		if(numCorrections < corrections)
		{
			// Full Update
			old_dirs.push_back(s);
			old_stps.push_back(y);
		}
		else {
			// Limited-Memory Update
			old_dirs[0].release();
			old_dirs.pop_front();
			old_stps[0].release();
			old_stps.pop_front();
			
			old_dirs.push_back(s);
			old_stps.push_back(y);
		}

		// Update scale of initial Hessian approximation
		Hdiag = ys / arrayMul(y, y);
	}
}

void minConf_TMP::lbfgs(const Array &g, const std::deque<Array> &s_set, const std::deque<Array> &y_set, const double Hdiag, const std::vector<int> &cols, const std::vector<int> &rows, Array &d)
{
	int p = rows.size();
	int k = cols.size();
	
	double *ro = new double[k];
	for(int i = 0;i < k;i++)
	{
		const double *_y = y_set[cols[i]].getData();
		const double *_s = s_set[cols[i]].getData();
		double sum = 0;
		for(int j = 0;j < p;j++)
			sum += _y[rows[j]] * _s[rows[j]];
		
		ro[i] = 1 / sum;
	}
	
	double *q = new double[p * (k+1)];
	double *r = new double[p * (k+1)];
	double *al = new double[k];
	double *be = new double[k];
	
	double *_g = g.getData();
	int step = k+1;
	for(int i = 0;i < p;i++)
		q[i * step + k] = -_g[rows[i]];
		
	for(int i = k-1;i >=0;i--)
	{
		const double *_s = s_set[cols[i]].getData();
		const double *_y = y_set[cols[i]].getData();
		
		double ans = 0;
		for(int j = 0;j < p;j++)
			ans += _s[rows[j]] * q[j * step + i+1];
		al[i] = ro[i] * ans;
		
		for(int j = 0;j < p;j++)
			q[j * step + i] = q[j * step + i+1] - al[i] * _y[rows[j]];
	}
	
	// Multiply by Initial Hessian
	for(int j = 0;j < p;j++)
		r[j * step] = Hdiag * q[j * step];
	
	for(int i = 0;i < k;i++)
	{
		const double *_s = s_set[cols[i]].getData();
		const double *_y = y_set[cols[i]].getData();
		
		double ans = 0;
		for(int j = 0;j < p;j++)
			ans += _y[rows[j]] * r[j * step + i];
		be[i] = ro[i] * ans; 
		
		for(int j = 0;j < p;j++)
			r[j * step + i+1] = r[j * step + i] + _s[rows[j]] * (al[i] - be[i]);
	}
	
	double *_d = d.getData();
	for(int j = 0;j < p;j++)
		_d[rows[j]] = r[step * j + k];
	       
	delete [] ro;
	ro = NULL;
	
	delete [] q;
	q = NULL;
	
	delete [] r;
	r = NULL;
	
	delete [] al;
	al = NULL;
	
	delete [] be;
	be = NULL;
}

void minConf_TMP::loadData(int _nInstance, int _dim, double *_input, double *_output)
{
	nInstance = _nInstance;
	dim = _dim;
	input = _input;
	output = _output;
}

void minConf_TMP::process()
{
	g.create(nVars);
	funEvals = 0;
	
	// Output Log
	if(opt->verbose >= 3)
		printf("%10s %10s %15s %15s %15s\n","Iteration","FunEvals","Step Length","Function Val","Opt Cond");	
	funEvalMultiplier = 1;

	// Evaluate Initial Point
	projectBounds();
	f = funObj(x, g);
	funEvals++;
	
	// Compute Working Set
	computeWorkingSet();
	
	// Check Optimality
	if(working.empty())
	{
		if(opt->verbose >= 1)
			std::cout<<"All variables are at their bound and no further progress is possible at initial point."<<std::endl;
		return ;
	}
	else {
		if(norm_L2(g, working) <= opt->optTol)
		{
			if (opt->verbose >=1)
				std::cout<<"All working variables satisfy optimality condition at initial point."<<std::endl;
			return ;
		}
	}
	
	if(opt->verbose >= 3)
	{ 
		switch(opt->method)
		{
			case SD:
				std::cout<<" Steepest Descent"<<std::endl;
				break;
			case LBFGS:
				std::cout<<" L-BFGS"<<std::endl;
				break;
		}
	}	
	
	int it = 0;

	// Used for LBFGS
	std::deque<Array> old_dirs;
	std::deque<Array> old_stps;
		
	// Used for BFGS
	Array B;
	
	double Hdiag;
	Array g_old(nVars);
	Array x_old(nVars);
	double f_old;
	
	Array x_new(nVars);
	Array g_new(nVars);
	double f_new;
	
	Array d(nVars);
	
	while(funEvals <= opt->maxIter)
	{
		// Compute Step Direction
		d.setZeros();
		
		switch(opt->method)
		{
			case SD:
			{
				double *_d = d.getData();
				double *_g = g.getData();
				for(std::vector<int>::iterator iter = working.begin();iter != working.end();++iter)
					_d[*iter] = -_g[*iter];
				break;
			}
			
			case LBFGS:
			{
				double *_d = d.getData();
				double *_g = g.getData();
				if(it == 0)
				{
					for(std::vector<int>::iterator iter = working.begin();iter != working.end();++iter)
						_d[*iter] = -_g[*iter];
					Hdiag = 1;
				}
				else{
					if(opt->damped)
					{
						//TODO:
					/*
						[old_dirs,old_stps,Hdiag] = dampedUpdate(g-g_old,x-x_old,corrections,verbose==3,old_dirs,old_stps,Hdiag);
					*/
						exit(0);
					}
					else{
						Array y = arraySub(g, g_old);
						Array s = arraySub(x, x_old);
						lbfgsUpdate(y, s, opt->corrections, old_dirs, old_stps, Hdiag);
					}
					
					int col_num = old_dirs.size();
					std::vector<int> curvSat;
					for(int i = 0;i < col_num;++i)
					{
						double *p = old_dirs[i].getData();
						double *q = old_stps[i].getData();
						double sum = 0;
						for(std::vector<int>::iterator iter = working.begin();iter != working.end();++iter)
							sum += p[*iter] * q[*iter];
							
						if(sum > 1e-10)
							curvSat.push_back(i);
					}
					lbfgs(g, old_dirs, old_stps, Hdiag, curvSat, working, d);
				}
				g.copyTo(g_old);
				x.copyTo(x_old);
				break;
			}	 
			default:
			{
				std::cout<<"Unknown method."<<std::endl;
				return ;
			}
		}
		
		// Check that Progress can be made along the direction
		f_old = f;
		double gtd = arrayMul(g, d);
		if(gtd > -opt->optTol)
		{
			if(opt->verbose >= 2)
				std::cout<<"Directional Derivative below optTol."<<std::endl;
			break;
		}

		// Select Initial Guess to step length
		double t;
		if(it == 0)
		{
			double sum = 0;
			double *_g = g.getData();
			for(std::vector<int>::iterator iter = working.begin();iter != working.end();++iter)
				 sum +=  abs(_g[*iter]);
			t = min(1, 1 / sum);
		}
		else {
			t = 1.;
		}
		
		// Evaluate the Objective and Projected Gradient at the Initial Step	
		double *_x_new = x_new.getData();
		double *_x = x.getData();
		double *_d = d.getData();
		
		for(int i = 0;i < x.size;i++)
			_x_new[i] = _x[i] + _d[i] * t;
		projectBounds(&x_new);
		
		f_new = funObj(x_new, g_new);
		funEvals++;
		
		// Backtracking Line Search
		int lineSearchIters = 1;

		while(1)
		{
			Array sub = arraySub(x_new, x);
			double delta = opt->suffDec * arrayMul(g, sub);
			sub.release();
			if(f_new <= f + delta)
				break;
			
			double temp = t;
			if(opt->interp == 0)
			{
				if(opt->verbose == 3)
					std::cout<<"Halving Step Size"<<std::endl;
				t = .5*t;
			}
			else {
				if(opt->verbose == 3)
					std::cout<<"Cubic Backtracking"<<std::endl;
				t = polyInterp(0., f, gtd,t, f_new, arrayMul(g_new, d));
			}
			
			// Adjust if change is too small or too large
			if( t < temp * 1e-3)
			{
				if(opt->verbose == 3)
					std::cout<<"Interpolated value too small, Adjusting"<<std::endl;
				t = temp * 1e-3;
			}
			else {
				if( t > temp * 0.6 )
				{
					if(opt->verbose == 3)
						std::cout<<"Interpolated value too large, Adjusting"<<std::endl;
					t = temp * 0.6;
				}
			}
			
			// Check whether step has become too small
			double sum = 0;
			for(int j = 0;j < d.size;j++)
				sum += abs(t * _d[j]);
			
			if(sum < opt->optTol)
			{
				if(opt->verbose == 3)
					std::cout<<"Line Search failed"<<std::endl;
				t = 0;
				f_new = f;
				g.copyTo(g_new);
				break;
			}
			
			// Evaluate New Point
			for(int j = 0;j < x.size;j++)
				_x_new[j] = _x[j] + _d[j] * t;
			projectBounds(&x_new);
			f_new = funObj(x_new, g_new);
			funEvals++;
			lineSearchIters++;
		}
		
		// Take Step
		x_new.copyTo(x);
		g_new.copyTo(g);
		f = f_new;
		
		// Compute Working Set
		computeWorkingSet();
		
		// Output Log
		double sum = 0;
		double *_g = g.getData();
		for(std::vector<int>::iterator iter = working.begin();iter != working.end();++iter)
			sum +=  abs(_g[*iter]);
		if(opt->verbose >= 2)
			printf("%10d %10d %15.5e %15.5e %15.5e\n", it + 1, funEvals * funEvalMultiplier, t, f, sum);
		
		// Check Optimality
		if(working.empty())
		{
			if(opt->verbose >= 1)
				std::cout<<"All variables are at their bound and no further progress is possible"<<std::endl;
			break;
		}
		else {
			if(norm_L2(g, working) <= opt->optTol)
        	{
				if(opt->verbose >= 1)
					std::cout<<"All working variables satisfy optimality condition."<<std::endl;
				break;
			}
      	}
      	
		// Check for lack of progress
		sum = 0;
		_d = d.getData();
		for(int i = 0;i < d.size;i++)
				sum += abs(t * _d[i]);
		if(sum < opt->optTol)
		{
			if(opt->verbose >= 1)
				std::cout<<"Step size below optTol."<<std::endl;
			break;
		}

		if( abs(f - f_old) < opt->optTol)
		{
			if(opt->verbose >= 1)
				std::cout<<"Function value changing by less than optTol."<<std::endl;
			break;
		}

		if(funEvals * funEvalMultiplier > opt->maxIter)
		{
			if(opt->verbose >= 1)
				std::cout<<"Function Evaluations exceeds maxIter."<<std::endl;
			break;
		}
		it++;
	}
	
	// Release memory
	for(std::deque<Array>::iterator iter = old_dirs.begin();iter != old_dirs.end();++iter)
		iter->release();
	
	for(std::deque<Array>::iterator iter = old_stps.begin();iter != old_stps.end();++iter)
		iter->release();
		
	x_old.release();
	x_new.release();
	g_old.release();
	g_new.release();
	g.release();
	d.release();
}
