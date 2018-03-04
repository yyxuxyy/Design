#include "material.h"
#include <cmath>
#include "xxt.h"

Material::Material(){}

Material::~Material(){}

void Material::setName(double name)
{
	this->name = name;
	updateInfo();
}

double Material::getName() const{return name;}

double Material::get_E() const{return E;}

double Material::get_G() const{return G;}

double Material::get_��() const{return ��;}

double Material::get_��c() const{return ��c;}

double Material::get_��() const{return ��;}

//*------------------------------------*//

//[]explicitֻ������������
//explicit Concrete::Concrete(double name)
Concrete::Concrete(double name)
{
	setName(name);
}

Concrete::~Concrete(){}

double Concrete::get_fcuk() const{return fcuk;}

double Concrete::get_fck() const{return fck;}

double Concrete::get_ftk() const{return ftk;}

double Concrete::get_fc() const{return fc;}

double Concrete::get_ft() const{return ft;}

double Concrete::get_fcc() const{return fcc;}

double Concrete::get_fct() const{return fct;}

double Concrete::get_��0() const{return ��0;}

double Concrete::get_��cu() const{return ��cu;}

double Concrete::get_��1() const{return ��1;}

double Concrete::get_��1() const{return ��1;}

void Concrete::updateInfo()
{
	fcuk = name;
	E = 1e5 / (2.2 + 34.7 / fcuk);
	G = 0.4 * E;
	�� = 0.2;
	��c = 1e-5;
	�� = 25;
	fck = 0.88 * calc_��c1() * calc_��c2() * fcuk;
	ftk = 0.88 * 0.395 * pow(fcuk, 0.55) * pow((1 - 1.645 * calc_��()), 0.45) * calc_��c2();
	fc = fck / 1.4;
	ft = ftk / 1.4;
	fcc = 0.85 * fc;
	fct = 0.55 * ft;
	��0 = calc_��0();
	��cu = calc_��cu();
	��1 = calc_��1();
	��1 = calc_��1();
}

double Concrete::calc_��c1() const
{
	//C50������0.76��C80ȡ0.82���м��ڲ�
	return interpolation(50, 0.76, 80, 0.82, fcuk);
}

double Concrete::calc_��c2() const
{
	//C40������1.00��C80ȡ0.87���м��ڲ�
	return interpolation(40, 1.00, 80, 0.87, fcuk);
}

double Concrete::calc_��() const
{
	//C40������0.12��C80ȡ0.08���м��ڲ�//[]��ȡֵ����C25�����µĽ����淶�Բ���
	return interpolation(40, 0.12, 80, 0.08, fcuk);
}

double Concrete::calc_��0() const
{
	double result = 0.002 + 0.5 * (fcuk - 50) / 100000;
	return result < 0.002 ? 0.002 : result;
}

double Concrete::calc_��cu() const
{
	double result = 0.0033 - (fcuk - 50) / 100000;
	return result > 0.0033 ? 0.0033 : result;
}

double Concrete::calc_��1() const
{
	return interpolation(50, 1.0, 80, 0.94, fcuk);
}

double Concrete::calc_��1() const
{
	return interpolation(50, 0.8, 80, 0.74, fcuk);
}

//*------------------------------------*//

Rebar::Rebar(double name){
	setName(name);
}

Rebar::~Rebar(){}

double Rebar::get_fy() const{ return fy; }

double Rebar::get_fy_c() const{ return fy_c; }

double Rebar::get_fyv() const{ return fyv; }

void Rebar::updateInfo(){
	E = name <= 300 + EPSILON ? 2.1e5 : 2e5;//[XXT]double�ܴ���һЩ��double��Ҫ���о�ȷ�ıȽϣ���double���300ʵ�ʿ���Ϊ300.00000001����ʱ��Ҫ��300�ȣ�Ҫ��300.0001��
	G = 0.4 * E;//[]��ȷ��
	�� = 0.3;//[]��ȷ��
	��c = 1.2e-5;//[]��ȷ��
	�� = 78;
	fy = calc_fy();
	fy_c = calc_fy_c();
	fyv = calc_fyv(); 
}

double Rebar::calc_fy() const
{
	return name < 500 - EPSILON ? name / 1.1 : name / 1.15;
}
double Rebar::calc_fy_c() const{
	return name < 500 - EPSILON ? name / 1.1 : name / 1.2;//[]��ȷ��
}

double Rebar::calc_fyv() const{
	return fy > 360 ? 360 : fy;
}