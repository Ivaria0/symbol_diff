#include <iostream>
#include <string>


class Expression
{
public:
	virtual Expression *diff() = 0;
	virtual void print() = 0;
};

class Number : public Expression
{
	double number;
public:
	Number(double n) : number(n) {}
	Expression *diff()
	{
		return new Number(0);
	}
	void print()
	{
		std::cout << number;
	}
};

class Variable : public Expression
{
	char variable;
public:
	Variable(char v) : variable(v) {}
	Expression *diff()
	{
		return new Number(1);
	}
	void print()
	{
		std::cout << variable;
	}
};

class Add : public Expression
{
	Expression *first, *second;
public:
	Add(Expression *f, Expression *s) : first(f), second(s) {}
	Expression *diff()
	{
		return new Add(first->diff(), second->diff());
	}
	void print()
	{
		std::cout << "(";
		first->print();
		std::cout << "+";
		second->print();
		std::cout << ")";
	}
};

class Sub : public Expression
{
	Expression *first, *second;
public:
	Sub(Expression *f, Expression *s) : first(f), second(s) {}
	Expression *diff()
	{
		return new Sub(first->diff(), second->diff());
	}
	void print()
	{
		std::cout << "(";
		first->print();
		std::cout << "-";
		second->print();
		std::cout << ")";
	}
};

class Mul : public Expression
{
	Expression *first, *second;
public:
	Mul(Expression *f, Expression *s) : first(f), second(s) {}
	Expression *diff()
	{
		return new Add(new Mul(first->diff(), second), new Mul(first, second->diff()));
	}
	void print()
	{
		std::cout << "(";
		first->print();
		std::cout << "*";
		second->print();
		std::cout << ")";
	}
};

class Div : public Expression
{
	Expression *first, *second;
public:
	Div(Expression *f, Expression *s) : first(f), second(s) {}
	Expression *diff()
	{
		return new Div(new Sub(new Mul(first->diff(), second), new Mul(first, second->diff())), new Mul(second, second));
	}
	void print()
	{
		std::cout << "(";
		first->print();
		std::cout << "/";
		second->print();
		std::cout << ")";
	}
};

Expression *read_expression(std::string a)
{
	Expression *e;
	if (a[0] != '(') {
		if (((a[0] >= '0') && (a[0] <= '9')) || (a[0] == '.')) {
			double n = 0;
			int i = 0;
			bool point = false;
			int r = 1;
			while (i < a.length()) {
				if (point) r *= 10;
				if (a[i] != '.') {
					n *= 10;
					n += (int)(a[i] - '0');
				}
				else point = true;
				i++;
			}
			e = new Number(n / r);
		}
		else e = new Variable(a[0]);
	}
	else {
		int s = 1;
		int i = 1;
		if (a[i] == '(') {
			s++;
			while (s > 1) {
				i++;
				if (a[i] == '(') s++;
				if (a[i] == ')') s--;
			}
			i++;
		}
		else {
			while ((a[i] != '+') && (a[i] != '*') && (a[i] != '/')) i++;
		}
		int j = 1;
		s = 0;
		if (a[i + j] == '(') {
			s++;
			while (s > 0) {
				j++;
				if (a[i + j] == '(') s++;
				if (a[i + j] == ')') s--;
			}
		}
		else {
			while (a[i + j] != ')') j++;
			j--;
		}
		std::string b(a, 1, i - 1);
		std::string c(a, i + 1, j);
		Expression *e1 = read_expression(b);
		Expression *e2 = read_expression(c);
		switch (a[i]){
		case '+': e = new Add(e1, e2); break;
		case '-': e = new Sub(e1, e2); break;
		case '*': e = new Mul(e1, e2); break;
		case '/': e = new Div(e1, e2); break;
		default: e = new Number(0); break;
		}
	}
	return e;
}

int main()
{
	Expression *e = new Add(new Number(1), new Variable('x'));
	std::cout << "\n";
	Expression *de = e->diff();
	de->print();
	std::cout << "\n";
	Expression *m = new Mul(new Variable('x'), e);
	Expression *dm = m->diff();
	dm->print();
	std::cout << "\n";
	delete e;
	delete de;
	delete m;
	delete dm;
	Expression *k = new Div(new Variable('x'), new Add(new Number(1), new Mul(new Variable('x'), new Variable('x'))));
	Expression *dk = k->diff();
	dk->print();
	delete k;
	delete dk;
	std::cout << "\n";
	Expression *e = read_expression("((x+1)/x)");
	e->print();
	std::cout << "\n";
	Expression *de = e->diff();
	de->print();
	std::cout << "\n";
	delete e;
	delete de;
}