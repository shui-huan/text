#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<vector>
#include<iomanip>
#include<iterator>
using namespace std;
//������ͨ���ࣨ���������˻�������Ϣ��
class Card
{
private:
	friend class ATM;
	long acctnum, psword;                //�˻�������
	long long telnum;                    //�绰
	long long idnum;                     //���֤
	string name, addr;                   //��������ַ
	char sex;                            //�Ա�
protected:
	long GetPsword() { return psword; }
	long GetCard() { return acctnum; }
public:
	Card(long acct, long ps, string n , string a , long long tel , long long id, char s ) :
		acctnum(acct), psword(ps), name(n), addr(a), telnum(tel), idnum(id), sex(s) {}
	~Card();
	void createAcct();//�˻�����
	void perfectInfo();//������Ϣ����
	void showCard() const;//��Ϣ��ʾ        
	bool Match();//�ж������Ƿ�ƥ�䣬�����˻���¼ 
	virtual void Deposit() = 0; //��GetCardNo_1()��ʵ�ʹ����Ǵ��ṩ�ӿ�
	virtual void Withdraw() = 0; //��GetCardNo_2()��ʵ�ʹ�����ȡ��ṩ�ӿ�
	virtual void queryBalance() = 0;//��GetCardNo_3()��ʵ�ʹ����ǲ�ѯ���ṩ�ӿ�                  
};

//����˻�
class Debit :public Card
{
private:
	friend class ATM;
	double balance;               //�˻����
public:
	Debit(long cardnum , long corenum , double money , string n , string a , long tel , long long id , char s ) :
		Card(cardnum, corenum, n, a, tel, id, s), balance(money) { }
	void Deposit();                                                                                               //���
	void Withdraw();                                                                                              //ȡ��     
	void queryBalance();                                                                                          //����ѯ

};


//���ÿ��˻�
class Credit :public Card
{
private:
	friend class ATM;
	double balance;             //�˻����
	double maxLoan;             //͸֧���
	double rate;                //͸֧����
	double owes;      
	double commmission;         //ȡ��������
public:
	Credit(long cardnum , long corenum ,string n , string a , long long tel , long long id , char s , double money , double loan , double ra , double owe):
		Card(cardnum, corenum, n, a, tel, id, s), balance(money), maxLoan(loan), rate(ra), owes(owe){}
	void Deposit();                                                                     //���
	void Withdraw();                                                                    //ȡ��
	void Repay();                                                                       //����͸֧
	void queryBalance();                                                               //����ѯ

};

//ATM
class ATM
{
private:
	vector<Credit> Credits;        //���ÿ��˻�����
	vector<Debit> Debits;          //����˻�����
protected:
	void debitMenu();               //�������
	void debitOperate(Debit& d);    //�������
	void creditMenu();              //���ÿ�����
	void creditOperate(Credit& c);  //���ÿ�����
public:
	~ATM();
	void openCardType();     //��������
	void closeCardType();     //��������
	void createDebit();             //��������˻�
	void createCredit();            //�������ÿ��˻�
	void queryCredit();             //�����Ϣ��ѯ
	void queryDebit();              //���ÿ���Ϣ��ѯ
	void deleteDebit();             //ɾ������˻�
	void deleteCredit();            //ɾ�����ÿ��˻�
	void loginAcct();               //�˻���¼
	void menu();                    //������
	void ATMfuncation();            //ATM����
	void disp();                    //��ʼ����
	void exitdisp();                //��������                       
};


void Card::createAcct()
{
	cout << "�½��˺�:";
	cin >> acctnum;
	cout << "����:";
	cin >> psword;
}

void Card::perfectInfo()
{
	cout << "�������˻���Ϣ:\n";
	cout << "����������:";
	cin >> name;
	cout << "������绰����:";
	cin >> telnum;
	cout << "���������֤��:";
	cin >> idnum;
	cout << "�������ͥסַ:";
	cin >> addr;
	cout << "�������Ա�(m for man, f for female):";
	cin >> sex;
}
void ATM::createDebit()
{
	if (Debits.size() == Debits.max_size()) {
		cout << "����˻��Ѵ����ޡ�";
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
			cout << "�˻��Ѵ��ڣ��½�ʧ�ܡ�\n";
			return;
		}
	}
	if (it == Debits.end())
		cout << "�˻������ɹ���\n";

	//������Ϣ����
	text.perfectInfo();
	Debits.push_back(text);
}

void ATM::createCredit()
{
	if (Credits.size() == Credits.max_size()) {
		cout << "���ÿ��˻��Ѵ����ޡ�";
		return;
	}
	Credit text(922337203685477580, 922337203685477580, "string", "string", 922337203685477580, 922337203685477580, 's', 0,0,0,0);
	//��ʼ��text Credit(long ,long ,string ,string ,long long ,long long ,char ,double ,double ,double ,double ) :
	text.createAcct();
	vector<Credit>::iterator it;
	for (it = Credits.begin(); it != Credits.end(); it++)
	{
		if ((*it).GetCard() == text.acctnum) {
			cout << "�˻��Ѵ��ڣ��½�ʧ�ܡ�\n";
			return;
		}
	}
	if (it == Credits.end())
		cout << "�˻������ɹ���\n";
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
	cout << "����������:";
	cin >> pass;
	int count = 4;
	while ((pass != psword) && count) {
		printf("�����������ʣ��%d�λ��ᡣ\n", count);
		cout << "����������:";
		cin >> pass;
		count--;
	}
	if (count == 0 && pass != psword) {
		cout << "��¼ʧ�ܡ�\n";
		return false;
	}
	cout << "��¼�ɹ���\n";
	return true;
}

void Card::showCard() const
{
	cout << "�˻�:" << acctnum
		<< "\n����:" << name
		<< "\n�Ա�:" << sex
		<< "\n�绰:" << telnum
		<< "\n���֤��:" << idnum
		<< "\nסַ:" << addr << endl;
}



void Debit::queryBalance()
{
	cout << "�˻����:" << balance << endl;
}

void Debit::Withdraw()
{
	double m = 0;
	cout << "������ȡ����:";
	cin >> m;
	if (m < 0) {
		cout << "ȡ�����������" << endl<< "ȡ��ʧ�ܡ�"<<endl;
	}
	else {
		if (m > balance) {
			cout << "ȡ������˻���ʣ���\n"
				<< "ȡ��ʧ�ܡ�\n";
		}
		else {
			balance -= m;
			cout << "ȡ��ɹ���\n"
				<< "�˻����:" << balance << endl;
		}
	}
}

void Debit::Deposit()
{
	double m;  //�����
	cout << "Debit���:";
	cout << "����������:";
	cin >> m;
	if (m < 0) {
		cout << "������������\n"
			<< "���ȡ����\n";
	}
	else {
		cout << "����������£�" << endl;
		cout << "(1)ʱ��<6����,����0.011\n"
			<< "(2)6����<ʱ��<1��,����0.015\n"
			<< "(3)��<ʱ��,����0.025\n"
			<< "�������Ĵ��ʱ��������ѡ��:";
		//�������ѡ��
		int ans;
		double rat;
		cin >> ans;
		if (ans < 1 && ans>3) {
			cout << "�������󣬴��ȡ����\n";
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
		cout << "���ɹ�.\n"
			<< "�˻����:" << balance << endl;
	}
}

void Credit::queryBalance()
{
	cout << "�˻����:" << balance
		<< "\t͸֧���:" << maxLoan
		<< "\t����:" << rate
		<< "\t͸֧���:" << owes << endl;
}

void Credit::Repay()
{
	double owe; //�������

	cout << "͸֧���:" << owes << "\t�˻����:" << balance << endl;
	cout << "�����볥�����:";
	cin >> owe;

	cout << "�Ƿ�ʹ���˻�����͸֧(1 for Yes, 2 for No):";
	int ans;
	cin >> ans;
	if (ans != 1 && ans != 2) {
		cout << "����ѡ����󣬳���ȡ����\n";
		return;
	}

	//ʹ���˻�����
	else if (ans == 1)
	{
		//�˻�����㹻
		if (balance >= owe)
		{
			balance -= owe;
			owes -= owe;
			cout << "�����ɹ���\n"
				<< "�˻����:" << balance
				<< "\t͸֧���:" << owes << endl;
		}
		else
		{
			//�˻�����
			cout << "�˻����㣬�Ƿ�ʣ�ಿ�ֲ���(1 for Yes, 2 for No):";
			cin >> ans;
			if (ans == 1)
			{
				balance = 0;
				owes -= owe;
				cout << "�����ɹ���"
					<< "�˻����:" << balance
					<< "\t͸֧���:" << owes << endl;
			}
			else
			{
				cout << "����ʧ�ܡ�\n";
			}
		}
	}
}

void Credit::Withdraw()
{
	double m;  //ȡ����
	cout << "����Ҫ����֧��1.5%��������" << endl;
	cout << "������ȡ����:";
	cin >> m;

	if (m <= balance * 1.015) {
		balance -= (m*1.015);
		cout << "ȡ��ɹ���\n"
			<< "�˻����:" << balance << endl;
	}
	else {
		cout << "�������㣬"
			<< "�Ƿ�͸֧(1 for Yes, 2 for No):";
		int ans;
		cin >> ans;
		if (ans == 1) {
			if (m > (balance + maxLoan - owes)*1.015) {
				cout << "���󣺳������ޣ�"
					<< "ȡ��ʧ�ܡ�\n";
			}
			else {
				double interest;   //͸֧��Ϣ
				m -= balance;
				interest = m * 1.015;
				owes += (m + interest);
				balance = 0;
				cout << "ȡ��ɹ���\n"
					<< "�˻����:" << balance;
			}
		}
		else
			cout << "����ȡ��ʧ�ܡ�\n";
	}
}

void Credit::Deposit()
{
	double m;  //�����
	cout << "Credit���:";
	cout << "����������:";
	cin >> m;
	if (m < 0) {
		cout << "������������\n"
			<< "���ȡ����\n";
	}
	else {
		balance += m;
		cout << "���ɹ�.\n"
			<< "�˻����:" << balance << endl;
	}
}


void ATM::disp()
{

	cout << endl << endl << endl << endl << endl;
	cout << "                                         *******************************" << endl;
	cout << "                                                                      " << endl;
	cout << "                                                ��ӭʹ��ATM��! \t" << endl;
	cout << "                                                                      " << endl;
	cout << "                                         *******************************" << endl << endl;

	system("pause");
}

void ATM::exitdisp()
{

	system("cls");

	cout << "                                         *******************************" << endl;
	cout << "                                         *                             *" << endl;
	cout << "                                         *    ��л����ʹ��!            *" << endl;
	cout << "                                         *                             *" << endl;
	cout << "                                         *******************************" << endl;
}


void ATM::debitMenu()
{
	system("cls");
	cout << endl << endl << "                        ��ѡ�������ţ�(0~4)" << endl;
	cout << "                           **********************************************************" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           *                    0.����������                        *" << endl;
	cout << "                           *                    1.���                              *" << endl;
	cout << "                           *                    2.ȡ��                              *" << endl;
	cout << "                           *                    3.��ʾ���                          *" << endl;
	cout << "                           *                    4.������Ϣ                          *" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           **********************************************************" << endl;
}

void ATM::debitOperate(Debit& d)
{
	//�����û�����
	debitMenu();

	int choice;
	cout << "              ����������ѡ��:";
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
			cout << "�����������"<<endl;
		}
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *"<<endl;
		cout << "              ����������ѡ��:";
		cin >> choice;
	}
	//����������
	menu();
}

void ATM::creditOperate(Credit& c)
{
	//�����û�����
	creditMenu();

	int choice;
	cout << "              �������������:";
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
			cout << "�������\n";
		}
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n\n";
		cout << "              ����������ѡ��:";
		cin >> choice;
	}

	//����������
	menu();
}

void ATM::creditMenu()
{
	system("cls");
	cout << endl << endl << endl << endl << "                        ��ѡ�������ţ�(0~5)" << endl;
	cout << "                           **********************************************************" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           *                    0.����������                        *" << endl;
	cout << "                           *                    1.���                              *" << endl;
	cout << "                           *                    2.ȡ��                              *" << endl;
	cout << "                           *                    3.��ʾ���                          *" << endl;
	cout << "                           *                    4.��ʾ��Ϣ                          *" << endl;
	cout << "                           *                    5.����͸֧                          *" << endl;
	cout << "                           *                                                        *" << endl;
	cout << "                           **********************************************************" << endl;
}

void ATM::loginAcct()
{
	cout << "�������˺����ͣ�1Ϊ��� 2Ϊ���ÿ�";
	int select;
	cin >> select;
	cout << "�������˺�:";
	long acct;
	cin >> acct;
	if (select == 1) {
		vector<Debit>::iterator it;
		for (it = Debits.begin(); it != Debits.end(); it++)
		{
			if ((*it).GetCard() == acct) break;
		}

		if (it == Debits.end()) {
			cout << "�����˺Ų����ڡ�\n";
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
			cout << "�����˺Ų����ڡ�\n";
			return;
		}
		if ((*it).Card::Match())  creditOperate(*it);
	}
}
void ATM::queryDebit()
{
	cout << "�����ѯ�˺�:";
	long acct;
	cin >> acct;
	vector<Debit>::iterator it;
	for (it = Debits.begin(); it != Debits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Debits.end()) {
		cout << "�����˺Ų����ڡ�\n";
		return;
	}
	(*it).showCard();
	(*it).queryBalance();
}

void ATM::queryCredit()
{
	cout << "�����ѯ�˺�:";
	long acct;
	cin >> acct;
	vector<Credit>::iterator it;
	for (it = Credits.begin(); it != Credits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Credits.end()) {
		cout << "�����˺Ų����ڡ�\n";
		return;
	}
	(*it).showCard();
	(*it).queryBalance();
}

void ATM::deleteDebit()
{
	long acct, ps;
	cout << "�������˺�:";
	cin >> acct;
	vector<Debit>::iterator it;
	for (it = Debits.begin(); it != Debits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Debits.end()) {
		cout << "�����˺Ų����ڡ�\n";
		return;
	}
	cout << "����������:";
	cin >> ps;
	if (ps != (*it).GetPsword()) {
		cout << "�����������\n";
		return;
	}
	cout << "�Ƿ�ȷ��ɾ��:";
	cout << "�ǣ�1 ��2" << endl;
	int ans;
	cin >> ans;
	if (ans == 1) {
		cout << "ɾ���ɹ���\n";
		Debits.erase(it);
		return;
	}
	else
	{
		cout << "ȡ���ɹ�" << endl;
		return;
	}
}

void ATM::deleteCredit()
{
	long acct, ps;
	cout << "�������˺�:";
	cin >> acct;
	vector<Credit>::iterator it;
	for (it = Credits.begin(); it != Credits.end(); it++)
	{
		if ((*it).GetCard() == acct) break;
	}
	if (it == Credits.end()) {
		cout << "�˺Ų����ڡ�\n";
		return;
	}
	cout << "����������:";
	cin >> ps;
	if (ps != (*it).GetPsword()) {
		cout << "�����������\n";
		return;
	}
	cout << "�Ƿ�ȷ��ɾ��:";
	char ans;
	cin >> ans;
	if (ans == '��') {
		cout << "ȡ��ɾ����\n";
		return;
	}
	Credits.erase(it);
	cout << "ɾ���ɹ���\n";
}



void ATM::menu()
{
	system("cls");
	cout << endl << endl << endl << endl << "                        ��ѡ�������ţ�(0~3)" << endl;
	cout << "                           **********************************************************" << endl;
	cout << "                                                                                                                                           " << endl;
	cout << "                                               0.�˳�                                                                                   " << endl;
	cout << "                                               1.����					                                                                "<< endl;
	cout << "                                               2.����					                                                               " << endl;
	cout << "                                               3.�˻���¼			                                                                   " << endl;
	cout << "                                                                                                                                           " << endl;
	cout << "                           **********************************************************" << endl;
}

void ATM::openCardType()
{
	cout << "�������п�����" << endl;
	cout << "1.���" << endl;
	cout << "2.���ÿ�" << endl;
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
	cout << "�������п�����" << endl;
	cout << "1.���" << endl;
	cout << "2.���ÿ�" << endl;
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
	cout << "              ����������ѡ��:";
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
			cout << "�����������\n";
		}
		cout << "*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *\n";
		cout << "              ����������ѡ��:";
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
