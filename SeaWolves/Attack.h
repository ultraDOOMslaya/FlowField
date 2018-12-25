#pragma once
class Attack
{
public:
	Attack();
	~Attack();

	virtual int getMaxRange() = 0;

protected:
	int maxRange;
};

class Wack : public Attack
{
public:
	int getMaxRange() {
		return 10;
	}
};

class Shoot : public Attack
{
public:
	int getMaxRange() {
		return 200;
	}
};

class Evocate : public Attack
{
public:
	int getMaxRange() {
		return 150;
	}
};