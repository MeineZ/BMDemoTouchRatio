#include <pch.h>
#include <FieldSidesData.h>

FieldSidesData::FieldSidesData(float own, float opponent, float neutral) :
	own(own),
	opponent(opponent),
	neutral(neutral)
{ }

void FieldSidesData::AddOwn( float own )
{
	this->own += own;
}

void FieldSidesData::AddOpponent( float opponent )
{
	this->opponent += opponent;
}

void FieldSidesData::AddNeutral( float neutral )
{
	this->neutral += neutral;
}

float FieldSidesData::GetOwn() const
{
	return own;
}

float FieldSidesData::GetOpponent() const
{
	return opponent;
}

float FieldSidesData::GetNeutral() const
{
	return neutral;
}

float FieldSidesData::GetTotal() const
{
	return own + opponent + neutral;
}