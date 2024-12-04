#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<vector>
#include<iomanip>
#include<iterator>
using namespace std;
//定义普通卡类（包含个人账户基本信息）
class Card
{
private:
	friend class ATM;
	long acctnum, psword;                //账户、密码
	long long telnum;                    //电话
	long long idnum;                     //身份证
	string name, addr;                   //姓名、地址
	char sex;                            //性别
protected:
	long GetPsword() { return psword; }
	long GetCard() { return acctnum; }
public:
	Card(long acct, long ps, string n , string a , long long tel , long long id, char s ) :
		acctnum(acct), psword(ps), name(n), addr(a), telnum(tel), idnum(id), sex(s) {}
	~Card();
	void createAcct();//账户创建
	void perfectInfo();//个人信息完善
	void showCard() const;//信息显示        
	bool Match();//判断密码是否匹配，用于账户登录 
	virtual void Deposit() = 0; //即GetCardNo_1()但实际功能是存款，提供接口
	virtual void Withdraw() = 0; //即GetCardNo_2()但实际功能是取款，提供接口
	virtual void queryBalance() = 0;//即GetCardNo_3()但实际功能是查询余额，提供接口                  
};

//储蓄卡账户
class Debit :public Card
{
private:
	friend class ATM;
	double balance;               //账户余额
public:
	Debit(long cardnum , long corenum , double money , string n , string a , long tel , long long id , char s ) :
		Card(cardnum, corenum, n, a, tel, id, s), balance(money) { }
	void Deposit();                                                                                               //存款
	void Withdraw();                                                                                              //取款     
	void queryBalance();                                                                                          //余额查询

};


//信用卡账户
class Credit :public Card
{
private:
	friend class ATM;
	double balance;             //账户余额
	double maxLoan;             //透支额度
	double rate;                //透支利率
	double owes;      
	double commmission;         //取款手续费
public:
	Credit(long cardnum , long corenum ,string n , string a , long long tel , long long id , char s , double money , double loan , double ra , double owe):
		Card(cardnum, corenum, n, a, tel, id, s), balance(money), maxLoan(loan), rate(ra), owes(owe){}
	void Deposit();                                                                     //存款
	void Withdraw();                                                                    //取款
	void Repay();                                                                       //偿还透支
	void queryBalance();                                                               //余额查询

};

//ATM
class ATM
{
private:
	vector<Credit> Credits;        //信用卡账户管理
	vector<Debit> Debits;          //储蓄卡账户管理
protected:
	void debitMenu();               //储蓄卡界面
	void debitOperate(Debit& d);    //储蓄卡操作
	void creditMenu();              //信用卡界面
	void creditOperate(Credit& c);  //信用卡操作
public:
	~ATM();
	void openCardType();     //开户界面
	void closeCardType();     //销户界面
	void createDebit();             //创建储蓄卡账户
	void createCredit();            //创建信用卡账户
	void queryCredit();             //储蓄卡信息查询
	void queryDebit();              //信用卡信息查询
	void deleteDebit();             //删除储蓄卡账户
	void deleteCredit();            //删除信用卡账户
	void loginAcct();               //账户登录
	void menu();                    //主界面
	void ATMfuncation();            //ATM功能
	void disp();                    //开始界面
	void exitdisp();                //结束界面                       
};


void Card::createAcct()
{
	cout << "新建账号:";
	cin >> acctnum;
	cout << "密码:";
	cin >> psword;
}

void Card::perfectInfo()
{
	cout << "请完善账户信息:\n";
	cout << "请输入姓名:";
	cin >> name;
	cout << "请输入电话号码:";
	cin >> telnum;
	cout << "请输入身份证号:";
	cin >> idnum;
	cout << "请输入家庭住址:";
	cin >> addr;
	cout << "请输入性别(m for man, f for female):";
	cin >> sex;
}
void ATM::createDebit()
{
	if (Debits.size() == Debits.max_size()) {
		cout << "储蓄卡账户已达上限。";
		return;
	}
	Debit text(922337203685477580, 922337203685477580, 0, "string", "string", 922337203685477580, 922337203685477580, 's');
	//Debit(long acct , long ps , double bal , string n , string a , long tel , long long id , char s ) :
	text.createAcct();
	vector<Debit>::iterator it;
	for (it = Debits.begin()
		; it != Debits.end(); it++)
	{
		if ((*it).GetCard() == text.acctnum) {
			cout << "账户已存在，新建失败。\n";
			return;
		}
	}
	if (it == Debits.end())
		cout << "账户创建成功。\n";

	//基本信息完善
	text.perfectInfo();
	Debits.push_back(text);
}

void ATM::createCredit()
{
	if (Credits.size() == Credits.max_size()) {
		cout << "信用卡账户已达上限。";
		return;
	}
	Credit text(922337203685477580, 922337203685477580, "string", "string", 922337203685477580, 922337203685477580, 's', 0,0,0,0);
	//初始化text Credit(long ,long ,string ,string ,long long ,long long ,char ,double ,double ,double ,double ) :
	text.createAcct();
	vector<Credit>::iterator it;
	for (it = Credits.begin(); it != Credits.end(); it++)
	{
		if ((*it).GetCard() == text.acctnum) {
			cout << "账户已存在，新建失败。\n";
			return;
		}
	}
	if (it == Credits.end())
		cout << "账户创建成功。\n";
		text.perfectInfo();
		Credits.push_back(text);

}
Card::~Card()
{
	name.clear();
	addr.clear();
}

bool Card::Match()
{
	long pass;
	cout << "请输入密码:";
	cin >> pass;
	int count = 4;
	while ((pass != psword) && count) {
		printf("密码错误，您还剩余%d次机会。\n", count);
		cout << "请重新输入:";
		cin >> pass;
		count--;
	}
	if (count == 0 && pass != psword) {
		cout << "登录失败。\n";
		return false;
	}
	cout << "登录成功。\n";
	return true;
}

void Card::showCard() const
{
	cout << "账户:" << acctnum
		<< "\n姓名:" << name
		<< "\n性别:" << sex
		<< "\n电话:" << telnum
		<< "\n身份证号:" << idnum
		<< "\n住址:" << addr << endl;
}



void Debit::queryBalance()
{
	cout << "账户余额:" << balance << endl;
}

void Debit::Withdraw()
{
	double m = 0;
	cout << "请输入取款金额:";
	cin >> m;
	if (m < 0) {
		cout << "取款金额输入错误" << endl<< "取款失败。"<<endl;
	}
	else {
		if (m > balance) {
			cout << "取款金额超过账户所剩余额\n"
				<< "取款失败。\n";
		}
		else {
			balance -= m;
			cout << "取款成功。\n"
				<< "账户余额:" << balance << endl;
		}
	}
}

void Debit::Deposit()
{
	double m;  //存款金额
	cout << "Debit存款:";
	cout << "请输入存款金额:";
	cin >> m;
	if (m < 0) {
		cout << "存款金额输入错误\n"
			<< "存款取消。\n";
	}
	else {
		cout << "存款利率如下：" << endl;
		cout << "(1)时间<6个月,利率0.011\n"
			<< "(2)6个月<时间<1年,利率0.015\n"
			<< "(3)年<时间,利率0.025\n"
			<< "根据您的存款时间请输入选择:";
		//存款利率选择
		int ans;
		double rat;
		cin >> ans;
		if (ans < 1 && ans>3) {
			cout << "操作错误，存款取消！\n";
			return;
		}
		else if (ans == 1) 
			rat = 0.011;
		else if (ans == 2) 
			rat = 0.015;
		else if (ans == 3)
			rat = 0.025;

		m *= (1 + rat);
		balance += m;
		cout << "存款成功.\n"
			<< "账户余额:" << balance << endl;
	}
}

void Credit::queryBalance()
{
	cout << "账户余额:" << balance
		<< "\t透支额度:" << maxLoan
		<< "\t利率:" << rate
		<< "\t透支金额:" << owes << endl;
}

void Credit::Repay()
{
	double owe; //偿还金额

	cout << "透支金额:" << owes << "\t账户余额:" << balance << endl;
	cout << "请输入偿还金额:";
	cin >> owe;

	cout << "是否使用账户余额偿还透支(1 for Yes, 2 for No):";
	int ans;
	cin >> ans;
	if (ans != 1 && ans != 2) {
		cout << "错误：选择错误，偿还取消。\n";
		return;
	}

	//使用账户余额还款
	else if (ans == 1)
	{
		//账户余额足够
		if (balance >= owe)
		{
			balance -= owe;
			owes -= owe;
			cout << "偿还成功。\n"
				<< "账户余额:" << balance
				<< "\t透支金额:" << owes << endl;
		}
		else
		{
			//账户余额不足
			cout << "账户余额不足，是否将剩余部分补齐(1 for Yes, 2 for No):";
			cin >> ans;
			if (ans == 1)
			{
				balance = 0;
				owes -= owe;
				cout << "偿还成功。"
					<< "账户余额:" << balance
					<< "\t透支金额:" << owes << endl;
			}
			else
			{
				cout << "偿还失败。\n";
			}
		}
	}
}

void Credit::Withdraw()
{
	double m;  //取款金额
	cout << "您需要额外支付1.5%的手续费" << endl;
	cout << "请输入取款金额:";
	cin >> m;

	if (m <= balance * 1.015) {
		balance -= (m*1.015);
		cout << "取款成功。\n"
			<< "账户余额:" << balance << endl;
	}
	else {
		cout << "错误：余额不足，"
			<< "是否透支(1 for Yes, 2 for No):";
		int ans;
		cin >> ans;
		if (ans == 1) {
			if (m > (balance + maxLoan - owes)*1.015) {
				cout << "错误：超出上限，"
					<< "取款失败。\n";
			}
			else {
				double interest;   //透支利息
				m -= balance;
				interest = m * 1.015;
				owes += (m + interest);
				balance = 0;
				cout << "取款成功。\n"
					<< "账户余额:" << balance;
			}
		}
		else
			cout << "错误：取款失败。\n";
	}
}

void Credit::Deposit()
{
	double m;  //存款金额
	cout << "Credit存款:";
	cout << "请输入存款金额:";
	cin >> m;
	if (m < 0) {
		cout << "错误：输入有误\n"
			<< "存款取消。\n";
	}
	else {
		balance += m;
		cout << "存款成功.\n"
			<< "账户余额:" << balance << endl;
	}
}


void ATM::disp()
{

	cout << endl << endl << endl << endl << endl;
	cout << "                                         *******************************" << endl;
	cout << "                                                                      " << endl;
	cout << "                                                欢迎使用ATM机! \t" << endl;
	cout << "                                                                      " << endl;
	cout << "                                         *******************************" << endl << endl;

	system("pause");
}

void ATM::exitdisp()
{

	system("cls");

	cout << "                                         *******************************" << endl;
	cout << "                                         *                             *" << endl;
	cout << "                                         *    感谢您的使用!            *" << endl;
	cout << "                                         *                             *" << endl;
	cout << "                                         *******************************" << endl;
}


void ATM::debitMenu()
{
	system("cls");
	cout << endl << endl << "                        请选择操作序号！(0~4)" << endl;
	cout << "                           **********************************************************" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           *                    0.返回主界面                        *" << endl;
	cout << "                           *                    1.存款                              *" << endl;
	cout << "                           *                    2.取款                              *" << endl;
	cout << "                           *                    3.显示余额                          *" << endl;
	cout << "                           *                    4.个人信息                          *" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           **********************************************************" << endl;
}

void ATM::debitOperate(Debit& d)
{
	//进入用户界面
	debitMenu();

	int choice;
	cout << "              请输入您的选择:";
	cin >> choice;
	while (choice != 0) {
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n";
		switch (choice)
		{
		case 1:
			d.Deposit();
			break;
		case 2:
			d.Withdraw();
			break;
		case 3:
			d.queryBalance();
			break;
		case 4:
			d.showCard();
			break;
		default:
			cout << "错误：输入错误。"<<endl;
		}
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *"<<endl;
		cout << "              请输入您的选择:";
		cin >> choice;
	}
	//返回主界面
	menu();
}

void ATM::creditOperate(Credit& c)
{
	//进入用户界面
	creditMenu();

	int choice;
	cout << "              请输入服务类型:";
	cin >> choice;
	while (choice != 0) {
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n";
		switch (choice)
		{
		case 1:
			c.Deposit();
			break;
		case 2:
			c.Withdraw();
			break;
		case 3:
			c.queryBalance();
			break;
		case 4:
			c.showCard();
			break;
		case 5:
			c.Repay();
			break;
		default:
			cout << "输入错误！\n";
		}
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n\n";
		cout << "              请输入您的选择:";
		cin >> choice;
	}

	//返回主界面
	menu();
}

void ATM::creditMenu()
{
	system("cls");
	cout << endl << endl << endl << endl << "                        请选择操作序号！(0~5)" << endl;
	cout << "                           **********************************************************" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           *                    0.返回主界面                        *" << endl;
	cout << "                           *                    1.存款                              *" << endl;
	cout << "                           *                    2.取款                              *" << endl;
	cout << "                           *                    3.显示余额                          *" << endl;
	cout << "                           *                    4.显示信息                          *" << endl;
	cout << "                           *                    5.偿还透支                          *" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           **********************************************************" << endl;
}

void ATM::loginAcct()
{
	cout << "请输入账号类型：1为储蓄卡 2为信用卡";
	int select;
	cin >> select;
	cout << "请输入账号:";
	long acct;
	cin >> acct;
	if (select == 1) {
		vector<Debit>::iterator it;
		for (it = Debits.begin(); it != Debits.end(); it++)
		{
			if ((*it).GetCard() == acct) break;
		}

		if (it == Debits.end()) {
			cout << "错误：账号不存在。\n";
			return;
		}
		if ((*it).Card::Match())  debitOperate(*it);
	}
	else {
		vector<Credit>::iterator it;
		for (it = Credits.begin(); it != Credits.end(); it++)
		{
			if ((*it).GetCard() == acct) break;
		}
		if (it == Credits.end()) {
			cout << "错误：账号不存在。\n";
			return;
		}
		if ((*it).Card::Match())  creditOperate(*it);
	}
}
void ATM::queryDebit()
{
	cout << "输入查询账号:";
	long acct;
	cin >> acct;
	vector<Debit>::iterator it;
	for (it = Debits.begin(); it != Debits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Debits.end()) {
		cout << "错误：账号不存在。\n";
		return;
	}
	(*it).showCard();
	(*it).queryBalance();
}

void ATM::queryCredit()
{
	cout << "输入查询账号:";
	long acct;
	cin >> acct;
	vector<Credit>::iterator it;
	for (it = Credits.begin(); it != Credits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Credits.end()) {
		cout << "错误：账号不存在。\n";
		return;
	}
	(*it).showCard();
	(*it).queryBalance();
}

void ATM::deleteDebit()
{
	long acct, ps;
	cout << "请输入账号:";
	cin >> acct;
	vector<Debit>::iterator it;
	for (it = Debits.begin(); it != Debits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Debits.end()) {
		cout << "错误：账号不存在。\n";
		return;
	}
	cout << "请输入密码:";
	cin >> ps;
	if (ps != (*it).GetPsword()) {
		cout << "错误：密码错误。\n";
		return;
	}
	cout << "是否确认删除:";
	cout << "是：1 否：2" << endl;
	int ans;
	cin >> ans;
	if (ans == 1) {
		cout << "删除成功。\n";
		Debits.erase(it);
		return;
	}
	else
	{
		cout << "取消成功" << endl;
		return;
	}
}

void ATM::deleteCredit()
{
	long acct, ps;
	cout << "请输入账号:";
	cin >> acct;
	vector<Credit>::iterator it;
	for (it = Credits.begin(); it != Credits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Credits.end()) {
		cout << "账号不存在。\n";
		return;
	}
	cout << "请输入密码:";
	cin >> ps;
	if (ps != (*it).GetPsword()) {
		cout << "错误：密码错误\n";
		return;
	}
	cout << "是否确认删除:";
	char ans;
	cin >> ans;
	if (ans == '否') {
		cout << "取消删除。\n";
		return;
	}
	Credits.erase(it);
	cout << "删除成功。\n";
}



void ATM::menu()
{
	system("cls");
	cout << endl << endl << endl << endl << "                        请选择操作序号！(0~3)" << endl;
	cout << "                           **********************************************************" << endl;
	cout << "                                                                                                                                           " << endl;
	cout << "                                               0.退出                                                                                   " << endl;
	cout << "                                               1.开户					                                                                "<< endl;
	cout << "                                               2.销户					                                                               " << endl;
	cout << "                                               3.账户登录			                                                                   " << endl;
	cout << "                                                                                                                                           " << endl;
	cout << "                           **********************************************************" << endl;
}

void ATM::openCardType()
{
	cout << "输入银行卡类型" << endl;
	cout << "1.储蓄卡" << endl;
	cout << "2.信用卡" << endl;
	int choice;
	cin >> choice;
	switch (choice)
	{
	case 1:
		createDebit();
		break;
	case 2:
		createCredit();
		break;
	}
}
void ATM::closeCardType()
{
	cout << "输入银行卡类型" << endl;
	cout << "1.储蓄卡" << endl;
	cout << "2.信用卡" << endl;
	int choice;
	cin >> choice;
	switch (choice)
	{
	case 1:
		deleteDebit();
		break;
	case 2:
		deleteCredit();
		break;
	}
}
void ATM::ATMfuncation()
{
	menu();
	int choice;
	cout << "              请输入您的选择:";
	cin >> choice;
	while (choice != 0) {
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n";
		switch (choice)
		{
		case 1:
			openCardType();
			break;
		case 2:
			closeCardType();
			break;
		case 3:
			loginAcct();
			break;
		default:
			cout << "错误：输入错误！\n";
		}
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n";
		cout << "              请输入您的选择:";
		cin >> choice;
	}
}



ATM::~ATM()
{
	Credits.clear();
	vector<Credit>().swap(Credits);
	Debits.clear();
	vector<Debit>().swap(Debits);
}

int main() {

	ATM atm;
	atm.disp();
	atm.ATMfuncation();
	atm.exitdisp();

}
