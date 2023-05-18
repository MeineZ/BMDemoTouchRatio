/*
 * Copyright 2023 MeineZ@GitHub

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

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